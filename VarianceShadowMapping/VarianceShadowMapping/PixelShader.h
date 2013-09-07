#ifndef	PIXELSHADER_H
#define PIXELSHADER_H

#include <d3d11.h>

class PixelShader
{
private:
	

protected:
	ID3D11PixelShader* m_pShader;
	ID3D11DeviceContext* m_pContext;

public:
	PixelShader();
	~PixelShader();

	virtual HRESULT CreatePixelShader(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetPixelShader()
	{
		if(m_pContext)
			m_pContext->PSSetShader(m_pShader, NULL, 0);
	}
};

#endif // PIXELSHADER_H