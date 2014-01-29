#include "ComputeShader.h"

ComputeShader::ComputeShader(void) : m_pShader(NULL), m_unDimX(1), m_unDimY(1), m_unDimZ(1)
{

}


ComputeShader::~ComputeShader(void)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}
}

HRESULT ComputeShader::CreateComputeShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	if(pDevice)
	{
		if(m_pShader)
		{
			m_pShader->Release();
			m_pShader = nullptr;
		}

		retVal = pDevice->CreateComputeShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Compute Shader" , L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
	}

	return retVal;
}

void ComputeShader::Dispatch()
{
	if(m_pContext)
		m_pContext->Dispatch(m_unDimX, m_unDimY, m_unDimZ);
}