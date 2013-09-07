#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class DirectXCore;
class VertexBuffer;
class IndexBuffer;

class Model
{
private:
	string m_szName;
	vector<Mesh> m_vMeshes;

public:
	Model(void);
	~Model(void);

	void Render(DirectXCore* pCore);

	bool LoadModel(const char* szFilename, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);

	Mesh* GetMesh(size_t index = 0) {return (index < m_vMeshes.size()) ? &m_vMeshes[index] : nullptr;}
};

#endif