#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <D3D11.h>
#pragma comment(lib, "d3d11.lib")

#include <atlbase.h>

class DirectXCore;

class RenderTarget
{
private:
	CComPtr<ID3D11RenderTargetView> m_pTarget;
	CComPtr<ID3D11Texture2D> m_pTexture;

public:
	RenderTarget();
	~RenderTarget();

	void Create(DirectXCore* pCore, ID3D11Texture2D* pTexture = nullptr, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc = nullptr);
	void Create(DirectXCore* pCore, DXGI_FORMAT eFormat = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc = nullptr);

	ID3D11RenderTargetView* GetRenderTargetView() const {return m_pTarget;}

	inline operator ID3D11RenderTargetView* () {return m_pTarget;}
	inline operator const ID3D11RenderTargetView* () const {return m_pTarget;}
};

#endif