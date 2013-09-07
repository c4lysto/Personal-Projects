#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject(void)
{

}


GameObject::~GameObject(void)
{

}

void GameObject::Update(float fElapsedTime)
{
	
}

void GameObject::Render(DirectXCore* pCore, Camera* pCam)
{
	pCam->SetMVPAndWorldMatrices(m_Matrix);

	m_Model.Render(pCore);
}