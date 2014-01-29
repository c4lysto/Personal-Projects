#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <RMMath.h>
using namespace RMMath;

#include "Model.h"

class GameObject
{
private:
	Model* m_pModel;
	Matrix4f m_Matrix;
	Vec3f m_Velocity;

public:
	GameObject(void);
	~GameObject(void);

	bool LoadModel(const char* szFilename, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);

	Model* GetModel() const {return m_pModel;}
	void SetModel(Model* pModel) { if(m_pModel) m_pModel->Release();  m_pModel = pModel; if(m_pModel) m_pModel->AddRef(); }

	Matrix4f& GetMatrix() {return m_Matrix;}

	void Update(float fElapsedTime);

	void RenderIndexed(DirectXCore* pCore);
	void RenderIndexedNoTex(DirectXCore* pCore);
};

#endif