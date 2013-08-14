#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <RMMath.h>
using namespace RMMath;

#include "Model.h"

class GameObject
{
private:
	Model m_Model;
	Matrix4f m_Matrix;
	Vec3f m_Velocity;

public:
	GameObject(void);
	~GameObject(void);

	Model* GetModel() {return &m_Model;}

	Matrix4f GetMatrix() const {return m_Matrix;}

	void Update(float fElapsedTime);
};

#endif