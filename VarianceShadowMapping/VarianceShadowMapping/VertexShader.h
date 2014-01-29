#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Shader.h"

class VertexShader : public IShader
{
private:
	ID3D11VertexShader* m_pShader;

public:
	VertexShader();
	~VertexShader();

	virtual HRESULT CreateVertexShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetVertexShader()
	{
		if(m_pContext)
			m_pContext->VSSetShader(m_pShader, NULL, 0);
	}

	static void ResetVertexShader()
	{
		if(m_pContext)
			m_pContext->VSSetShader(NULL, NULL, 0);
	}
};

#endif // VERTEXSHADER_H