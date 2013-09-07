#include "IndexBuffer.h"
#include "DirectXCore.h"

#define INDEXBUFFER_INCREMENT 1000

void IndexBuffer::Initialize(DirectXCore* renderer)
{
	m_pCore = renderer;

	m_pIndexBuffer = nullptr;
	m_unCurrSize = m_unNumIndicesUsed = 0;
}

void IndexBuffer::ClearBuffer()
{
	SAFE_RELEASE(m_pIndexBuffer);
	m_unCurrSize = m_unNumIndicesUsed = 0;
}

void IndexBuffer::IncrementSize(unsigned int unNumIndices)
{
	unsigned int unIncrementSize = (unNumIndices / INDEXBUFFER_INCREMENT) * INDEXBUFFER_INCREMENT + INDEXBUFFER_INCREMENT;

	m_unCurrSize += unIncrementSize;

	ID3D11Buffer* pOldBuffer = m_pIndexBuffer;

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof(unsigned int) * m_unCurrSize;
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_pCore->GetDevice()->CreateBuffer(&buffDesc, NULL, &m_pIndexBuffer);

	if(pOldBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE oldBuff, newBuff;

		ID3D11DeviceContext* pContext = m_pCore->GetContext();

		pContext->Map(pOldBuffer, NULL, D3D11_MAP_READ, NULL, &oldBuff);
		pContext->Map(m_pIndexBuffer, NULL, D3D11_MAP_WRITE, NULL, &newBuff);
			memcpy(newBuff.pData, oldBuff.pData, m_unNumIndicesUsed * sizeof(unsigned int));
		pContext->Unmap(m_pIndexBuffer, NULL);
		pContext->Unmap(pOldBuffer, NULL);

		SAFE_RELEASE(pOldBuffer);
	}
}

unsigned int IndexBuffer::AddIndices(const unsigned int* pIndices, unsigned int unNumIndices)
{
	if(unNumIndices + m_unNumIndicesUsed > m_unCurrSize)
		IncrementSize((unNumIndices + m_unNumIndicesUsed) - m_unCurrSize);

	unsigned int unCurrOffset = m_unNumIndicesUsed;

	D3D11_MAPPED_SUBRESOURCE buff;
	m_pCore->GetContext()->Map(m_pIndexBuffer, NULL, D3D11_MAP_WRITE_NO_OVERWRITE,  NULL, &buff);
		memcpy((char*)buff.pData + m_unNumIndicesUsed * sizeof(unsigned int), pIndices, unNumIndices * sizeof(unsigned int));
	m_pCore->GetContext()->Unmap(m_pIndexBuffer, NULL);

	m_unNumIndicesUsed += unNumIndices;

	return unCurrOffset;
}

unsigned int IndexBuffer::AddIndices(const std::vector<unsigned int>& pIndices)
{
	return AddIndices(pIndices.data(), pIndices.size());
}

void IndexBuffer::PrepareIndexBuffer()
{
	m_pCore->GetContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}