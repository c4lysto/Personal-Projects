#include "PixelShader.h"

PixelShader::PixelShader() : m_pShader(nullptr), m_pContext(nullptr)
{

}

PixelShader::~PixelShader()
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}

	m_pContext = nullptr;
}

HRESULT PixelShader::CreatePixelShader(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}

	m_pContext = pContext;

	HRESULT retVal = S_OK;

	// Will Return S_OK if device is NULL just in case we want a NULL shader
	// so that we can set the stages of the pipeline as wanted
	if(pDevice)
	{
		retVal = pDevice->CreatePixelShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
			MessageBox(NULL, L"Failed To Create Pixel Shader" , L"", MB_OK | MB_ICONERROR);
	}

	return retVal;
}