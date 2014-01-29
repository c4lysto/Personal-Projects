#include "SpotLight.h"
#include "Camera.h"

ID3D11Buffer* SpotLight::m_pSpotLightConstBuffer = nullptr;
ID3D11ClassInstance* SpotLight::m_pSpotLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer
const UINT g_SpotLightActualByteWidth = (sizeof(SpotLight) - (sizeof(void*) + (1 * sizeof(Matrix4f))));
const UINT g_SpotLightConstByteWidth = (g_SpotLightActualByteWidth + (16 - (g_SpotLightActualByteWidth % 16)));

void SpotLight::CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName)
{
	if(!m_pSpotLightConstBuffer && pDevice)
	{
		D3D11_BUFFER_DESC dirLightDesc;
		ZeroMemory(&dirLightDesc, sizeof(D3D11_BUFFER_DESC));
		dirLightDesc.Usage = D3D11_USAGE_DYNAMIC;
		dirLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dirLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dirLightDesc.ByteWidth = g_SpotLightConstByteWidth;

		if(FAILED(pDevice->CreateBuffer(&dirLightDesc, NULL, &m_pSpotLightConstBuffer)))
			MessageBox(NULL, L"Failed to create Spot Light Constant Buffer" , L"", MB_OK | MB_ICONERROR);
	}

	if(!m_pSpotLightClassInstance && m_pLightClassLinkage)
	{
		if(FAILED(m_pLightClassLinkage->GetClassInstance(szClassInstanceName, 0, &m_pSpotLightClassInstance)))
			MessageBox(NULL, L"Failed to create Spot Light Class Instance" , L"", MB_OK | MB_ICONERROR);
	}
}

void SpotLight::UpdateConstantBuffer(ID3D11DeviceContext* pContext)
{
	if(pContext)
	{
		D3D11_MAPPED_SUBRESOURCE lightSubresource;

		pContext->Map(m_pSpotLightConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &lightSubresource);
			// NOTE: Copy from first data member in the class to avoid mem copying the vtable pointer
			memcpy(lightSubresource.pData, &m_vColor, g_SpotLightActualByteWidth);
		pContext->Unmap(m_pSpotLightConstBuffer, 0);
	};
}

void SpotLight::ReleaseConstantBufferAndClassLinkage()
{
	SAFE_RELEASE(m_pSpotLightConstBuffer);
	SAFE_RELEASE(m_pSpotLightClassInstance);
}

void SpotLight::InitializeViewProjMatrix(const Camera* pCam)
{

}

void SpotLight::BuildViewAndProjectionMatrices()
{
	Matrix4f worldMat;
	worldMat.zAxis = m_vDirection;

	worldMat.xAxis = CrossProduct(g_WorldUp, worldMat.zAxis);
	worldMat.xAxis.normalize();

	worldMat.yAxis = CrossProduct(worldMat.zAxis, worldMat.xAxis);
	worldMat.yAxis.normalize();

	worldMat.position = m_vPosition;

	m_mViewMatrix = MatrixInverse(worldMat);

	Matrix4f projMatrix;
	projMatrix.MakePerspective(acos(m_fOuterConeRatio) * 2.0f, 1.0f, 0.1f, m_fRadius);

	m_mViewProjMatrix = m_mViewMatrix * projMatrix;

	Vec4f point = Vec4f(0.0f, 0.0f, 0.0f, 1.0f) * m_mViewProjMatrix;

	float texelX = point.x * SHADOW_RESOLUTION * 0.5f;
	float texelY = point.y * SHADOW_RESOLUTION * 0.5f;

	float dX = floor(texelX + 0.5f) - texelX;
	float dY = floor(texelY + 0.5f) - texelY;

	dX /= SHADOW_RESOLUTION * 0.5f;
	dY /= SHADOW_RESOLUTION * 0.5f;

	m_mViewProjMatrix.Translate(dX, dY, 0.0f);

	Matrix4f texOffset;
	texOffset.MakeTextureMatrixOffsetLH(SHADOW_RESOLUTION, SHADOW_RESOLUTION);

	m_mViewProjectionTexMatrix = m_mViewProjMatrix * texOffset;
}

void SpotLight::SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam)
{
	pCam->SetShaderObjectBuffer(mMatrix, mMatrix * m_mViewMatrix, mMatrix * m_mViewProjMatrix);
}