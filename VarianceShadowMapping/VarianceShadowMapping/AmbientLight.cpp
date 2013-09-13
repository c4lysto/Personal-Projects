#include "AmbientLight.h"

ID3D11Buffer* AmbientLight::m_pAmbientLightConstBuffer = nullptr;
ID3D11ClassInstance* AmbientLight::m_pAmbientLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer
const UINT g_AmbientLightConstByteWidth = (sizeof(AmbientLight) + (16 - sizeof(AmbientLight) % 16));

void AmbientLight::CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName)
{
	// double check to make sure we don't leak memory
	if(!m_pAmbientLightConstBuffer && pDevice)
	{
		D3D11_BUFFER_DESC ptLightDesc;
		ZeroMemory(&ptLightDesc, sizeof(D3D11_BUFFER_DESC));
		ptLightDesc.Usage = D3D11_USAGE_DYNAMIC;
		ptLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ptLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ptLightDesc.ByteWidth = g_AmbientLightConstByteWidth;

		if(FAILED(pDevice->CreateBuffer(&ptLightDesc, NULL, &m_pAmbientLightConstBuffer)))
			MessageBox(NULL, L"Failed to create Ambient Light Constant Buffer" , L"", MB_OK | MB_ICONERROR);
	}

	if(!m_pAmbientLightClassInstance && ILight::m_pLightClassLinkage)
	{
		if(FAILED(ILight::m_pLightClassLinkage->GetClassInstance(szClassInstanceName, 0, &m_pAmbientLightClassInstance)))
			MessageBox(NULL, L"Failed to create Ambient Light Class Instance" , L"", MB_OK | MB_ICONERROR);
	}
}

void AmbientLight::UpdateConstantBuffer(ID3D11DeviceContext* pContext)
{
	if(pContext)
	{
		D3D11_MAPPED_SUBRESOURCE lightSubresource;

		pContext->Map(m_pAmbientLightConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &lightSubresource);
			// NOTE: Copy from first data member in the class to avoid mem copying the vtable pointer
			memcpy(lightSubresource.pData, &m_vColor, sizeof(AmbientLight));
		pContext->Unmap(m_pAmbientLightConstBuffer, 0);
	}
}

void AmbientLight::ReleaseConstantBufferAndClassLinkage()
{
	SAFE_RELEASE(m_pAmbientLightConstBuffer);
	SAFE_RELEASE(m_pAmbientLightClassInstance);
}