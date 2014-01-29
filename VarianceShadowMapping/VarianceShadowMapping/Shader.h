#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>

class IShader
{
protected:
	static ID3D11DeviceContext* m_pContext;

public:
	IShader() {}
	virtual ~IShader() = 0 {}

	static void SetDeviceContext(ID3D11DeviceContext* pContext) { m_pContext = pContext; }
};

__declspec(selectany) ID3D11DeviceContext* IShader::m_pContext = nullptr;

#endif