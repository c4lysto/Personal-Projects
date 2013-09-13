#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <atlbase.h>

class DirectXCore;

enum TextureSamplerStateEnum {WRAP_SAMPLER_STATE, CLAMP_SAMPLER_STATE, NUM_SAMPLER_STATES};
enum BlendModeEnum {ADDITIVE_BLEND_MODE, NO_BLEND_MODE, NUM_BLEND_MODES};

class RenderController
{
private:
	DirectXCore* m_pCore;

	CComPtr<ID3D11SamplerState> m_pTextureSamplers[NUM_SAMPLER_STATES];
	CComPtr<ID3D11BlendState> m_BlendStates[NUM_BLEND_MODES];

	void InitShaderSamplers();
	void InitShaderTextures();

	void InitBlendStates();

public:
	RenderController(void);
	~RenderController(void);

	void Initialize(DirectXCore* pCore);

	void SetBlendState(BlendModeEnum eBlendMode);
};

#endif