#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class DirectXCore;
class VertexBuffer;
class IndexBuffer;

class Model
{
	friend class GameObject;

private:
	unsigned int m_unRefCount;
	string m_szName;
	vector<Mesh> m_vMeshes;

	bool LoadModel(const char* szFilename, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);

public:
	Model(void);
	~Model(void);

	void AddRef() {++m_unRefCount;}
	void Release() {if(--m_unRefCount == 0) delete this;}

	void RenderIndexed(DirectXCore* pCore);
	void RenderIndexedNoTex(DirectXCore* pCore);


	Mesh* GetMesh(size_t index = 0) {return (index < m_vMeshes.size()) ? &m_vMeshes[index] : nullptr;}
};

#endif