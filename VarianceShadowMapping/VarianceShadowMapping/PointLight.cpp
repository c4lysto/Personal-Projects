#include "PointLight.h"

ID3D11Buffer* PointLight::m_pPtLightConstBuffer = nullptr;
ID3D11ClassInstance* PointLight::m_pPtLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer
const UINT g_PointLightActualByteWidth = (sizeof(PointLight) - sizeof(void*));
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