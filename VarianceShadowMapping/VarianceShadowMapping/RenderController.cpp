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
	InitShaderTextures();
	InitBlendStates();
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

	if(FAILED(m_pCore->GetDevice()->CreateSamplerState(&samplerDesc, &m_pTextureSamplers[WRAP_SAMPLER_STATE].p)))
		MessageBox(NULL, L"Failed to Create Wrap Texture Sampler", L"", MB_OK | MB_ICONERROR);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	if(FAILED(m_pCore->GetDevice()->CreateSamplerState(&samplerDesc, &m_pTextureSamplers[CLAMP_SAMPLER_STATE].p)))
		MessageBox(NULL, L"Failed to Create Wrap Texture Sampler", L"", MB_OK | MB_ICONERROR);


	ID3D11SamplerState* pSamplers[NUM_SAMPLER_STATES];

	for(unsigned int i = 0; i < NUM_SAMPLER_STATES; ++i)
		pSamplers[i] = m_pTextureSamplers[i].p;

	m_pCore->GetContext()->PSSetSamplers(0, NUM_SAMPLER_STATES, pSamplers);
}

void RenderController::InitShaderTextures()
{

}

void RenderController::InitBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	m_pCore->GetDevice()->CreateBlendState(&blendDesc, &m_BlendStates[ADDITIVE_BLEND_MODE].p);

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	m_pCore->GetDevice()->CreateBlendState(&blendDesc, &m_BlendStates[NO_BLEND_MODE].p);
}

void RenderController::SetBlendState(BlendModeEnum eBlendMode)
{
	if(eBlendMode >= 0 && eBlendMode < NUM_BLEND_MODES)
		m_pCore->GetContext()->OMSetBlendState(m_BlendStates[eBlendMode].p, NULL, UINT_MAX);
}