#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <RMMath.h>
using namespace RMMath;

#include "Model.h"

class Camera;

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

	Matrix4f& GetMatrix() {return m_Matrix;}

	void Update(float fElapsedTime);

	void Render(DirectXCore* pCore, Camera* pCam);
};

#endif