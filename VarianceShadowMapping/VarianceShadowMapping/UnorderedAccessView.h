#ifndef UNORDEREDACCESSVIEW_H
#define UNORDEREDACCESSVIEW_H

#include <d3d11.h>

class UnorderedAccessView
{
private:
	ID3D11UnorderedAccessView* m_pUAV;

public:
	UnorderedAccessView(void);
	~UnorderedAccessView(void);

	void CreateUAV(ID3D11Device* pDevice, ID3D11Resource* pResource, D3D11_UNORDERED_ACCESS_VIEW_DESC* pUAVDesc = NULL);

	ID3D11UnorderedAccessView* GetUAV() const {return m_pUAV;}
};

#endif //UNORDEREDACCESSVIEW_H