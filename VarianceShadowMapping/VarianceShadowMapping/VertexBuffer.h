#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <D3D11.h>
#pragma comment(lib, "d3d11.lib")

#include "VertexDeclarations.h"

class DirectXCore;

class VertexBuffer
{
private:
	ID3D11Buffer* m_pVertexBuffer;
	DirectXCore* m_pCore;
	unsigned int m_unDeclSize;
	unsigned int m_unCurrSize;
	unsigned int m_unNumVertsUsed;
	
public:
	void Initialize(DirectXCore* pCore, unsigned int unDeclSize);
	ID3D11Buffer*& GetBuffer() {return m_pVertexBuffer;}

	void PrepareVertexBuffer();

	const unsigned int& GetStride() const {return m_unDeclSize;}

	void IncrementSize(unsigned int unNumVerts);
	unsigned int AddVerts(void* pVerts, unsigned int unNumVerts);
};

#endif