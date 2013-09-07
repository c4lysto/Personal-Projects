#include "ILight.h"
#include "DirectXCore.h"

ID3D11ClassLinkage* ILight::m_pLightClassLinkage = nullptr;

void ILight::CreateClassLinkage(DirectXCore* pCore)
{
	if(!m_pLightClassLinkage)
	{
		if(FAILED(pCore->GetDevice()->CreateClassLinkage(&m_pLightClassLinkage)))
			MessageBox(NULL, L"Failed to create Point Light Class Linkage" , L"", MB_OK | MB_ICONERROR);
	}

	if(m_pLightClassLinkage)
	{

	}
}

void ILight::ReleaseClassLinkage()
{
	m_pLightClassLinkage->Release();
}