#include "RenderController.h"
#include "DirectXCore.h"

#include <DirectXMath.h>
using namespace DirectX;

RenderController::RenderController(void)
{
}


RenderController::~RenderController(void)
{
}

void RenderController::Initialize(DirectXCore* pCore)
{
	m_pCore = pCore;

	InitShaderSamplers();
	InitBlendStates();
	InitDepthStencilStates();
}

void RenderController::InitShaderSamplers()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if(FAILED(m_pCore->GetDevice()->CreateSamplerState(&samplerDesc, &m_pTextureSamplers[WRAP_SAMPLER_STATE].p)))
		MessageBox(NULL, L"Failed to Create Wrap Texture Sampler", L"", MB_OK | MB_ICONERROR);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	if(FAILED(m_pCore->GetDevice()->CreateSamplerState(&samplerDesc, &m_pTextureSamplers[CLAMP_SAMPLER_STATE].p)))
		MessageBox(NULL, L"Failed to Create Clamp Texture Sampler", L"", MB_OK | MB_ICONERROR);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = 0;

	if(FAILED(m_pCore->GetDevice()->CreateSamplerState(&samplerDesc, &m_pTextureSamplers[CLAMP_POINT_SAMPLER_STATE].p)))
		MessageBox(NULL, L"Failed to Create Clamp Point Texture Sampler", L"", MB_OK | MB_ICONERROR);


	ID3D11SamplerState* pSamplers[NUM_SAMPLER_STATES];

	for(unsigned int i = 0; i < NUM_SAMPLER_STATES; ++i)
		pSamplers[i] = m_pTextureSamplers[i].p;

	m_pCore->GetContext()->PSSetSamplers(0, NUM_SAMPLER_STATES, pSamplers);
	m_pCore->GetContext()->DSSetSamplers(0, NUM_SAMPLER_STATES, pSamplers);
}

void RenderController::InitBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_pCore->GetDevice()->CreateBlendState(&blendDesc, &m_BlendStates[ADDITIVE_BLEND_MODE].p);

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	m_pCore->GetDevice()->CreateBlendState(&blendDesc, &m_BlendStates[NO_BLEND_MODE].p);

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_pCore->GetDevice()->CreateBlendState(&blendDesc, &m_BlendStates[ALPHA_BLEND_MODE].p);
}

void RenderController::InitDepthStencilStates()
{
	m_DepthStencilStates[DEPTH_STENCIL_DEFAULT] = NULL;

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;

	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	if (FAILED(m_pCore->GetDevice()->CreateDepthStencilState(&dsDesc, &m_DepthStencilStates[DEPTH_READ_NO_WRITE].p)))
		MessageBox(NULL, L"Failed to Create DEPTH_NONE_STENCIL_NONE state", L"", MB_OK | MB_ICONERROR);

	dsDesc.StencilEnable = FALSE;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	if(FAILED(m_pCore->GetDevice()->CreateDepthStencilState(&dsDesc, &m_DepthStencilStates[DEPTH_LESSEQUAL_NO_STENCIL].p)))
		MessageBox(NULL, L"Failed to Create DEPTH_LESSEQUAL_NO_STENCIL state", L"", MB_OK | MB_ICONERROR);


	dsDesc.DepthEnable = FALSE;
	if(FAILED(m_pCore->GetDevice()->CreateDepthStencilState(&dsDesc, &m_DepthStencilStates[DEPTH_NONE_STENCIL_NONE].p)))
		MessageBox(NULL, L"Failed to Create DEPTH_NONE_STENCIL_NONE state", L"", MB_OK | MB_ICONERROR);
}

void RenderController::SetBlendState(BlendModeEnum eBlendMode, const Vec4f vBlendFactor, UINT unSampleMask)
{
	if(eBlendMode >= 0 && eBlendMode < NUM_BLEND_MODES)
		m_pCore->GetContext()->OMSetBlendState(m_BlendStates[eBlendMode].p, vBlendFactor.vector, unSampleMask);
}

void RenderController::SetDepthStencilState(DepthStencilStateEnum eDepthStencilState, UINT unStencilRef)
{
	if(eDepthStencilState >= 0 && eDepthStencilState < NUM_DEPTH_STENCIL_STATES)
		m_pCore->GetContext()->OMSetDepthStencilState(m_DepthStencilStates[eDepthStencilState].p, unStencilRef);
}