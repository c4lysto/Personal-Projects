#include "RenderTarget.h"
#include "DirectXCore.h"

RenderTarget::RenderTarget()
{

}
	
RenderTarget::~RenderTarget()
{
}

void RenderTarget::Create(DirectXCore* pCore, ID3D11Texture2D* pTexture, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc)
{
	if(pTexture)
	{
		m_pTexture.p = pTexture;
		if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture.p, targetviewDesc, &m_pTarget.p)))
			MessageBox(NULL, L"Failed to Create Render Target", L"", MB_OK | MB_ICONERROR);
	}
}

void RenderTarget::Create(DirectXCore* pCore, DXGI_FORMAT eFormat, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = pCore->GetScreenWidth();
	texDesc.Height = pCore->GetScreenHeight();
	texDesc.Format = eFormat;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;


	if(FAILED(pCore->GetDevice()->CreateTexture2D(&texDesc, NULL, &m_pTexture.p)))
		MessageBox(NULL, L"Failed To Create Render Target Texture", L"", MB_OK | MB_ICONERROR);

	if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture.p, targetviewDesc, &m_pTarget.p)))
		MessageBox(NULL, L"Failed to Create Render Target", L"", MB_OK | MB_ICONERROR);
}