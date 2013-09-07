#ifndef POINTLIGHT_H
#define PIONTLIGHT_H

#include "ILight.h"

#define POINTLIGHT_CONST_BUFF_REGISTER 3

class PointLight : public ILight
{
private:
	Vec3f m_vPosition;
	float m_fRadius;

	static ID3D11Buffer* m_pPtLightConstBuffer;
	static ID3D11ClassInstance* m_pPtLightClassInstance;

public:
	PointLight(void);
	~PointLight(void);

	inline void SetPosition(const Vec3f& vPos) {m_vPosition = vPos;}
	inline void SetPosition(float fX, float fY, float fZ) 
	{m_vPosition.x = fX; m_vPosition.y = fY; m_vPosition.z = fZ;}

	inline void SetRadius(float fRadius) {m_fRadius = fRadius;}

	inline Vec3f GetPosition() const {return m_vPosition;}
	inline float GetRadius() const {return m_fRadius;}

	void SetConstantBufferPS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->PSSetConstantBuffers(POINTLIGHT_CONST_BUFF_REGISTER, 1, &m_pPtLightConstBuffer);}
	void SetConstantBufferVS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->VSSetConstantBuffers(POINTLIGHT_CONST_BUFF_REGISTER, 1, &m_pPtLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif	// POINTLIGHT_H