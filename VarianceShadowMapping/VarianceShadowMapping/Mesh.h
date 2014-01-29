#ifndef MESH_H
#define MESH_H

#include "VertexDeclarations.h"
using namespace VertexDecl;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <fstream>

#include <d3d11.h>
#include "DirectXCore.h"

class Model;

enum MeshTextureEnum {MESH_TEX_DIFFUSE, MESH_TEX_NORMAL, MESH_TEX_DISPLACEMENT, MESH_TEX_SPECULAR, MESH_TEX_MAX};

class MeshPrimitiveData
{
	friend class Mesh;

private:
	unsigned int m_unRefCount;
	unsigned int m_unNumVertices;
	unsigned int m_unVertexOffset;
	unsigned int m_unIndexOffset;
	unsigned int m_unVertexStride;
	D3D11_PRIMITIVE_TOPOLOGY m_eTopologyType;
	VertexTypeEnum m_eVertType;
	void* m_pVertices;
	vector<unsigned int> m_vIndices;
	string m_szName;

public:
	MeshPrimitiveData() : m_unRefCount(1), m_pVertices(nullptr), m_unNumVertices(0), m_unVertexStride(0), m_unVertexOffset(0), m_unIndexOffset(0),
		// default for now til I find a different way to do it
		//					V
					m_eTopologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), m_eVertType(NUM_VERTEX_TYPES){}

	~MeshPrimitiveData()
	{
		if(m_pVertices)
			delete[] m_pVertices;

		m_unNumVertices = m_unVertexStride = 0;
		m_eVertType = NUM_VERTEX_TYPES;

		m_vIndices.clear();
		m_szName.clear();
	}

	void AddRef() {++m_unRefCount;}
	void Release() 
	{
		if(--m_unRefCount == 0) 
			delete this;
	}
};

class VertexBuffer;
class IndexBuffer;

class Mesh
{
	friend class Model;

private:
	MeshPrimitiveData* m_pPrimitiveData;
	ID3D11ShaderResourceView* m_vTextures[MESH_TEX_MAX];

public:
	Mesh(void);
	Mesh(const Mesh& rhs);
	Mesh& operator=(const Mesh& rhs);
	~Mesh(void);

	bool LoadMesh(std::ifstream& inFile, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer);

	inline void* GetVertices() {return (m_pPrimitiveData) ? m_pPrimitiveData->m_pVertices : nullptr;}
	inline unsigned int GetNumVertices() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_unNumVertices : 0;}
	inline unsigned int GetVertexStride() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_unVertexStride : 0;}
	inline unsigned int GetVertexOffset() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_unVertexOffset : 0;}
	inline unsigned int GetIndexOffset() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_unIndexOffset : 0;}
	inline VertexTypeEnum GetVertexType() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_eVertType : NUM_VERTEX_TYPES;}
	inline const vector<unsigned int>* GetIndices() const {return (m_pPrimitiveData) ? &m_pPrimitiveData->m_vIndices : nullptr;}
	inline unsigned int GetNumIndices() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_vIndices.size() : 0;}
	inline string GetName() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_szName : "";}
	inline D3D11_PRIMITIVE_TOPOLOGY GetTopologyType() const {return (m_pPrimitiveData) ? m_pPrimitiveData->m_eTopologyType : D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;}
	inline void SetTopologyType(D3D11_PRIMITIVE_TOPOLOGY eTopologyType) {m_pPrimitiveData->m_eTopologyType = eTopologyType;}

	inline void SetDSTextureSRVs(DirectXCore* pCore) const
	{
		pCore->GetContext()->DSSetShaderResources(0, MESH_TEX_MAX, m_vTextures);
	}

	inline void SetPSTextureSRVs(DirectXCore* pCore) const
	{
		pCore->GetContext()->PSSetShaderResources(0, MESH_TEX_MAX, m_vTextures);
	}

	inline void SetTexture(ID3D11ShaderResourceView* pTexture, MeshTextureEnum eTextureType)
	{
		if(eTextureType >= MESH_TEX_DIFFUSE && eTextureType < MESH_TEX_MAX)
		{
			SAFE_RELEASE(m_vTextures[eTextureType]);

			m_vTextures[eTextureType] = pTexture;

			if(m_vTextures[eTextureType])
				m_vTextures[eTextureType]->AddRef();
		}
	}

	inline void SetMeshPrimitiveData(MeshPrimitiveData* pPrimitiveData)
	{
		m_pPrimitiveData = pPrimitiveData;

		if(m_pPrimitiveData)
			m_pPrimitiveData->AddRef();
	}
};

#endif