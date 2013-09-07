#include "Model.h"
#include "DirectXCore.h"
#include "VertexDeclarations.h"
using namespace VertexDecl;

#include <fstream>

Model::Model(void)
{

}

Model::~Model(void)
{

}

void Model::Render(DirectXCore* pCore)
{
	for(size_t i = 0; i < m_vMeshes.size(); ++i)
	{
		Mesh& currMesh = m_vMeshes[i];

		// Set the Textures for the Pixel Shader
		currMesh.SetPSTextureSRVs(pCore);

		pCore->GetContext()->IASetPrimitiveTopology(currMesh.GetTopologyType());

		pCore->GetContext()->DrawIndexed(currMesh.GetNumIndices(), currMesh.GetIndexOffset(), currMesh.GetVertexOffset());
	}
}

bool Model::LoadModel(const char* szFilename, DirectXCore* pCore, VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer)
{
	std::ifstream inFile(szFilename, std::ios_base::in | std::ios_base::binary);

	if(inFile.is_open())
	{
		if(inFile.good())
		{
			Mesh newMesh;
			
			if(newMesh.LoadMesh(inFile, pCore, pVertexBuffer, pIndexBuffer))
				m_vMeshes.push_back(newMesh);
		}
		else
			return false;
	}
	else
		MessageBox(NULL, L"Error: Model file failed to open!", L"", MB_OK | MB_ICONERROR);

	return true;
}