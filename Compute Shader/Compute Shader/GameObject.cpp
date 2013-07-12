#include "GameObject.h"


GameObject::GameObject(void)
{

}


GameObject::~GameObject(void)
{

}

void GameObject::Update(float fElapsedTime)
{
	m_Matrix.position += m_Velocity * fElapsedTime;
}