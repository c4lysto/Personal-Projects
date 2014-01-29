#ifndef	PIXELSHADER_H
#define PIXELSHADER_H

#include "Shader.h"

class PixelShader : public IShader
{
private:
	

protected:
	ID3D11PixelShader* m_pShader;

public:
	PixelShader();
	~PixelShader();

	virtual HRESULT CreatePixelShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetPixelShader()
	{
		if(m_pContext)
			m_pContext->PSSetShader(m_pShader, NULL, 0);
	}

	static void ResetPixelShader()
	{
		if(m_pContext)
			m_pContext->PSSetShader(NULL, NULL, 0);
	}
};

#endif // PIXELSHADER_H