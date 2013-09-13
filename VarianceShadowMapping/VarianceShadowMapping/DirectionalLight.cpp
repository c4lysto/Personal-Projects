#include "DirectionalLight.h"

ID3D11Buffer* DirectionalLight::m_pDirLightConstBuffer = nullptr;
ID3D11ClassInstance* DirectionalLight::m_pDirLightClassInstance = nullptr;

// NOTE: sizeof(void*) represents the vftable pointer
const UINT g_DirLightActualByteWidth = (sizeof(DirectionalLight) - sizeof(void*));
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