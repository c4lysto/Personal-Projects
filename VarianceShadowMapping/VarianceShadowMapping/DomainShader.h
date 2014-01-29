#ifndef DOMAINSHADER_H
#define DOMAINSHADER_H

#include "Shader.h"

class DomainShader : public IShader
{
private:
	ID3D11DomainShader* m_pShader;

public:
	DomainShader(void);
	~DomainShader(void);

	virtual HRESULT CreateDomainShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetDomainShader()
	{
		if(m_pContext)
			m_pContext->DSSetShader(m_pShader, NULL, 0);
	}

	static void ResetDomainShader()
	{
		if(m_pContext)
			m_pContext->DSSetShader(NULL, NULL, 0);
	}
};

#endif