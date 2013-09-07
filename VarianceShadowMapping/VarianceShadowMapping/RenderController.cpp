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