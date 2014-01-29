#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject(void) : m_pModel(nullptr)
{

}


GameObject::~GameObject(void)
{
	if(m_pModel)
	{
		m_pModel->Release();
		m_pModel = NULL;
	}
}

void GameObject::Update(float fElapsedTime)
{
	
}

bool GameObject::LoadModel(const char* szFilename, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer)
{
	if(m_pModel)
	{
		m_pModel->Release();
		m_pModel = NULL;
	}

	m_pModel = new Model;

	return m_pModel->LoadModel(szFilename, pCore, pVertexBuffer, pIndexBuffer);
}

void GameObject::RenderIndexed(DirectXCore* pCore)
{
	//pCam->SetMVPAndWorldMatrices(m_Matrix);

	if(m_pModel)
		m_pModel->RenderIndexed(pCore);
}

void GameObject::RenderIndexedNoTex(DirectXCore* pCore)
{
	//pLight->SetObjectMatrices(m_Matrix, pCam);

	if(m_pModel)
		m_pModel->RenderIndexedNoTex(pCore);
}