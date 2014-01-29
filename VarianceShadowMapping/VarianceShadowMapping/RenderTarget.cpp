#include "RenderTarget.h"
#include "DirectXCore.h"

RenderTarget::RenderTarget() : m_pTarget(nullptr), m_pTexture(nullptr), m_pSRV(nullptr)
{

}
	
RenderTarget::~RenderTarget()
{
	CleanUp();
}

void RenderTarget::CleanUp()
{
	SAFE_RELEASE(m_pTarget);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSRV);
}

void RenderTarget::Create(DirectXCore* pCore, ID3D11Texture2D* pTexture, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc)
{
	if(pTexture)
	{
		CleanUp();

		m_pTexture = pTexture;
		if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, targetviewDesc, &m_pTarget)))
			MessageBox(NULL, L"Failed to Create Render Target", L"", MB_OK | MB_ICONERROR);

		D3D11_TEXTURE2D_DESC texDesc;
		m_pTexture->GetDesc(&texDesc);

		if((texDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

			if(FAILED(pCore->GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pSRV)))
			{
				MessageBox(NULL, L"Failed to Create Shader Resource View", L"", MB_OK | MB_ICONERROR);
				return;
			}
		}
	}
}

void RenderTarget::Create(DirectXCore* pCore, unsigned int unWidth, unsigned int unHeight,
							DXGI_FORMAT eFormat, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc)
{
	CleanUp();

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = (!unWidth) ? pCore->GetScreenWidth() : unWidth;
	texDesc.Height = (!unHeight) ? pCore->GetScreenHeight() : unHeight;
	texDesc.Format = eFormat;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;


	if(FAILED(pCore->GetDevice()->CreateTexture2D(&texDesc, NULL, &m_pTexture)))
	{
		MessageBox(NULL, L"Failed To Create Render Target Texture", L"", MB_OK | MB_ICONERROR);
		return;
	}

	if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, targetviewDesc, &m_pTarget)))
	{
		MessageBox(NULL, L"Failed to Create Render Target", L"", MB_OK | MB_ICONERROR);
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = eFormat;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	if(FAILED(pCore->GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pSRV)))
	{
		MessageBox(NULL, L"Failed to Create Render Target", L"", MB_OK | MB_ICONERROR);
		return;
	}
}