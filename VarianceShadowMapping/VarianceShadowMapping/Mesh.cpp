#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "DDSTextureLoader.h"

#include <sstream>

Mesh::Mesh(void)
{
	m_pPrimitiveData = nullptr;
	ZeroMemory(m_vTextures, sizeof(ID3D11ShaderResourceView*) * MESH_TEX_MAX);
}

Mesh::Mesh(const Mesh& rhs)
{
	m_pPrimitiveData = rhs.m_pPrimitiveData;

	if(m_pPrimitiveData)
		m_pPrimitiveData->AddRef();

	for(size_t i = 0; i < MESH_TEX_MAX; ++i)
	{
		m_vTextures[i] = rhs.m_vTextures[i];

		if(m_vTextures[i])
			m_vTextures[i]->AddRef();
	}
}

Mesh::~Mesh(void)
{
	if(m_pPrimitiveData)
		m_pPrimitiveData->Release();
	
	for(size_t i = 0; i < MESH_TEX_MAX; ++i)
	{
		SAFE_RELEASE(m_vTextures[i]);
	}
}

Mesh& Mesh::operator=(const Mesh& rhs)
{
	if(this != &rhs)
	{
		m_pPrimitiveData = rhs.m_pPrimitiveData;

		if(m_pPrimitiveData)
			m_pPrimitiveData->AddRef();

		for(size_t i = 0; i < MESH_TEX_MAX; ++i)
		{
			m_vTextures[i] = rhs.m_vTextures[i];

			if(m_vTextures[i])
				m_vTextures[i]->AddRef();
		}
	}

	return *this;
}

bool Mesh::LoadMesh(std::ifstream& inFile, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer)
{
	if(inFile.is_open() && inFile.good() && !inFile.eof())
	{
		if(m_pPrimitiveData)
			m_pPrimitiveData->Release();

		m_pPrimitiveData = new MeshPrimitiveData;

		unsigned int unNameLength = 0, unNumTextures = 0, unTexNameLength = 0, unNumPolygons = 0;

		// Get Mesh Name Length
		inFile.read((char*)&unNameLength, sizeof(unsigned int));

		// Get Mesh Name
		char* szMeshName = new char[unNameLength];
		inFile.read(szMeshName, unNameLength);

		m_pPrimitiveData->m_szName = szMeshName;
		delete szMeshName;

		// Get Number of Textures Used
		inFile.read((char*)&unNumTextures, sizeof(unsigned int));

		// Load Textures
		for(unsigned int i = 0; i < unNumTextures; ++i)
		{
			// Get String Length
			inFile.read((char*)&unTexNameLength, sizeof(unsigned int));

			// Get Texture Name
			char* szTextureName = new char[unTexNameLength + 1];
			inFile.read(szTextureName, unTexNameLength);

			std::wstringstream szTextureLoadFile;
			szTextureLoadFile << "Textures/" << szTextureName;

			ID3D11ShaderResourceView* pTextureSRV;
			CreateDDSTextureFromFile(pCore->GetDevice(), szTextureLoadFile.str().c_str(), NULL, &pTextureSRV);

			//	Will Change when the Exporter is changed |
			//											 V
			SetTexture(pTextureSRV,		(MeshTextureEnum)i);
			SAFE_RELEASE(pTextureSRV);

			delete szTextureName;
		}

		// Get VertexType
		unsigned int unVertType;
		inFile.read((char*)&unVertType, sizeof(unsigned int));
		m_pPrimitiveData->m_eVertType = (VertexTypeEnum)unVertType;

		// Get Vertex Stride
		inFile.read((char*)&m_pPrimitiveData->m_unVertexStride, sizeof(unsigned int));

		// Get Number of Vertices
		inFile.read((char*)&m_pPrimitiveData->m_unNumVertices, sizeof(unsigned int));

		char* pVertices = new char[m_pPrimitiveData->m_unNumVertices * m_pPrimitiveData->m_unVertexStride];
		m_pPrimitiveData->m_pVertices = pVertices;

		for(unsigned int i = 0; i < m_pPrimitiveData->m_unNumVertices; ++i)
		{
			inFile.read(pVertices + (i * m_pPrimitiveData->m_unVertexStride), m_pPrimitiveData->m_unVertexStride);
		}

		m_pPrimitiveData->m_unVertexOffset = pVertexBuffer->AddVerts(m_pPrimitiveData->m_pVertices, m_pPrimitiveData->m_unNumVertices);

		inFile.read((char*)&unNumPolygons, sizeof(unsigned int));

		m_pPrimitiveData->m_vIndices.resize(unNumPolygons * 3);

		unsigned int unCurrTriangle;
		for(unsigned int i = 0; i < unNumPolygons; ++i)
		{
			unCurrTriangle = i * 3;

			inFile.read((char*)&m_pPrimitiveData->m_vIndices[unCurrTriangle], sizeof(unsigned int));
			inFile.read((char*)&m_pPrimitiveData->m_vIndices[unCurrTriangle + 1], sizeof(unsigned int));
			inFile.read((char*)&m_pPrimitiveData->m_vIndices[unCurrTriangle + 2], sizeof(unsigned int));
		}

		m_pPrimitiveData->m_unIndexOffset = pIndexBuffer->AddIndices(m_pPrimitiveData->m_vIndices);

		return true;
	}
	else
	{
		MessageBox(NULL, L"Failed To Start Loading Mesh", L"", MB_OK | MB_ICONERROR);

		return false;
	}
}