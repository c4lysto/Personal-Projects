#include "RenderTargetCube.h"
#include "DirectXCore.h"

RenderTargetCube::RenderTargetCube(void)
{
}

RenderTargetCube::~RenderTargetCube(void)
{
	CleanUp();
}

void RenderTargetCube::CleanUp()
{
	RenderTarget::CleanUp();

	for(unsigned int i = 0; i < 6; ++i)
		m_rtFaceRenderTargets[i].CleanUp();
		//SAFE_RELEASE(m_pFaceRenderTargets[i]);

}

void RenderTargetCube::Create(DirectXCore* pCore, unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc)
{
	CleanUp();

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = (!unWidth) ? pCore->GetScreenWidth() : unWidth;
	texDesc.Height = (!unHeight) ? pCore->GetScreenHeight() : unHeight;
	texDesc.Format = eFormat;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;


	if(FAILED(pCore->GetDevice()->CreateTexture2D(&texDesc, NULL, &m_pTexture)))
	{
		MessageBox(NULL, L"Failed To Create Render Target Cube Texture", L"", MB_OK | MB_ICONERROR);
		return;
	}

	if(!targetviewDesc)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = eFormat;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.ArraySize = 6;

		if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, &rtvDesc, &m_pTarget)))
		{
			MessageBox(NULL, L"Failed to Create Render Target Cube", L"", MB_OK | MB_ICONERROR);
			return;
		}

		rtvDesc.Texture2DArray.ArraySize = 1;
		for(unsigned int i = 0; i < 6; ++i)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			m_rtFaceRenderTargets[i].Create(pCore, m_pTexture, &rtvDesc);
			/*if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, &rtvDesc, &m_pFaceRenderTargets[i])))
			{
				MessageBox(NULL, L"Failed to Create Render Target View For Cube Face", L"", MB_OK | MB_ICONERROR);
				return;
			}*/
		}
	}
	else
	{
		if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, targetviewDesc, &m_pTarget)))
		{
			MessageBox(NULL, L"Failed to Create Render Target Cube", L"", MB_OK | MB_ICONERROR);
			return;
		}

		targetviewDesc->Texture2DArray.ArraySize = 1;
		for(unsigned int i = 0; i < 6; ++i)
		{
			targetviewDesc->Texture2DArray.FirstArraySlice = i;
			m_rtFaceRenderTargets[i].Create(pCore, m_pTexture, targetviewDesc);

			/*if(FAILED(pCore->GetDevice()->CreateRenderTargetView(m_pTexture, targetviewDesc, &m_rtFaceRenderTargets[i])))
			{
				MessageBox(NULL, L"Failed to Create Render Target View For Cube Face", L"", MB_OK | MB_ICONERROR);
				return;
			}*/
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = eFormat;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2DArray.MipLevels = -1;

	if(FAILED(pCore->GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pSRV)))
	{
		MessageBox(NULL, L"Failed to Create Render Target Cube", L"", MB_OK | MB_ICONERROR);
		return;
	}
}