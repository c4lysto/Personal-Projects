#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <DirectXMath.h>
using namespace DirectX;

#include "Model.h"

class GameObject
{
private:
	Model m_Model;
	XMMATRIX m_Matrix;
	XMFLOAT3 m_Velocity;

public:
	GameObject(void);
	~GameObject(void);

	Model* GetModel() {return &m_Model;}

	XMMATRIX GetMatrix() const {return m_Matrix;}

	void Update(float fElapsedTime);
};

#endif