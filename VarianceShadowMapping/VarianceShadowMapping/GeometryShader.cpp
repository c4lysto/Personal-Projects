#include "GeometryShader.h"


GeometryShader::GeometryShader(void) : m_pShader(nullptr)
{
}


GeometryShader::~GeometryShader(void)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}

}


HRESULT GeometryShader::CreateGeometryShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	if(pDevice)
	{
		if(m_pShader)
		{
			m_pShader->Release();
			m_pShader = nullptr;
		}

		retVal = pDevice->CreateGeometryShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Geometry Shader" , L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
	}

	return retVal;
}