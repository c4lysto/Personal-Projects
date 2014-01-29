#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <atlbase.h>

#include <RMMath.h>
using namespace RMMath;

class DirectXCore;

enum TextureSamplerStateEnum {WRAP_SAMPLER_STATE, CLAMP_SAMPLER_STATE, CLAMP_POINT_SAMPLER_STATE, NUM_SAMPLER_STATES};
enum BlendModeEnum {ADDITIVE_BLEND_MODE, ALPHA_BLEND_MODE, NO_BLEND_MODE, NUM_BLEND_MODES};
enum DepthStencilStateEnum {DEPTH_STENCIL_DEFAULT, DEPTH_LESSEQUAL_NO_STENCIL, DEPTH_NONE_STENCIL_NONE, 
					DEPTH_READ_NO_WRITE, NUM_DEPTH_STENCIL_STATES};

class RenderController
{
private:
	DirectXCore* m_pCore;

	CComPtr<ID3D11SamplerState> m_pTextureSamplers[NUM_SAMPLER_STATES];
	CComPtr<ID3D11BlendState> m_BlendStates[NUM_BLEND_MODES];
	CComPtr<ID3D11DepthStencilState> m_DepthStencilStates[NUM_DEPTH_STENCIL_STATES];

	void InitShaderSamplers();
	void InitBlendStates();
	void InitDepthStencilStates();

public:
	RenderController(void);
	~RenderController(void);

	void Initialize(DirectXCore* pCore);

	void SetBlendState(BlendModeEnum eBlendMode, const Vec4f vBlendFactor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), UINT unSampleMask = UINT_MAX);
	void SetDepthStencilState(DepthStencilStateEnum eDepthStencilState, UINT unStencilRef = 0);
};

#endif