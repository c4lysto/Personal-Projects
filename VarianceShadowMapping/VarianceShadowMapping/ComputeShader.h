#ifndef COMPUTESHADER_H
#define	COMPUTESHADER_H

#include "Shader.h"

class ComputeShader : public IShader
{
private:
	unsigned int m_unDimX, m_unDimY, m_unDimZ;
	
	ID3D11ComputeShader* m_pShader;

public:
	ComputeShader(void);
	~ComputeShader(void);

	HRESULT CreateComputeShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	void SetDispatchDimensions(unsigned int unDimX = 1, unsigned int unDimY = 1, unsigned int unDimZ = 1)
	{m_unDimX = unDimX; m_unDimY = unDimY; m_unDimZ = unDimZ;}

	void Dispatch();

	void SetComputeShader()
	{
		if(m_pContext)
			m_pContext->CSSetShader(m_pShader, NULL, 0);
	}

	static void ResetComputeShader()
	{
		if(m_pContext)
			m_pContext->CSSetShader(NULL, NULL, 0);
	}
};

#endif //COMPUTESHADER_H