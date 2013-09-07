#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <d3d11.h>

class VertexShader
{
private:
	ID3D11VertexShader* m_pShader;
	ID3D11DeviceContext* m_pContext;

protected:

public:
	VertexShader();
	~VertexShader();

	virtual HRESULT CreateVertexShader(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetVertexShader()
	{
		if(m_pContext)
			m_pContext->VSSetShader(m_pShader, NULL, 0);
	}
};

#endif // VERTEXSHADER_H