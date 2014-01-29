#include "DirectionalLight.h"
#include "Camera.h"

ID3D11Buffer* DirectionalLight::m_pDirLightConstBuffer = nullptr;
ID3D11ClassInstance* DirectionalLight::m_pDirLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer								// represents View, Proj, Matrices
const UINT g_DirLightActualByteWidth = (sizeof(DirectionalLight) - (sizeof(void*) + (1 * sizeof(Matrix4f))));
const UINT g_DirLightConstByteWidth = (g_DirLightActualByteWidth + (16 - (g_DirLightActualByteWidth % 16)));

void DirectionalLight::CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName)
{
	if(!m_pDirLightConstBuffer && pDevice)
	{
		D3D11_BUFFER_DESC dirLightDesc;
		ZeroMemory(&dirLightDesc, sizeof(D3D11_BUFFER_DESC));
		dirLightDesc.Usage = D3D11_USAGE_DYNAMIC;
		dirLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dirLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dirLightDesc.ByteWidth = g_DirLightConstByteWidth;

		if(FAILED(pDevice->CreateBuffer(&dirLightDesc, NULL, &m_pDirLightConstBuffer)))
			MessageBox(NULL, L"Failed to create Directional Light Constant Buffer" , L"", MB_OK | MB_ICONERROR);
	}

	if(!m_pDirLightClassInstance && m_pLightClassLinkage)
	{
		if(FAILED(m_pLightClassLinkage->GetClassInstance(szClassInstanceName, 0, &m_pDirLightClassInstance)))
			MessageBox(NULL, L"Failed to create Directional Light Class Instance" , L"", MB_OK | MB_ICONERROR);
	}
}

void DirectionalLight::UpdateConstantBuffer(ID3D11DeviceContext* pContext)
{
	if(pContext)
	{
		D3D11_MAPPED_SUBRESOURCE lightSubresource;

		pContext->Map(m_pDirLightConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &lightSubresource);
			// NOTE: Copy from first data member in the class to avoid mem copying the vtable pointer
			memcpy(lightSubresource.pData, &m_vColor, g_DirLightActualByteWidth);
		pContext->Unmap(m_pDirLightConstBuffer, 0);
	};
}

void DirectionalLight::ReleaseConstantBufferAndClassLinkage()
{
	SAFE_RELEASE(m_pDirLightConstBuffer);
	SAFE_RELEASE(m_pDirLightClassInstance);
}

void DirectionalLight::InitializeViewProjMatrix(const Camera* pCam)
{
	Sphere sphere;
	sphere.Build(pCam->GetFrustum().vCorners, NUM_FRUSTUM_CORNERS);

	float3 radiusVec(sphere.fRadius, sphere.fRadius, sphere.fRadius);
	AABB aabb = {sphere.vCenter - radiusVec, sphere.vCenter + radiusVec};


	m_fNearClip = max(0, aabb.vMinPoint.z);
	m_fFarClip = aabb.vMaxPoint.z;

	m_ProjectionMatrix.MakeOrthographic(aabb.vMaxPoint.x - aabb.vMinPoint.x, 
			aabb.vMaxPoint.y - aabb.vMinPoint.y, m_fNearClip, m_fFarClip);


	m_ViewMatrix.zAxis = m_vDirection;

	Vec3f& zAxis = m_ViewMatrix.zAxis;
	zAxis.normalize();

	Vec3f& xAxis = m_ViewMatrix.xAxis;
	xAxis = CrossProduct(g_IdentityY3, zAxis);
	xAxis.normalize();

	Vec3f& yAxis = m_ViewMatrix.yAxis;
	yAxis = CrossProduct(zAxis, xAxis);
	yAxis.normalize();

	m_ViewMatrix.OrthoNormalInvert();

	UpdateViewProjMatrix(pCam);
}

void DirectionalLight::CalculateProjectionMatrix(const Frustum& camFrustum)
{
	AABB boundingVolume = {Vec3f(FLT_MAX, FLT_MAX, FLT_MAX), Vec3f(FLT_MIN, FLT_MIN, FLT_MIN)};
	//const Matrix4fA modelViewMatrix = pCam->GetWorldMatrix() * m_ViewMatrix;

	for(unsigned int i = 0; i < NUM_FRUSTUM_CORNERS; ++i)
	{
		Vec4fA transformedCorner(camFrustum.vCorners[i], 1.0f);

		transformedCorner *= m_ViewMatrix;

		if(transformedCorner.x < boundingVolume.vMinPoint.x)
			boundingVolume.vMinPoint.x = transformedCorner.x;
		else if(transformedCorner.x > boundingVolume.vMaxPoint.x)
			boundingVolume.vMaxPoint.x = transformedCorner.x;

		if(transformedCorner.y < boundingVolume.vMinPoint.y)
			boundingVolume.vMinPoint.y = transformedCorner.y;
		else if(transformedCorner.y > boundingVolume.vMaxPoint.y)
			boundingVolume.vMaxPoint.y = transformedCorner.y;

		if(transformedCorner.z < boundingVolume.vMinPoint.z)
			boundingVolume.vMinPoint.z = transformedCorner.z;
		else if(transformedCorner.z > boundingVolume.vMaxPoint.z)
			boundingVolume.vMaxPoint.z = transformedCorner.z;
	}

	m_fNearClip = boundingVolume.vMinPoint.z;
	m_fFarClip = boundingVolume.vMaxPoint.z;

	m_ProjectionMatrix.MakeOrthographic(boundingVolume.vMaxPoint.x - boundingVolume.vMinPoint.x, 
			boundingVolume.vMaxPoint.y - boundingVolume.vMinPoint.y, m_fNearClip, m_fFarClip);
}

void DirectionalLight::UpdateViewProjMatrix(const Camera* pCam)
{
	Sphere sphere;
	sphere.Build(pCam->GetFrustum().vCorners, NUM_FRUSTUM_CORNERS);

	Vec3f Pos = sphere.vCenter - (m_vDirection * sphere.fRadius);

	Vec3f& vViewPos = m_ViewMatrix.position;

	vViewPos.x = -(Vec3f(m_ViewMatrix.Xx, m_ViewMatrix.Yx, m_ViewMatrix.Zx).dot_product(Pos));
	vViewPos.y = -(Vec3f(m_ViewMatrix.Xy, m_ViewMatrix.Yy, m_ViewMatrix.Zy).dot_product(Pos));
	vViewPos.z = -(Vec3f(m_ViewMatrix.Xz, m_ViewMatrix.Yz, m_ViewMatrix.Zz).dot_product(Pos));

	//CalculateProjectionMatrix(pCam->GetFrustum());

	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;

	Vec4f point = Vec4f(0.0f, 0.0f, 0.0f, 1.0f) * m_ViewProjectionMatrix;

	float texelX = point.x * SHADOW_RESOLUTION * 0.5f;
	float texelY = point.y * SHADOW_RESOLUTION * 0.5f;

	float dX = floor(texelX + 0.5f) - texelX;
	float dY = floor(texelY + 0.5f) - texelY;

	dX /= SHADOW_RESOLUTION * 0.5f;
	dY /= SHADOW_RESOLUTION * 0.5f;

	m_ViewProjectionMatrix.Translate(dX, dY, 0.0f);

	Matrix4f texOffset;	
	texOffset.MakeTextureMatrixOffsetLH(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	m_ViewProjTextureMatrix = m_ViewProjectionMatrix * texOffset;
}

void DirectionalLight::SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam)
{
	pCam->SetShaderObjectBuffer(mMatrix, mMatrix * m_ViewMatrix, mMatrix * m_ViewProjectionMatrix);
}