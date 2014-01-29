#include "DomainShader.h"

DomainShader::DomainShader(void) : m_pShader(nullptr)
{
}


DomainShader::~DomainShader(void)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}
}


HRESULT DomainShader::CreateDomainShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	if(pDevice)
	{
		if(m_pShader)
		{
			m_pShader->Release();
			m_pShader = nullptr;
		}

		retVal = pDevice->CreateDomainShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Domain Shader", L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
	}

	return retVal;
}