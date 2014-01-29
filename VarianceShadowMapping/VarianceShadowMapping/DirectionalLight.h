#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "ILight.h"

#define DIRLIGHT_CONST_BUFF_REGISTER 5

class Camera;

class DirectionalLight : public ILight
{
private:
	Vec3f m_vDirection;
	float m_fNearClip;
	Matrix4f m_ViewProjTextureMatrix, m_ViewMatrix;
	float m_fFarClip;
	Matrix4f m_ProjectionMatrix, m_ViewProjectionMatrix;

	static ID3D11Buffer* m_pDirLightConstBuffer;
	static ID3D11ClassInstance* m_pDirLightClassInstance;

	void CalculateProjectionMatrix(const Frustum& camFrustum);

public:
	DirectionalLight() {}
	~DirectionalLight() {}

	virtual Matrix4f GetViewMatrix(unsigned int unCubeFace = 0) const {return m_ViewMatrix;}
	virtual Matrix4f GetViewProjectionMatrix(unsigned int unCubeFace = 0) const {return m_ViewProjectionMatrix;}

	inline void SetDirection(const Vec3f& vDirection) {m_vDirection = Normalize(vDirection);}
	inline void SetDirection(float fX, float fY, float fZ)
	{m_vDirection.x = fX; m_vDirection.y = fY; m_vDirection.z = fZ; m_vDirection.normalize();}

	inline Vec3f GetDirection() const {return m_vDirection;}

	void InitializeViewProjMatrix(const Camera* pCam);
	void UpdateViewProjMatrix(const Camera* pCam);
	virtual void SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam);


	void SetConstantBufferPS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->PSSetConstantBuffers(DIRLIGHT_CONST_BUFF_REGISTER, 1, &m_pDirLightConstBuffer);}
	void SetConstantBufferVS(ID3D11DeviceContext* pContext)
	{if(pContext) pContext->VSSetConstantBuffers(DIRLIGHT_CONST_BUFF_REGISTER, 1, &m_pDirLightConstBuffer);}

	void UpdateConstantBuffer(ID3D11DeviceContext* pContext);

	static void CreateConstantBufferAndClassLinkage(ID3D11Device* pDevice, LPCSTR szClassInstanceName);
	static void ReleaseConstantBufferAndClassLinkage();
};

#endif	//DIRECTIONALLIGHT_H