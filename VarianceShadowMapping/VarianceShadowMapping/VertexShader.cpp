#include "VertexShader.h"

VertexShader::VertexShader() : m_pShader(nullptr), m_pContext(nullptr)
{

}
	
VertexShader::~VertexShader()
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}

	m_pContext = nullptr;
}

HRESULT VertexShader::CreateVertexShader(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	m_pContext = pContext;

	if(pDevice)
	{
		if(m_pShader)
		{
			m_pShader->Release();
			m_pShader = nullptr;
		}

		retVal = pDevice->CreateVertexShader(pShaderBytecode, unBytecodeLength, NULL, &m_pShader);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Vertex Shader" , L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
	}

	return retVal;
}