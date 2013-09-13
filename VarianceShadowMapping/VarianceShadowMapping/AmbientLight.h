#ifndef AMBIENTLIGHT_H
#define	AMBIENTLIGHT_H

#include "ILight.h"

#define AMBIENTLIGHT_CONST_BUFF_REGISTER 6

class AmbientLight
{
private:
	Vec3f m_vColor;

	static ID3D11Buffer* m_pAmbientLightConstBuffer;
	static ID3D11ClassInstance* m_pAmbientLightClassInstance;

public:
	AmbientLight() : m_vColor(1.0f, 1.0f, 1.0f) {}
	~AmbientLight() {}

	void SetLightColor(const Vec3f& vColor) {m_vColor = vColor;}
	void SetLightColor(float fX, float fY, float fZ)
	{m_vColor.x = fX; m_vColor.y = fY; m_vColor.z = fZ;}

	void SetConstantBufferPS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->PSSetConstantBuffers(AMBIENTLIGHT_CONST_BUFF_REGISTER, 1, &m_pAmbientLightConstBuffer);}
	void SetConstantBufferVS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->VSSetConstantBuffers(AMBIENTLIGHT_CONST_BUFF_REGISTER, 1, &m_pAmbientLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif