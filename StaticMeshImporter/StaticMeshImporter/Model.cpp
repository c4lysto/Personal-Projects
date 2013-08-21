#include "Model.h"
#include "DirectXCore.h"
#include "VertexDeclarations.h"
using namespace VertexDecl;

#include <fstream>

Model::Model(void)
{
	m_pVertices = nullptr;
	m_unNumVertices = m_unVertexStride = 0;
}

Model::~Model(void)
{
	if(m_pVertices)
		delete[] m_pVertices;
}

void Model::Render(DirectXCore* pCore)
{

}

void Model::CreateBoxVertClr(DirectXCore* pCore)
{
	m_vIndices.clear();
	SAFE_DELETE(m_pVertices);

	m_unNumVertices = 8;
	m_unVertexStride = sizeof(VertClr);

	VertClr* vertices = new VertClr[m_unNumVertices];
	m_pVertices = vertices;

	// Front Top Left - 0
	vertices[0].position = Vec3f(-1.0f, 1.0f, -1.0f);
	vertices[0].color = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);

	// Front Top Right - 1
	vertices[1].position = Vec3f(1.0f, 1.0f, -1.0f);
	vertices[1].color = Vec4f(0.0f, 1.0f, 0.0f, 1.0f);

	// Front Bottom Right - 2
	vertices[2].position = Vec3f(1.0f, -1.0f, -1.0f);
	vertices[2].color = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);

	// Front Bottom Left - 3
	vertices[3].position = Vec3f(-1.0f, -1.0f, -1.0f);
	vertices[3].color = Vec4f(1.0f, 1.0f, 0.0f, 1.0f);


	// Back Top Left - 4
	vertices[4].position = Vec3f(-1.0f, 1.0f, 1.0f);
	vertices[4].color = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);

	// Back Top Right - 5
	vertices[5].position = Vec3f(1.0f, 1.0f, 1.0f);
	vertices[5].color = Vec4f(0.0f, 1.0f, 0.0f, 1.0f);

	// Back Bottom Right - 6
	vertices[6].position = Vec3f(1.0f, -1.0f, 1.0f);
	vertices[6].color = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);

	// Back Bottom Left - 7
	vertices[7].position = Vec3f(-1.0f, -1.0f, 1.0f);
	vertices[7].color = Vec4f(1.0f, 1.0f, 0.0f, 1.0f);

	unsigned int indices[] = 
	{
		0, 1, 2,
		0, 2, 3,

		1, 5, 6,
		1, 6, 2,

		5, 4, 7,
		5, 7, 6,

		4, 0, 3,
		4, 3, 7,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7
	};

	m_vIndices.resize(ARRAYSIZE(indices));

	memcpy(m_vIndices.data(), indices, m_vIndices.size() * sizeof(unsigned int));
}

void Model::CreateBoxVertNormUV(DirectXCore* pCore)
{
	m_vIndices.clear();
	
	if(m_pVertices)
		delete[] m_pVertices;

	m_unNumVertices = 24;
	m_unVertexStride = sizeof(VertNormUV);

	VertNormUV* vertices = new VertNormUV[m_unNumVertices];
	m_pVertices = vertices;

#pragma region Front Face
	// Top Left - 0
	vertices[0].position = Vec3f(-1.0f, 1.0f, -1.0f);
	vertices[0].normal = Vec3f(0.0f, 0.0f, -1.0f);
	vertices[0].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 1
	vertices[1].position = Vec3f(1.0f, 1.0f, -1.0f);
	vertices[1].normal = Vec3f(0.0f, 0.0f, -1.0f);
	vertices[1].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 2
	vertices[2].position = Vec3f(1.0f, -1.0f, -1.0f);
	vertices[2].normal = Vec3f(0.0f, 0.0f, -1.0f);
	vertices[2].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 3
	vertices[3].position = Vec3f(-1.0f, -1.0f, -1.0f);
	vertices[3].normal = Vec3f(0.0f, 0.0f, -1.0f);
	vertices[3].uv = Vec2f(0.0f, 1.0f);
#pragma endregion

#pragma region Right Face
	// Top Left - 4
	vertices[4].position = Vec3f(1.0f, 1.0f, -1.0f);
	vertices[4].normal = Vec3f(1.0f, 0.0f, 0.0f);
	vertices[4].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 5
	vertices[5].position = Vec3f(1.0f, 1.0f, 1.0f);
	vertices[5].normal = Vec3f(1.0f, 0.0f, 0.0f);
	vertices[5].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 6
	vertices[6].position = Vec3f(1.0f, -1.0f, 1.0f);
	vertices[6].normal = Vec3f(1.0f, 0.0f, 0.0f);
	vertices[6].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 7
	vertices[7].position = Vec3f(1.0f, -1.0f, -1.0f);
	vertices[7].normal = Vec3f(1.0f, 0.0f, 0.0f);
	vertices[7].uv = Vec2f(0.0f, 1.0f);
#pragma endregion

#pragma region Back Face
	// Top Left - 8
	vertices[8].position = Vec3f(1.0f, 1.0f, 1.0f);
	vertices[8].normal = Vec3f(0.0f, 0.0f, 1.0f);
	vertices[8].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 9
	vertices[9].position = Vec3f(-1.0f, 1.0f, 1.0f);
	vertices[9].normal = Vec3f(0.0f, 0.0f, 1.0f);
	vertices[9].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 10
	vertices[10].position = Vec3f(-1.0f, -1.0f, 1.0f);
	vertices[10].normal = Vec3f(0.0f, 0.0f, 1.0f);
	vertices[10].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 11
	vertices[11].position = Vec3f(1.0f, -1.0f, 1.0f);
	vertices[11].normal = Vec3f(0.0f, 0.0f, 1.0f);
	vertices[11].uv = Vec2f(0.0f, 1.0f);
#pragma endregion

#pragma region Left Face
	// Top Left - 12
	vertices[12].position = Vec3f(-1.0f, 1.0f, 1.0f);
	vertices[12].normal = Vec3f(-1.0f, 0.0f, 0.0f);
	vertices[12].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 13
	vertices[13].position = Vec3f(-1.0f, 1.0f, -1.0f);
	vertices[13].normal = Vec3f(-1.0f, 0.0f, 0.0f);
	vertices[13].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 14
	vertices[14].position = Vec3f(-1.0f, -1.0f, -1.0f);
	vertices[14].normal = Vec3f(-1.0f, 0.0f, 0.0f);
	vertices[14].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 15
	vertices[15].position = Vec3f(-1.0f, -1.0f, 1.0f);
	vertices[15].normal = Vec3f(-1.0f, 0.0f, 0.0f);
	vertices[15].uv = Vec2f(0.0f, 1.0f);
#pragma endregion

#pragma region Top Face
	// Top Left - 16
	vertices[16].position = Vec3f(-1.0f, 1.0f, 1.0f);
	vertices[16].normal = Vec3f(0.0f, 1.0f, 0.0f);
	vertices[16].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 17
	vertices[17].position = Vec3f(1.0f, 1.0f, 1.0f);
	vertices[17].normal = Vec3f(0.0f, 1.0f, 0.0f);
	vertices[17].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 18
	vertices[18].position = Vec3f(1.0f, 1.0f, -1.0f);
	vertices[18].normal = Vec3f(0.0f, 1.0f, 0.0f);
	vertices[18].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 19
	vertices[19].position = Vec3f(-1.0f, 1.0f, -1.0f);
	vertices[19].normal = Vec3f(0.0f, 1.0f, 0.0f);
	vertices[19].uv = Vec2f(0.0f, 1.0f);
#pragma endregion

#pragma region Bottom Face
	// Top Left - 20
	vertices[20].position = Vec3f(-1.0f, -1.0f, -1.0f);
	vertices[20].normal = Vec3f(0.0f, -1.0f, 0.0f);
	vertices[20].uv = Vec2f(0.0f, 0.0f);

	// Top Right - 21
	vertices[21].position = Vec3f(1.0f, -1.0f, -1.0f);
	vertices[21].normal = Vec3f(0.0f, -1.0f, 0.0f);
	vertices[21].uv = Vec2f(1.0f, 0.0f);

	// Bottom Right - 22
	vertices[22].position = Vec3f(1.0f, -1.0f, 1.0f);
	vertices[22].normal = Vec3f(0.0f, -1.0f, 0.0f);
	vertices[22].uv = Vec2f(1.0f, 1.0f);

	// Bottom Left - 23
	vertices[23].position = Vec3f(-1.0f, -1.0f, 1.0f);
	vertices[23].normal = Vec3f(0.0f, -1.0f, 0.0f);
	vertices[23].uv = Vec2f(0.0f, 1.0f);
#pragma endregion


	unsigned int indices[] = 
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	m_vIndices.resize(ARRAYSIZE(indices));

	memcpy(m_vIndices.data(), indices, m_vIndices.size() * sizeof(unsigned int));
}

bool Model::LoadModel(const char* szFilename)
{
	std::ifstream inFile(szFilename, std::ios_base::in | std::ios_base::binary);

	if(inFile.is_open())
	{
		if(inFile.good())
		{
			unsigned int unNameLength = 0, unNumTextures = 0, unTexNameLength = 0, unNumPolygons = 0;

			// Get Mesh Name Length
			inFile.read((char*)&unNameLength, sizeof(unsigned int));

			// Get Mesh Name
			char* szMeshName = new char[unNameLength];
			inFile.read(szMeshName, unNameLength);

			m_szName = szMeshName;
			delete szMeshName;

			// Get Number of Textures Used
			inFile.read((char*)&unNumTextures, sizeof(unsigned int));
			unNumTextures = 0;

			// Load Textures
			for(unsigned int i = 0; i < unNumTextures; ++i)
			{
				// Get String Length
				inFile.read((char*)&unTexNameLength, sizeof(unsigned int));

				// Get Texture Name
				char* szTextureName = new char[unTexNameLength + 1];
				inFile.read(szTextureName, unTexNameLength);

				delete szTextureName;
			}

			// Get VertexType
			unsigned int unVertType;
			inFile.read((char*)&unVertType, sizeof(unsigned int));
			m_eVertType = (VertexTypeEnum)unVertType;

			// Get Vertex Stride
			inFile.read((char*)&m_unVertexStride, sizeof(unsigned int));

			// Get Number of Vertices
			inFile.read((char*)&m_unNumVertices, sizeof(unsigned int));

			char* pVertices = new char[m_unNumVertices * m_unVertexStride];
			m_pVertices = pVertices;

			for(unsigned int i = 0; i < m_unNumVertices; ++i)
			{
				inFile.read(pVertices + (i * m_unVertexStride), m_unVertexStride);
			}

			inFile.read((char*)&unNumPolygons, sizeof(unsigned int));

			m_vIndices.resize(unNumPolygons * 3);

			unsigned int unCurrTriangle;
			for(unsigned int i = 0; i < unNumPolygons; ++i)
			{
				unCurrTriangle = i * 3;

				inFile.read((char*)&m_vIndices[unCurrTriangle], sizeof(unsigned int));
				inFile.read((char*)&m_vIndices[unCurrTriangle + 1], sizeof(unsigned int));
				inFile.read((char*)&m_vIndices[unCurrTriangle + 2], sizeof(unsigned int));
			}
		}
		else
			return false;
	}
	else
		MessageBox(NULL, L"Error: Model file failed to open!", L"", MB_OK | MB_ICONERROR);

	return true;
}