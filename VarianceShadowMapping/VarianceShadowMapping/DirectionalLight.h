#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "ILight.h"

#define DIRLIGHT_CONST_BUFF_REGISTER 5

class DirectionalLight : public ILight
{
private:
	Vec3f m_vDirection;

	static ID3D11Buffer* m_pDirLightConstBuffer;
	static ID3D11ClassInstance* m_pDirLightClassInstance;

public:
	DirectionalLight() {}
	~DirectionalLight() {}

	inline void SetDirection(const Vec3f& vDirection) {m_vDirection = Normalize(vDirection);}
	inline void SetDirection(float fX, float fY, float fZ)
	{m_vDirection.x = fX; m_vDirection.y = fY; m_vDirection.z = fZ; m_vDirection.normalize();}

	inline Vec3f GetDirection() const {return m_vDirection;}

	void SetConstantBufferPS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->PSSetConstantBuffers(DIRLIGHT_CONST_BUFF_REGISTER, 1, &m_pDirLightConstBuffer);}
	void SetConstantBufferVS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->VSSetConstantBuffers(DIRLIGHT_CONST_BUFF_REGISTER, 1, &m_pDirLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif	//DIRECTIONALLIGHT_H