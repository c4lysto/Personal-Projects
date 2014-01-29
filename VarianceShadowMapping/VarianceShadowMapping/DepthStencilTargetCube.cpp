#include "DepthStencilTargetCube.h"


DepthStencilTargetCube::DepthStencilTargetCube(void)
{
}


DepthStencilTargetCube::~DepthStencilTargetCube(void)
{
}

void DepthStencilTargetCube::Create(ID3D11Device* pDevice, unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, D3D11_DEPTH_STENCIL_VIEW_DESC* pDepthStencilViewDesc)
{
	if(m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = nullptr;
	}

	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	// Create a Texture 2D that can be used a pixel shader if desired
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = unWidth;
	texDesc.Height = unHeight;
	texDesc.Format = eFormat;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if(FAILED(pDevice->CreateTexture2D(&texDesc, NULL, &m_pTexture)))
		MessageBox(NULL, L"Failed To Create Depth Stencil Cube Texture", L"", MB_OK | MB_ICONERROR);

	if(!pDepthStencilViewDesc)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthStencilDesc.Texture2DArray.ArraySize = 6;

		switch(eFormat)
		{
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
			{
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			}
			break;

		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_R32_FLOAT:
			{
				depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			}
			break;
		}

		if(FAILED(pDevice->CreateDepthStencilView(m_pTexture, &depthStencilDesc, &m_pDepthStencil)))
			MessageBox(NULL, L"Failed to Create Depth Stencil Cube View", L"", MB_OK | MB_ICONERROR);
	}
	else
	{
		if(FAILED(pDevice->CreateDepthStencilView(m_pTexture, pDepthStencilViewDesc, &m_pDepthStencil)))
				MessageBox(NULL, L"Failed to Create Depth Stencil Cube View", L"", MB_OK | MB_ICONERROR);
	}
}