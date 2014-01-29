#ifndef	GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Shader.h"

class GeometryShader : public IShader
{
private:
	ID3D11GeometryShader* m_pShader;

public:
	GeometryShader(void);
	~GeometryShader(void);

	virtual HRESULT CreateGeometryShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	virtual void SetGeometryShader()
	{
		if(m_pContext)
			m_pContext->GSSetShader(m_pShader, NULL, 0);
	}

	static void ResetGeometryShader()
	{
		if(m_pContext)
			m_pContext->GSSetShader(NULL, NULL, 0);
	}
};

#endif	//GEOMETRYSHADER_H
