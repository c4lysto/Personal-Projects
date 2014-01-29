#include "UnorderedAccessView.h"


UnorderedAccessView::UnorderedAccessView(void) : m_pUAV(NULL)
{
}


UnorderedAccessView::~UnorderedAccessView(void)
{
	if(m_pUAV)
	{
		m_pUAV->Release();
		m_pUAV = NULL;
	}
}

void UnorderedAccessView::CreateUAV(ID3D11Device* pDevice, ID3D11Resource* pResource, D3D11_UNORDERED_ACCESS_VIEW_DESC* pUAVDesc)
{
	if(m_pUAV)
	{
		m_pUAV->Release();
		m_pUAV = NULL;
	}

	if(FAILED(pDevice->CreateUnorderedAccessView(pResource, pUAVDesc, &m_pUAV)))
		MessageBox(NULL, L"Failed To Create Unordered Accss View", L"", MB_OK | MB_ICONERROR);
}