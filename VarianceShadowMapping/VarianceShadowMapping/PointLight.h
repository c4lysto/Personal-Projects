#ifndef POINTLIGHT_H
#define PIONTLIGHT_H

#include "ILight.h"

#define POINTLIGHT_CONST_BUFF_REGISTER 3

class PointLight : public ILight
{
private:
	Vec3f m_vPosition;
	float m_fRadius;
	Matrix4f m_ViewMatrices[6];
	Matrix4f m_ProjectionMatrix;

	static ID3D11Buffer* m_pPtLightConstBuffer;
	static ID3D11ClassInstance* m_pPtLightClassInstance;

public:
	PointLight(void);
	~PointLight(void);

	virtual Matrix4f GetViewMatrix(unsigned int unCubeFace = 0) const 
	{return (unCubeFace < 6) ? m_ViewMatrices[unCubeFace] : Matrix4f();}
	virtual Matrix4f GetViewProjectionMatrix(unsigned int unCubeFace = 0) const 
	{return (unCubeFace < 6) ? (m_ViewMatrices[unCubeFace] * m_ProjectionMatrix) : Matrix4f();}

	virtual void InitializeViewProjMatrix(const Camera* pCam);
	virtual void SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam);

	void UpdateViewProjMatrix(const Camera* pCam);

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
	void SetConstantBufferGS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->GSSetConstantBuffers(POINTLIGHT_CONST_BUFF_REGISTER, 1, &m_pPtLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif	// POINTLIGHT_H