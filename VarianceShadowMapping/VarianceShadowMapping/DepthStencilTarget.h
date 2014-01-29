#ifndef DEPTHSTENCILTARGET_H
#define DEPTHSTENCILTARGET_H

#include <d3d11.h>

class DepthStencilTarget
{
protected:
	ID3D11Texture2D* m_pTexture;
	ID3D11DepthStencilView* m_pDepthStencil;

public:
	DepthStencilTarget(void);
	~DepthStencilTarget(void);

	void Create(ID3D11Device* pDevice, ID3D11Texture2D* pTexture, D3D11_DEPTH_STENCIL_VIEW_DESC* pDepthStencilViewDesc = nullptr);
	virtual void Create(ID3D11Device* pDevice, unsigned int unWidth, unsigned int unHeight, 
				DXGI_FORMAT eFormat = DXGI_FORMAT_R24G8_TYPELESS, D3D11_DEPTH_STENCIL_VIEW_DESC* pDepthStencilViewDesc = nullptr);

	inline ID3D11DepthStencilView* GetDepthStencilView() const {return m_pDepthStencil;}
};

#endif