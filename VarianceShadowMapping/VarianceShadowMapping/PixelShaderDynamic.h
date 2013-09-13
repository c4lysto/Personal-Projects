#ifndef PIXELSHADERDYNAMIC_H
#define	PIXELSHADERDYNAMIC_H

#include "PixelShader.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

struct ID3D11ShaderReflection;

class PixelShaderDynamic : public PixelShader
{
private:
	ID3D11ClassLinkage* m_pClassLinkage;
	ID3D11ClassInstance** m_pDynamicLinkageArray;
	vector<string> m_vInterfaceNames;
	ID3D11ShaderReflection* m_pReflector;

	void InitClassLinkage(ID3D11Device* pDevice);
	inline HRESULT InitPixelShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);
	HRESULT InitShaderReflection(const void* pShaderBytecode, SIZE_T unBytecodeLength);

protected:


public:
	PixelShaderDynamic(void);
	~PixelShaderDynamic(void);

	virtual HRESULT CreatePixelShader(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength);

	UINT AddInterfaceByName(LPCSTR szAbstractObjectName);

	void SetClassInstance(LPCSTR szShaderObjectName, LPCSTR szAbstractObjectName, UINT unInstanceIndex = 0);
	void SetClassInstance(LPCSTR szShaderObjectName, unsigned int unIndex, UINT unInstanceIndex = 0);

	virtual void SetPixelShader()
	{
		if(m_pContext)
			m_pContext->PSSetShader(m_pShader, m_pDynamicLinkageArray, m_vInterfaceNames.size());
	}
};

inline HRESULT PixelShaderDynamic::InitPixelShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	if(m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}

	HRESULT retVal = pDevice->CreatePixelShader(pShaderBytecode, unBytecodeLength, m_pClassLinkage, &m_pShader);

	if(FAILED(retVal))
	{
		MessageBox(NULL, L"Failed To Create Pixel Shader (Dynamic Linked PS)" , L"", MB_OK | MB_ICONERROR);
		return retVal;
	}

	return retVal;
}

#endif // PIXELSHADERDYNAMIC_H