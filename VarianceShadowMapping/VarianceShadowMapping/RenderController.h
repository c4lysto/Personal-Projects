#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <atlbase.h>

class DirectXCore;

enum TextureSamplerStateEnum {WRAP_SAMPLER_STATE, CLAMP_SAMPLER_STATE, NUM_SAMPLER_STATES};

class RenderController
{
private:
	DirectXCore* m_pCore;

	CComPtr<ID3D11SamplerState> m_pTextureSamplers[NUM_SAMPLER_STATES];

	void InitShaderSamplers();
	void InitShaderTextures();

public:
	RenderController(void);
	~RenderController(void);

	void Initialize(DirectXCore* pCore);
};

#endif