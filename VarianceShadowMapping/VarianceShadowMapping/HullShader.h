#ifndef HULLSHADER_H
#define HULLSHADER_H

#include "Shader.h"

class HullShader : public IShader
{
private:
	ID3D11HullShader* m_pShader;

public:
	HullShader(void);
	~HullShader(void);

	virtual HRESULT CreateHullShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetHullShader()
	{
		if(m_pContext)
			m_pContext->HSSetShader(m_pShader, NULL, 0);
	}

	static void ResetHullShader()
	{
		if(m_pContext)
			m_pContext->HSSetShader(NULL, NULL, 0);
	}
};

#endif