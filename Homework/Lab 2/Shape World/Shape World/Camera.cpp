#include "Camera.h"
#include "DirectXCore.h"
#include "GameObject.h"

#define TRANSLATION_SPEED 5.0f

#define ROTATION_SPEED 10 * PI

#define CAMERA_CONST_BUFF_REGISTER 0
#define OBJECT_CONST_BUFF_REGISTER 1

void Camera::Initialize(DirectXCore* pCore,/*Renderer* renderer, */float fFOV, float fNearClip, float fFarClip)
{
	//m_pRenderer = renderer;
	m_pCore = pCore;
	m_bDirty = true;

	//m_fFOV = fFOV;
	m_fNearClip = fNearClip;
	m_fFarClip = fFarClip;

	//m_frFrustum.Build(fFOV, fNearClip, fFarClip, 
	//	m_pRenderer->GetScreenWidth() / (float)m_pRenderer->GetScreenHeight(), m_mWorldMatrix);

	m_mProjectionMatrix = XMMatrixPerspectiveFovLH(fFOV, 
			pCore->GetScreenWidth() / (float)pCore->GetScreenHeight(), fNearClip, fFarClip);

	m_mWorldMatrix = m_mViewMatrix = m_ShaderCameraBuffer.m_mViewProjectionMatrix = XMMatrixIdentity();

	D3D11_BUFFER_DESC shaderBuffDesc;
	ZeroMemory(&shaderBuffDesc, sizeof(D3D11_BUFFER_DESC));
	shaderBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderBuffDesc.ByteWidth = sizeof(ShaderCameraBuffer);

	if(FAILED(m_pCore->GetDevice()->CreateBuffer(&shaderBuffDesc, 0, &m_pCameraConstBuffer.p)))
		MessageBox(NULL, L"Failed to create Camera Constant Buffer" , L"", MB_OK | MB_ICONERROR);

	shaderBuffDesc.ByteWidth = sizeof(ShaderObjectBuffer);

	if(FAILED(m_pCore->GetDevice()->CreateBuffer(&shaderBuffDesc, 0, &m_pObjectConstBuffer.p)))
		MessageBox(NULL, L"Failed to create Object Constant Buffer" , L"", MB_OK | MB_ICONERROR);

	m_mWorldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	
	m_pCore->GetContext()->VSSetConstantBuffers(CAMERA_CONST_BUFF_REGISTER, 1, &m_pCameraConstBuffer.p);
	m_pCore->GetContext()->VSSetConstantBuffers(OBJECT_CONST_BUFF_REGISTER, 1, &m_pObjectConstBuffer.p);
}

void Camera::ResetCamera()
{
	//m_mWorldMatrix.make_identity();
	m_bDirty = true;
}

void Camera::ReorientCamera()
{
	//m_mWorldMatrix.make_identity3x3();
	m_bDirty = true;
}

void Camera::UpdateMatrices()
{
	if(m_bDirty)
	{
		m_mViewMatrix = XMMatrixInverse(0, m_mWorldMatrix);//MatrixInverse(m_mWorldMatrix);

		m_ShaderCameraBuffer.m_mViewProjectionMatrix = m_mViewMatrix * m_mProjectionMatrix;
		m_ShaderCameraBuffer.m_mInvViewProjMatrix = XMMatrixInverse(0, m_ShaderCameraBuffer.m_mViewProjectionMatrix);

		D3D11_MAPPED_SUBRESOURCE cameraSubresource;

		m_pCore->GetContext()->Map(m_pCameraConstBuffer.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &cameraSubresource);
			memcpy(cameraSubresource.pData, &m_ShaderCameraBuffer, sizeof(ShaderCameraBuffer));
		m_pCore->GetContext()->Unmap(m_pCameraConstBuffer.p, 0);

		//m_vPrevPos = m_mWorldMatrix.position;
	
		//m_frFrustum.Build(m_fFOV, m_fNearClip, m_fFarClip, 
			//m_pRenderer->GetScreenWidth() / (float)m_pRenderer->GetScreenHeight(), m_mWorldMatrix);

		m_bDirty = false;
	}
}

void Camera::SetMVPAndWorldMatrices(GameObject* pObject)
{
	if(pObject)
	{
		m_ShaderObjectBuffer.m_mMVPMatrix = pObject->GetMatrix() * m_ShaderCameraBuffer.m_mViewProjectionMatrix;
		m_ShaderObjectBuffer.m_mWorldMatrix = pObject->GetMatrix();

		D3D11_MAPPED_SUBRESOURCE objectSubresource;

		m_pCore->GetContext()->Map(m_pObjectConstBuffer.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &objectSubresource);
			memcpy(objectSubresource.pData, &m_ShaderObjectBuffer, sizeof(ShaderObjectBuffer));
		m_pCore->GetContext()->Unmap(m_pObjectConstBuffer.p, 0);
	}
}

void Camera::MoveForward()
{
	//m_mWorldMatrix.MoveForward(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::MoveBackward()
{
	//m_mWorldMatrix.MoveBackward(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::MoveLeft()
{
	//m_mWorldMatrix.MoveLeft(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::MoveRight()
{
	//m_mWorldMatrix.MoveRight(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::MoveUp()
{
	//m_mWorldMatrix.MoveUp(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::MoveDown()
{
	//m_mWorldMatrix.MoveDown(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::RotateLeft()
{
	//m_mWorldMatrix.Rotate_LocalY_Radians(-ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::RotateRight()
{
	//m_mWorldMatrix.Rotate_LocalY_Radians(ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::RotateUp()
{
	//m_mWorldMatrix.Rotate_LocalX_Radians(-ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

void Camera::RotateDown()
{
	//m_mWorldMatrix.Rotate_LocalX_Radians(ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());
	m_bDirty = true;
}

/*float Camera::GetAspectRatio() const
{ 
	return m_pRenderer->GetScreenWidth() / (float)m_pRenderer->GetScreenHeight(); 
}*/