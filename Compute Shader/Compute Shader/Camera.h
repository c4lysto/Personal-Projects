#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>
using namespace DirectX;
#include <atlbase.h>

//class Renderer;
class DirectXCore;
class GameObject;

typedef XMMATRIX Matrix4f;
typedef XMFLOAT3 Vec3f;

#define CAMERA_CONST_BUFF_REGISTER 1
#define OBJECT_CONST_BUFF_REGISTER 2

class Camera
{
private:
	struct ShaderCameraBuffer
	{
		Matrix4f m_mViewProjectionMatrix;
		Matrix4f m_mInvViewProjMatrix;
		XMFLOAT3 m_CameraPos;
	};

	struct ShaderObjectBuffer
	{
		Matrix4f m_mMVPMatrix;
		Matrix4f m_mWorldMatrix;
	};

private:
	Matrix4f m_mWorldMatrix;
	Matrix4f m_mViewMatrix;
	Matrix4f m_mProjectionMatrix;

	CComPtr<ID3D11Buffer> m_pCameraConstBuffer;
	CComPtr<ID3D11Buffer> m_pObjectConstBuffer;

	ShaderCameraBuffer m_ShaderCameraBuffer;
	ShaderObjectBuffer m_ShaderObjectBuffer;
	Vec3f m_vPrevPos;
	DirectXCore* m_pCore;
	float m_fNearClip;
	float m_fFarClip;
	float m_fFOV;

	bool m_bDirty;

public:
	void Initialize(DirectXCore* pCore,/*Renderer* renderer, */float fFOV, float fNearClip, float fFarClip);

	void ResetCamera();
	void ReorientCamera();

	inline bool Dirty() const {return m_bDirty;}

	inline float GetNearClip() const {return m_fNearClip;}
	inline float GetFarClip() const	{return m_fFarClip;}

	inline const Matrix4f& GetWorldMatrix()	const {return m_mWorldMatrix;}
	//inline const Vec3f& GetCameraPos() const	{return m_mWorldMatrix.wAxis;}
	inline const Matrix4f& GetViewMatrix() const	{return m_mViewMatrix;}
	inline const Matrix4f& GetProjectionMatrix() const {return m_mProjectionMatrix;}
	inline const Matrix4f& GetViewProjectionMatrix() const {return m_ShaderCameraBuffer.m_mViewProjectionMatrix;}
	//inline const Matrix4f GetInvViewProjMatrix() const	{return MatrixInverse(m_mViewProjectionMatrix);}
	//inline Frustum GetFrustum() const{return m_frFrustum;}
	inline float GetFOV() const	{ return m_fFOV; }
	float GetAspectRatio() const;

	const CComPtr<ID3D11Buffer>& GetCamConstBuff() const {return m_pCameraConstBuffer;}
	//Vec3f GetCameraMovement() const {return m_mWorldMatrix.position - m_vPrevPos;}

	void SetMVPAndWorldMatrices(GameObject* pObject);
	void SetMVPAndWorldMatrices(const XMMATRIX& matrix);

	inline void SetFOV(float fFOV) {m_fFOV = fFOV; m_bDirty = true;}
	inline void SetNearClip(float fNearClip) {m_fNearClip = fNearClip; m_bDirty = true;}
	inline void SetFarClip(float fFarClip) {m_fFarClip = fFarClip; m_bDirty = true;}

	/*inline void SetPosition(float fX, float fY, float fZ) 
	{m_mWorldMatrix.position.x = fX;
	 m_mWorldMatrix.position.y = fY;
	 m_mWorldMatrix.position.z = fZ; m_bDirty = true;}
	inline void SetPosition(const Vec3f& pos) {m_mWorldMatrix.position = pos; m_bDirty = true;}*/

	void MoveForward(double fElapsedTime);
	void MoveBackward(double fElapsedTime);
	void MoveLeft(double fElapsedTime);
	void MoveRight(double fElapsedTime);
	void MoveUp(double fElapsedTime);
	void MoveDown(double fElapsedTime);

	void RotateLeft(double fElapsedTime);
	void RotateRight(double fElapsedTime);
	void RotateUp(double fElapsedTime);
	void RotateDown(double fElapsedTime);

	void RotateCameraMouseMovement(POINT movement, double fElapsedTime);

	// depth is a 0-1 value
	XMVECTOR Unproject(POINT screenPos, float depth);

	void UpdateMatrices();
};

#endif