#ifndef ILIGHT_H
#define ILIGHT_H

#include <d3d11.h>

#include <RMMath.h>
using namespace RMMath;

class DirectXCore;
class Camera;
struct ID3D11ClassLinkage;
struct ID3D11ClassInstance;

#define SHADOW_RESOLUTION 2048
#define POINT_LIGHT_SHADOW_RESOLUTION 256

#pragma vtordisp(on)
class ILight
{
protected:
	Vec3f m_vColor;
	float m_fSpecularPower;

	static ID3D11Buffer* m_pLightConstBuffer;

private:
	static ID3D11ClassInstance** m_pShaderLinkageArray;

public:
	ILight() : m_vColor(1.0f, 1.0f, 1.0f), m_fSpecularPower(0.0f) {}
	virtual ~ILight() = 0 {}

	static ID3D11ClassLinkage* m_pLightClassLinkage;

	virtual void InitializeViewProjMatrix(const Camera* pCam) = 0;
	//virtual void UpdateViewProjMatrix() = 0;
	virtual void SetObjectMatrices(const Matrix4f& mMatrix, Camera* pCam) = 0;

	virtual Matrix4f GetViewMatrix(unsigned int unCubeFace = 0) const = 0;
	virtual Matrix4f GetViewProjectionMatrix(unsigned int unCubeFace = 0) const = 0;

	Vec3f GetLightColor() const {return m_vColor;}
	float GetSpecularPower() const {return m_fSpecularPower;}

	void SetLightColor(const Vec3f& vColor) {m_vColor = vColor;}
	void SetLightColor(float fX, float fY, float fZ)
	{m_vColor.x = fX; m_vColor.y = fY; m_vColor.z = fZ;}
	void SetSpecularPower(float fSpecularPower) {m_fSpecularPower = fSpecularPower;}

	static void CreateClassLinkage(DirectXCore* pCore);
	static void ReleaseClassLinkage();
};

#endif