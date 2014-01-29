#include "HullShader.h"

HullShader::HullShader(void) : m_pShader(nullptr)
{
}


HullShader::~HullShader(void)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}
}


HRESULT HullShader::CreateHullShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	if(pDevice)
	{
		if(m_pShader)
		{
			m_pShader->Release();
			m_pShader = nullptr;
		}

		retVal = pDevice->CreateHullShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Hull Shader", L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
	}

	return retVal;
}