#include "Camera.h"
#include "DirectXCore.h"
#include "GameObject.h"

#define TRANSLATION_SPEED 5.0f

#define ROTATION_SPEED PI
#define MOUSE_ROTATION_SPEED PI * 0.1f

void Camera::Initialize(DirectXCore* pCore,/*Renderer* renderer, */float fFOV, float fNearClip, float fFarClip)
{
	//m_pRenderer = renderer;
	m_pCore = pCore;
	m_bDirty = true;

	//m_fFOV = fFOV;
	m_fNearClip = fNearClip;
	m_fFarClip = fFarClip;

	m_fFOV = fFOV;

	//m_frFrustum.Build(fFOV, fNearClip, fFarClip, 
	//	m_pRenderer->GetScreenWidth() / (float)m_pRenderer->GetScreenHeight(), m_mWorldMatrix);

	m_mProjectionMatrix = XMMatrixPerspectiveFovLH(m_fFOV, 
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

	m_mWorldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -10.0f);
	
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

		XMFLOAT3 cameraPos;

		XMStoreFloat3(&cameraPos, m_mWorldMatrix.r[3]);

		memcpy(&m_ShaderCameraBuffer.m_CameraPos, &cameraPos, 12);

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

void Camera::SetMVPAndWorldMatrices(const XMMATRIX& matrix)
{
	m_ShaderObjectBuffer.m_mMVPMatrix = matrix * m_ShaderCameraBuffer.m_mViewProjectionMatrix;
	m_ShaderObjectBuffer.m_mWorldMatrix = matrix;

	D3D11_MAPPED_SUBRESOURCE objectSubresource;

	m_pCore->GetContext()->Map(m_pObjectConstBuffer.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &objectSubresource);
		memcpy(objectSubresource.pData, &m_ShaderObjectBuffer, sizeof(ShaderObjectBuffer));
	m_pCore->GetContext()->Unmap(m_pObjectConstBuffer.p, 0);
}

void Camera::MoveForward(double fElapsedTime)
{
	//m_mWorldMatrix.MoveForward(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	//m_mWorldMatrix *= XMMatrixTranslation(0.0f, 0.0f, float(fElapsedTime * TRANSLATION_SPEED));

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[2] * fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::MoveBackward(double fElapsedTime)
{
	//m_mWorldMatrix.MoveBackward(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	//m_mWorldMatrix *= XMMatrixTranslation(0.0f, 0.0f, float(-fElapsedTime * TRANSLATION_SPEED));

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[2] * -fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::MoveLeft(double fElapsedTime)
{
	//m_mWorldMatrix.MoveLeft(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	//m_mWorldMatrix *= XMMatrixTranslation(float(-fElapsedTime * TRANSLATION_SPEED), 0.0f, 0.0f);

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[0] * -fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::MoveRight(double fElapsedTime)
{
	//m_mWorldMatrix.MoveRight(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	//m_mWorldMatrix *= XMMatrixTranslation(float(fElapsedTime * TRANSLATION_SPEED), 0.0f, 0.0f);

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[0] * fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::MoveUp(double fElapsedTime)
{
	//m_mWorldMatrix.MoveUp(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	m_mWorldMatrix = XMMatrixTranslation(0.0f, float(fElapsedTime * TRANSLATION_SPEED), 0.0f) * m_mWorldMatrix;

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[1] * fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::MoveDown(double fElapsedTime)
{
	//m_mWorldMatrix.MoveDown(TRANSLATION_SPEED * Game::GetInstance()->GetDeltaTime());

	//m_mWorldMatrix = XMMatrixTranslation(0.0f, float(-fElapsedTime * TRANSLATION_SPEED), 0.0f) * m_mWorldMatrix;

	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_mWorldMatrix.r[1] * -fElapsedTime * TRANSLATION_SPEED);

	m_bDirty = true;
}

void Camera::RotateLeft(double fElapsedTime)
{
	//m_mWorldMatrix.Rotate_LocalY_Radians(-ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());

	__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);

	m_mWorldMatrix = XMMatrixRotationAxis(rotationAxis, float(-ROTATION_SPEED * fElapsedTime)) * m_mWorldMatrix;

	m_bDirty = true;
}

void Camera::RotateRight(double fElapsedTime)
{
	//m_mWorldMatrix.Rotate_LocalY_Radians(ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());

	__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);

	m_mWorldMatrix = XMMatrixRotationAxis(rotationAxis, float(ROTATION_SPEED * fElapsedTime)) * m_mWorldMatrix;

	m_bDirty = true;
}

void Camera::RotateUp(double fElapsedTime)
{
	//m_mWorldMatrix.Rotate_LocalX_Radians(-ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());

	__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);

	m_mWorldMatrix = XMMatrixRotationAxis(rotationAxis, float(-ROTATION_SPEED * fElapsedTime)) * m_mWorldMatrix;

	m_bDirty = true;
}

void Camera::RotateDown(double fElapsedTime)
{
	//m_mWorldMatrix.Rotate_LocalX_Radians(ROTATION_SPEED * Game::GetInstance()->GetDeltaTime());

	__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);

	m_mWorldMatrix = XMMatrixRotationAxis(rotationAxis, float(ROTATION_SPEED * fElapsedTime)) * m_mWorldMatrix;

	m_bDirty = true;
}

void Camera::RotateCameraMouseMovement(POINT movement, double fElapsedTime)
{
	if(movement.x)
	{
		__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);

		XMFLOAT4X4 tmp;
		XMStoreFloat4x4(&tmp, m_mWorldMatrix);

		XMFLOAT3 pos(tmp._41, tmp._42, tmp._43);

		m_mWorldMatrix *= XMMatrixTranslation(-pos.x, -pos.y, -pos.z);

		float fDotProduct;
		XMStoreFloat(&fDotProduct, XMVector3Dot(m_mWorldMatrix.r[1], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));

		if(fDotProduct > 0.0f)
			m_mWorldMatrix *= XMMatrixRotationAxis(rotationAxis, float(-MOUSE_ROTATION_SPEED * fElapsedTime * movement.x));
		else
			m_mWorldMatrix *= XMMatrixRotationAxis(rotationAxis, float(MOUSE_ROTATION_SPEED * fElapsedTime * movement.x));

		m_mWorldMatrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);

		m_bDirty = true;
	}

	if(movement.y)
	{
		__m128 rotationAxis = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);

		m_mWorldMatrix = XMMatrixRotationAxis(rotationAxis, float(-MOUSE_ROTATION_SPEED * fElapsedTime * movement.y)) * m_mWorldMatrix;

		m_bDirty = true;
	}
}

// depth is a 0-1 value
XMVECTOR Camera::Unproject(POINT screenPos, float depth)
{
	/*float fX = screenPos.x / (float)m_pCore->GetScreenWidth();
	fX = fX * 0.5f + 0.5f;

	float fY = screenPos.y / (float)m_pCore->GetScreenHeight();
	fY = fY * 0.5f + 0.5f;

	XMVECTOR vec = XMVectorSet(fX,  fY, depth, 1.0f);

	XMMATRIX invViewProj = XMMatrixInverse(0, XMMatrixMultiply(m_mViewMatrix, m_mProjectionMatrix));

	vec = XMVector4Transform(vec, invViewProj);

	XMFLOAT4 tmp;
	XMStoreFloat4(&tmp, vec);

	vec = _mm_div_ps(vec, XMVectorSet(tmp.w, tmp.w, tmp.w, tmp.w));

	return vec;*/

	const D3D11_VIEWPORT& viewport = m_pCore->GetViewport();

	XMVECTOR unprojectedPoint = XMVector3Unproject(XMVectorSet(screenPos.x, screenPos.y, 0, 0), viewport.TopLeftX, viewport.TopLeftY,
		viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, m_mProjectionMatrix, m_mViewMatrix, XMMatrixIdentity());

	return unprojectedPoint - m_mWorldMatrix.r[3];
}

/*float Camera::GetAspectRatio() const
{ 
	return m_pRenderer->GetScreenWidth() / (float)m_pRenderer->GetScreenHeight(); 
}*/