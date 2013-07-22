#include "GameObject.h"


GameObject::GameObject(void)
{
	m_Matrix = XMMatrixIdentity();
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


GameObject::~GameObject(void)
{

}

void GameObject::Update(float fElapsedTime)
{
	m_Matrix += XMMatrixTranslation(m_Velocity.x * fElapsedTime, m_Velocity.z * fElapsedTime, m_Velocity.z * fElapsedTime);
}