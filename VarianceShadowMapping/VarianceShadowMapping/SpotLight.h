#ifndef	SPOTLIGHT_H
#define SPOTLIGHT_H

#include "ILight.h"

#define SPOTLIGHT_CONST_BUFF_REGISTER 4

class SpotLight : public ILight
{
private:
	Vec3f m_vPosition;
	float m_fRadius;
	Matrix4f m_mViewProjectionTexMatrix, m_mViewMatrix;
	Vec3f m_vDirection;
	float m_fInnerConeRatio;
	float m_fOuterConeRatio;
	Matrix4f m_mViewProjMatrix;

	static ID3D11Buffer* m_pSpotLightConstBuffer;
	static ID3D11ClassInstance* m_pSpotLightClassInstance;

public:
	SpotLight() : m_fInnerConeRatio(0.0f), m_fOuterConeRatio(0.0f) {}
	~SpotLight() {}

	virtual Matrix4f GetViewMatrix(unsigned int unCubeFace = 0) const {return m_mViewMatrix;}
	virtual Matrix4f GetViewProjectionMatrix(unsigned int unCubeFace = 0) const {return m_mViewProjMatrix;}

	virtual void InitializeViewProjMatrix(const Camera* pCam);
	virtual void SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam);

	void BuildViewAndProjectionMatrices();

	inline void SetPosition(const Vec3f& vPos) {m_vPosition = vPos;}
	inline void SetPosition(float fX, float fY, float fZ) 
	{m_vPosition.x = fX; m_vPosition.y = fY; m_vPosition.z = fZ;}

	inline void SetDirection(const Vec3f& vDirection) {m_vDirection = Normalize(vDirection);}
	inline void SetDirection(float fX, float fY, float fZ)
	{m_vDirection.x = fX; m_vDirection.y = fY; m_vDirection.z = fZ; m_vDirection.normalize();}
	inline void SetLookAtPos(const Vec3f& vLookAt)
	{m_vDirection = Normalize(vLookAt - m_vPosition);}

	inline void SetInnerConeRatio(float fRatio) {m_fInnerConeRatio = fRatio;}
	inline void SetInnerConeAngle(float fAngle) {m_fInnerConeRatio = cos(fAngle);}
	inline void SetOuterConeRatio(float fRatio) {m_fOuterConeRatio = fRatio;}
	inline void SetOuterConeAngle(float fAngle) {m_fOuterConeRatio = cos(fAngle);}
	inline void SetRadius(float fRadius) {m_fRadius = fRadius;}

	inline Vec3f GetPosition() const {return m_vPosition;}
	inline Vec3f GetDirection() const {return m_vDirection;}
	inline float GetInnerConeRatio() const {return m_fInnerConeRatio;}
	inline float GetOuterConeRatio() const {return m_fOuterConeRatio;}
	inline float GetRadius() const {return m_fRadius;}

	void SetConstantBufferPS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->PSSetConstantBuffers(SPOTLIGHT_CONST_BUFF_REGISTER, 1, &m_pSpotLightConstBuffer);}
	void SetConstantBufferVS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->VSSetConstantBuffers(SPOTLIGHT_CONST_BUFF_REGISTER, 1, &m_pSpotLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif	//SPOTLIGHT_H