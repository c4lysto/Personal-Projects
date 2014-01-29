#include "PixelShaderDynamic.h"
#include <d3d11shader.h>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

PixelShaderDynamic::PixelShaderDynamic(void) : m_pClassLinkage(nullptr), m_pReflector(nullptr)
{
}


PixelShaderDynamic::~PixelShaderDynamic(void)
{
	if(m_pDynamicLinkageArray)
	{
		delete m_pDynamicLinkageArray;
		m_pDynamicLinkageArray = nullptr;
	}

	if(m_pClassLinkage)
	{
		m_pClassLinkage->Release();
		m_pClassLinkage = nullptr;
	}

	if(m_pReflector)
	{
		m_pReflector->Release();
		m_pReflector = nullptr;
	}
}

HRESULT PixelShaderDynamic::CreatePixelShader(ID3D11Device* pDevice, const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	HRESULT retVal = S_OK;

	// Will Return S_OK if device is NULL just in case we want a NULL shader
	// so that we can set the stages of the pipeline as wanted
	if(pDevice)
	{
		InitClassLinkage(pDevice);

		retVal = InitPixelShader(pDevice, pShaderBytecode, unBytecodeLength);	

		if(FAILED(retVal))
			return retVal;


		retVal = InitShaderReflection(pShaderBytecode, unBytecodeLength);

		if(FAILED(retVal))
		{
			MessageBox(NULL, L"Failed To Create Pixel Shader Reflection (Dynamic Linked PS)" , L"", MB_OK | MB_ICONERROR);
			return retVal;
		}
		
		return S_OK;
	}
	else
		return E_ABORT;
}

void PixelShaderDynamic::InitClassLinkage(ID3D11Device* pDevice)
{
	if(m_pClassLinkage)
	{
		m_pClassLinkage->Release();
		m_pClassLinkage = nullptr;
	}

	if(FAILED(pDevice->CreateClassLinkage(&m_pClassLinkage)))
		MessageBox(NULL, L"Failed To Create Class Linkage (Dynamic Linked PS)" , L"", MB_OK | MB_ICONERROR);
}

HRESULT PixelShaderDynamic::InitShaderReflection(const void* pShaderBytecode, SIZE_T unBytecodeLength)
{
	if(m_pReflector)
	{
		m_pReflector->Release();
		m_pReflector = nullptr;
	}

	HRESULT retVal = S_OK;
	
	retVal = D3DReflect(pShaderBytecode, unBytecodeLength, IID_ID3D11ShaderReflection, (void**)&m_pReflector);

	if(FAILED(retVal))
	{
		MessageBox(NULL, L"Failed To Create Pixel Shader Reflection (Dynamic Linked PS)" , L"", MB_OK | MB_ICONERROR);
		return retVal;
	}

	UINT unNumInterfaces = m_pReflector->GetNumInterfaceSlots();
	m_vInterfaceNames.resize(unNumInterfaces);

	m_pDynamicLinkageArray = (ID3D11ClassInstance**)calloc(unNumInterfaces, sizeof(ID3D11ClassInstance*));

	if(!m_pDynamicLinkageArray)
	{
		MessageBox(NULL, L"Not Enough Memory to Allocate Dynamic Linkage Array (Pixel Shader)" , L"", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	return S_OK;
}

UINT PixelShaderDynamic::AddInterfaceByName(LPCSTR szAbstractObjectName, UINT unArrayIndex)
{
	if(m_pReflector)
	{
		ID3D11ShaderReflectionVariable* pVariable = m_pReflector->GetVariableByName(szAbstractObjectName);

		if(!pVariable)
		{
			MessageBox(NULL, L"Failed To Add Interface to Dynamic Linkage Array (Pixel Shader)" , L"", MB_OK | MB_ICONERROR);
			return UINT_MAX;
		}

		UINT unSlotIndex = pVariable->GetInterfaceSlot(unArrayIndex);

		if((size_t)unSlotIndex < m_vInterfaceNames.size())
		{
			m_vInterfaceNames[unSlotIndex] = szAbstractObjectName;
			return unSlotIndex;
		}
	}

	return UINT_MAX;
}

void PixelShaderDynamic::SetClassInstance(LPCSTR szShaderObjectName, LPCSTR szAbstractObjectName, UINT unInstanceIndex)
{
	unsigned int unIndex = UINT_MAX;

	// find index of the class linkage
	for(size_t i = 0; i < m_vInterfaceNames.size(); ++i)
	{
		if(szAbstractObjectName == m_vInterfaceNames[i])
		{
			unIndex = i;
			break;
		}
	}

	// if the object is in the vector and is valid
	if(m_pClassLinkage && unIndex < m_vInterfaceNames.size())
	{
		m_pClassLinkage->GetClassInstance(szShaderObjectName, unInstanceIndex, &m_pDynamicLinkageArray[unIndex]);
	}
}

void PixelShaderDynamic::SetClassInstance(LPCSTR szShaderObjectName, unsigned int unIndex, UINT unInstanceIndex)
{
	if(m_pClassLinkage && unIndex < m_vInterfaceNames.size())
	{
		m_pClassLinkage->GetClassInstance(szShaderObjectName, unInstanceIndex, &m_pDynamicLinkageArray[unIndex]);
	}
}