#ifndef	DEPTHSTENCILTARGETCUBE_H
#define DEPTHSTENCILTARGETCUBE_H

#include "DepthStencilTarget.h"

class DepthStencilTargetCube : public DepthStencilTarget
{
public:
	DepthStencilTargetCube(void);
	~DepthStencilTargetCube(void);

	virtual void Create(ID3D11Device* pDevice, unsigned int unWidth, unsigned int unHeight, 
				DXGI_FORMAT eFormat = DXGI_FORMAT_R24G8_TYPELESS, D3D11_DEPTH_STENCIL_VIEW_DESC* pDepthStencilViewDesc = nullptr);
};

#endif //DEPTHSTENCILTARGETCUBE_H