#include "SpotLight.h"

ID3D11Buffer* SpotLight::m_pSpotLightConstBuffer = nullptr;
ID3D11ClassInstance* SpotLight::m_pSpotLightClassInstance = nullptr;

const UINT g_dirLightConstByteWidth = (sizeof(SpotLight) + (16 - (sizeof(SpotLight) % 16)));

void SpotLight::CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName)
{
	if(!m_pSpotLightConstBuffer && pDevice)
	{
		D3D11_BUFFER_DESC dirLightDesc;
		ZeroMemory(&dirLightDesc, sizeof(D3D11_BUFFER_DESC));
		dirLightDesc.Usage = D3D11_USAGE_DYNAMIC;
		dirLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dirLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dirLightDesc.ByteWidth = g_dirLightConstByteWidth;

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
			memcpy(lightSubresource.pData, &m_vColor, sizeof(SpotLight));
		pContext->Unmap(m_pSpotLightConstBuffer, 0);
	};
}

void SpotLight::ReleaseConstantBufferAndClassLinkage()
{
	SAFE_RELEASE(m_pSpotLightConstBuffer);
	SAFE_RELEASE(m_pSpotLightClassInstance);
}