#ifndef	RENDERTARGETCUBE_H
#define RENDERTARGETCUBE_H

#include "RenderTarget.h"

class RenderTargetCube : public RenderTarget
{
private:
	RenderTarget m_rtFaceRenderTargets[6];

public:
	RenderTargetCube(void);
	~RenderTargetCube(void);

	virtual void CleanUp();

	const RenderTarget& GetFaceRenderTarget(D3D11_TEXTURECUBE_FACE eFace) const {return m_rtFaceRenderTargets[eFace];}

	virtual void Create(DirectXCore* pCore, unsigned int unWidth = 0, unsigned int unHeight = 0,
				DXGI_FORMAT eFormat = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_RENDER_TARGET_VIEW_DESC* targetviewDesc = nullptr);
};

#endif