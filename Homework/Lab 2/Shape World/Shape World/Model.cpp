#include "Model.h"
#include "DirectXCore.h"
#include "VertexDeclarations.h"
using namespace VertexDecl;

Model::Model(void)
{
	m_pVertices = nullptr;
	m_unNumVertices = m_unVertexStride = 0;
}

Model::~Model(void)
{
	SAFE_DELETE(m_pVertices);
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
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// Front Top Right - 1
	vertices[1].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Front Bottom Right - 2
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Front Bottom Left - 3
	vertices[3].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	vertices[3].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);


	// Back Top Left - 4
	vertices[4].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vertices[4].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// Back Top Right - 5
	vertices[5].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Back Bottom Right - 6
	vertices[6].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	vertices[6].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Back Bottom Left - 7
	vertices[7].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vertices[7].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

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

void CreateBoxVertNormUV(DirectXCore* pCore)
{
	
}