#include "VertexBuffer.h"
#include "DirectXCore.h"

#define VERTBUFFER_INCREMENT 1000

void VertexBuffer::Initialize(DirectXCore* pCore, unsigned int unDeclSize)
{
	m_pCore = pCore;
	m_unDeclSize = unDeclSize;

	m_pVertexBuffer = nullptr;
	m_unCurrSize = 0;
	m_unNumVertsUsed = 0;
}

void VertexBuffer::IncrementSize(unsigned int unNumVerts)
{
	unsigned int unIncrementSize = (unNumVerts / VERTBUFFER_INCREMENT) * VERTBUFFER_INCREMENT + VERTBUFFER_INCREMENT;

	m_unCurrSize += unIncrementSize;

	ID3D11Buffer* pOldBuffer = m_pVertexBuffer;

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = m_unDeclSize * m_unCurrSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_pCore->GetDevice()->CreateBuffer(&buffDesc, NULL, &m_pVertexBuffer);

	if(pOldBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE oldBuff, newBuff;

		ID3D11DeviceContext* pContext = m_pCore->GetContext();

		pContext->Map(pOldBuffer, NULL, D3D11_MAP_READ, NULL, &oldBuff);
		pContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE, NULL, &newBuff);
			memcpy(newBuff.pData, oldBuff.pData, m_unNumVertsUsed * m_unDeclSize);
		pContext->Unmap(m_pVertexBuffer, NULL);
		pContext->Unmap(pOldBuffer, NULL);

		SAFE_RELEASE(pOldBuffer);
	}
}

unsigned int VertexBuffer::AddVerts(void* pVerts, unsigned int unNumVerts)
{
	if(unNumVerts + m_unNumVertsUsed > m_unCurrSize)
		IncrementSize((unNumVerts + m_unNumVertsUsed) - m_unCurrSize);

	unsigned int unCurrOffset = m_unNumVertsUsed;

	D3D11_MAPPED_SUBRESOURCE buff;
	m_pCore->GetContext()->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_NO_OVERWRITE,  NULL, &buff);
		memcpy((char*)buff.pData + m_unNumVertsUsed * m_unDeclSize, pVerts, unNumVerts * m_unDeclSize);
	m_pCore->GetContext()->Unmap(m_pVertexBuffer, NULL);

	m_unNumVertsUsed += unNumVerts;

	return unCurrOffset;
}

void VertexBuffer::PrepareVertexBuffer()
{
	unsigned int offset = 0;
	m_pCore->GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_unDeclSize, &offset);
}