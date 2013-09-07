#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <D3D11.h>
#pragma comment(lib, "d3d11.lib")

#include <D3DX11.h>
#pragma comment(lib, "d3dx11.lib")

#include <D3DX10.h>
#pragma comment(lib, "d3dx10.lib")

#include <vector>
using std::vector;

class DirectXCore;

class IndexBuffer
{
private:
	ID3D11Buffer* m_pIndexBuffer;
	DirectXCore* m_pCore;
	unsigned int m_unCurrSize;
	unsigned int m_unNumIndicesUsed;

public:
	void Initialize(DirectXCore* renderer);

	ID3D11Buffer* GetBuffer() const {return m_pIndexBuffer;}

	void ClearBuffer();

	void IncrementSize(unsigned int unNumIndices);
	unsigned int AddIndices(const unsigned int* pIndices, unsigned int unNumIndices);
	unsigned int AddIndices(const vector<unsigned int>& pIndices);

	void PrepareIndexBuffer();
};

#endif