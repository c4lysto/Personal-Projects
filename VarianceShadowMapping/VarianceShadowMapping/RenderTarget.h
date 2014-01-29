#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <D3D11.h>
#pragma comment(lib, "d3d11.lib")

#include <atlbase.h>

class DirectXCore;

class RenderTarget
{
protected:
	ID3D11RenderTargetView* m_pTarget;
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pSRV;

	

public:
	RenderTarget();
	~RenderTarget();

	virtual void CleanUp();

	void Create(DirectXCore* pCore, ID3D11Texture2D* pTexture, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc = nullptr);
	virtual void Create(DirectXCore* pCore, unsigned int unWidth = 0, unsigned int unHeight = 0,
				DXGI_FORMAT eFormat = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc = nullptr);

	ID3D11RenderTargetView* GetRenderTargetView() const {return m_pTarget;}
	ID3D11Texture2D* GetRenderTexture() const {return m_pTexture;}
	ID3D11ShaderResourceView* GetShaderResourceView() const {return m_pSRV;}


	inline operator ID3D11RenderTargetView* () {return m_pTarget;}
	inline operator const ID3D11RenderTargetView* () const {return m_pTarget;}
};

#endif