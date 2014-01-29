#include "PointLight.h"

ID3D11Buffer* PointLight::m_pPtLightConstBuffer = nullptr;
ID3D11ClassInstance* PointLight::m_pPtLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer
const UINT g_PointLightActualByteWidth = (sizeof(PointLight) - (sizeof(void*) + sizeof(Matrix4f)));
const UINT g_PointLightConstByteWidth = (g_PointLightActualByteWidth + (16 - g_PointLightActualByteWidth % 16));

PointLight::PointLight(void) : m_fRadius(1.0f)
{
	
}


PointLight::~PointLight(void)
{
}

void PointLight::CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName)
{
	// double check to make sure we don't leak memory
	if(!m_pPtLightConstBuffer && pDevice)
	{
		D3D11_BUFFER_DESC ptLightDesc;
		ZeroMemory(&ptLightDesc, sizeof(D3D11_BUFFER_DESC));
		ptLightDesc.Usage = D3D11_USAGE_DYNAMIC;
		ptLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ptLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ptLightDesc.ByteWidth = g_PointLightConstByteWidth;

		if(FAILED(pDevice->CreateBuffer(&ptLightDesc, NULL, &m_pPtLightConstBuffer)))
			MessageBox(NULL, L"Failed to create Point Light Constant Buffer" , L"", MB_OK | MB_ICONERROR);
	}

	if(!m_pPtLightClassInstance && m_pLightClassLinkage)
	{
		if(FAILED(m_pLightClassLinkage->GetClassInstance(szClassInstanceName, 0, &m_pPtLightClassInstance)))
			MessageBox(NULL, L"Failed to create Point Light Class Instance" , L"", MB_OK | MB_ICONERROR);
	}
}

void PointLight::UpdateConstantBuffer(ID3D11DeviceContext* pContext)
{
	if(pContext)
	{
		D3D11_MAPPED_SUBRESOURCE lightSubresource;

		pContext->Map(m_pPtLightConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &lightSubresource);
			// NOTE: Copy from first data member in the class to avoid mem copying the vtable pointer
			memcpy(lightSubresource.pData, &m_vColor, g_PointLightActualByteWidth);
		pContext->Unmap(m_pPtLightConstBuffer, 0);
	}
}

void PointLight::ReleaseConstantBufferAndClassLinkage()
{
	SAFE_RELEASE(m_pPtLightConstBuffer);
	SAFE_RELEASE(m_pPtLightClassInstance);
}

void PointLight::InitializeViewProjMatrix(const Camera* pCam)
{

}

void PointLight::SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam)
{

}

void PointLight::UpdateViewProjMatrix(const Camera* pCam)
{
	m_ProjectionMatrix.MakePerspective(PI_OVER_2, 1.0f, 0.1f, m_fRadius);

	Matrix4f worldMatrix;
	worldMatrix.position = m_vPosition;

	// setup Positive-Z View Matrix
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_POSITIVE_Z] = MatrixInverse(worldMatrix);

	// setup Negative-Z View Matrix
	worldMatrix.xAxis = Vec3f(-1.0f, 0.0f, 0.0f);
	worldMatrix.zAxis = Vec3f(0.0f, 0.0f, -1.0f);
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_NEGATIVE_Z] = MatrixInverse(worldMatrix);

	// setup Positive-X View Matrix
	worldMatrix.xAxis = Vec3f(0.0f, 0.0f, -1.0f);
	worldMatrix.zAxis = Vec3f(1.0f, 0.0f, 0.0f);
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_POSITIVE_X] = MatrixInverse(worldMatrix);

	// setup Negative-X View Matrix
	worldMatrix.xAxis = Vec3f(0.0f, 0.0f, 1.0f);
	worldMatrix.zAxis = Vec3f(-1.0f, 0.0f, 0.0f);
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_NEGATIVE_X] = MatrixInverse(worldMatrix);

	// setup Positive-Y View Matrix
	worldMatrix.xAxis = Vec3f(1.0f, 0.0f, 0.0f);
	worldMatrix.yAxis = Vec3f(0.0f, 0.0f, -1.0f);
	worldMatrix.zAxis = Vec3f(0.0f, 1.0f, 0.0f);
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_POSITIVE_Y] = MatrixInverse(worldMatrix);

	// setup Negative-Y View Matrix
	worldMatrix.yAxis = Vec3f(0.0f, 0.0f, 1.0f);
	worldMatrix.zAxis = Vec3f(0.0f, -1.0f, 0.0f);
	m_ViewMatrices[D3D11_TEXTURECUBE_FACE_NEGATIVE_Y] = MatrixInverse(worldMatrix);
}