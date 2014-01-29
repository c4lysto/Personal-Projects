#include "DirectXCore.h"

DirectXCore::DirectXCore(void) : m_unCurrNumRenderTargets(0)
{
#ifdef _DEBUG
	m_pDebugger = nullptr;
#endif
}

DirectXCore::~DirectXCore(void)
{
#ifdef _DEBUG
	//if(m_pDebugger)
		//m_pDebugger->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
#endif
}

bool DirectXCore::Initialize(HWND hWnd, unsigned short usWidth, unsigned short usHeight, bool bWindowed)
{
	if(FAILED(CreateDeviceAndSwapChain(hWnd, bWindowed)))
		return false;

	InitBackBufferAndViewport(usWidth, usHeight);

	InitDepthStencilBuffer(usWidth, usHeight);

	InitRasterizer();

	return true;
}

HRESULT DirectXCore::CreateDeviceAndSwapChain(HWND hWnd, bool bWindowed)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;

	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = bWindowed;
	swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	// ******************NOTE******************
	// May want to try D3D11_CREATE_DEVICE_SINGLETHREADED for parameter 4
	D3D_FEATURE_LEVEL featureLevels [] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr;

#ifdef _DEBUG
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pContext);

	//DXGIGetDebugInterface(DXGI_DEBUG_ALL, (void**)&m_pDebugger);
#else
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pContext);
#endif

	if(FAILED(hr))
		MessageBox(NULL, L"Failed to Create the Device and Swap Chain", L"", MB_OK | MB_ICONERROR);

	return hr;
}

void DirectXCore::InitBackBufferAndViewport(unsigned short usWidth, unsigned short usHeight)
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	m_rtBackBuffer.Create(this, pBackBuffer);
	SetRenderTargets(nullptr);

	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = usWidth;
	m_Viewport.Height = usHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &m_Viewport);

	SetViewports(NULL);

	D3D11_BUFFER_DESC shaderBuffDesc;
	ZeroMemory(&shaderBuffDesc, sizeof(D3D11_BUFFER_DESC));
	shaderBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderBuffDesc.ByteWidth = sizeof(PostProcessData);

	if(FAILED(m_pDevice->CreateBuffer(&shaderBuffDesc, 0, &m_pPostProcessConstBuffer.p)))
	{
		MessageBox(NULL, L"Failed to create Post-Process Constant Buffer" , L"", MB_OK | MB_ICONERROR);
		return;
	}

	m_pContext->CSSetConstantBuffers(POSTPROCESS_REGISTER, 1, &m_pPostProcessConstBuffer.p);
	m_pContext->GSSetConstantBuffers(POSTPROCESS_REGISTER, 1, &m_pPostProcessConstBuffer.p);

	SetPostProcessDetails();
}

void DirectXCore::InitDepthStencilBuffer(unsigned short usWidth, unsigned short usHeight)
{
	m_DepthStencil.Create(m_pDevice, usWidth, usHeight);
	SetDepthStencilTarget(nullptr);

	CommitRenderTargetAndDepthStencil();

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if(FAILED(m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDepthStencilState.p)))
	{
		MessageBox(NULL, L"Failed to Create the DepthStencil State", L"", MB_OK | MB_ICONERROR);
		return;
	}

	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.p, 0);
}

void DirectXCore::InitRasterizer()
{
	m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_RasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_RasterizerDesc.FrontCounterClockwise = FALSE;
	m_RasterizerDesc.DepthBias = 0;
	m_RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	m_RasterizerDesc.DepthBiasClamp = 0.0f;
	m_RasterizerDesc.DepthClipEnable = TRUE;
	m_RasterizerDesc.ScissorEnable = FALSE;
	m_RasterizerDesc.MultisampleEnable = FALSE;
	m_RasterizerDesc.AntialiasedLineEnable = FALSE;

	if(FAILED(m_pDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState.p)))
	{
		MessageBox(NULL, L"Failed to Create the Rasterizer State", L"", MB_OK | MB_ICONERROR);
		return;
	}

	m_pContext->RSSetState(m_pRasterizerState.p);
}

void DirectXCore::SetPostProcessDetails(unsigned int unWidth, unsigned int unHeight)
{
	m_PostProcessData.m_unScreenWidth = unWidth ? unWidth : unsigned int(m_Viewport.Width);
	m_PostProcessData.m_unScreenHeight = unHeight ? unHeight : unsigned int(m_Viewport.Height);
	m_PostProcessData.m_unPixelsPerThread = (m_PostProcessData.m_unScreenWidth * m_PostProcessData.m_unScreenHeight) / D3D11_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP;

	if(m_PostProcessData.m_unPixelsPerThread % D3D11_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP)
		m_PostProcessData.m_unPixelsPerThread += 1;

	D3D11_MAPPED_SUBRESOURCE postProcessSubresource;

	m_pContext->Map(m_pPostProcessConstBuffer.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &postProcessSubresource);
		memcpy(postProcessSubresource.pData, &m_PostProcessData, sizeof(PostProcessData));
	m_pContext->Unmap(m_pPostProcessConstBuffer.p, 0);
}

void DirectXCore::ChangeRasterizerState(RasterizerStateEnum eState, ...)
{
	va_list args;
	va_start(args, eState);

	switch(eState)
	{
	case RS_FILL_MODE:
		{
			D3D11_FILL_MODE fillMode = va_arg(args, D3D11_FILL_MODE);

			if(D3D11_FILL_WIREFRAME == fillMode || D3D11_FILL_SOLID == fillMode)
			{
				m_RasterizerDesc.FillMode = fillMode;
			}
		}
		break;

	case RS_CULL_MODE:
		{
			D3D11_CULL_MODE cullMode = va_arg(args, D3D11_CULL_MODE);

			if(cullMode >= D3D11_CULL_NONE && cullMode <= D3D11_CULL_BACK)
			{
				m_RasterizerDesc.CullMode = cullMode;
			}
		}
		break;

	case RS_FRONT_CCW:
		{
			BOOL frontCCW = va_arg(args, BOOL);

			m_RasterizerDesc.FrontCounterClockwise = frontCCW;
		}
		break;

	case RS_DEPTH_BIAS:
		{
			int depthBias = va_arg(args, int);

			m_RasterizerDesc.DepthBias = depthBias;
		}
		break;

	case RS_SLOPE_SCALED_DEPTH_BIAS:
		{
			double depthBias = va_arg(args, double);

			m_RasterizerDesc.SlopeScaledDepthBias = (float)depthBias;
		}
		break;

	case RS_DEPTH_BIAS_CLAMP:
		{
			double depthBiasClamp = va_arg(args, double);

			m_RasterizerDesc.DepthBiasClamp = (float)depthBiasClamp;
		}
		break;

	case RS_DEPTH_CLIP_ENABLE:
		{
			BOOL depthClipEnable = va_arg(args, BOOL);

			m_RasterizerDesc.DepthClipEnable = depthClipEnable;
		}
		break;

	case RS_SCISSOR_ENABLE:
		{
			BOOL scissorEnable = va_arg(args, BOOL);

			m_RasterizerDesc.ScissorEnable = scissorEnable;
		}
		break;

	case RS_MULTISAMPLE_ENABLE:
		{
			BOOL multisampleEnable = va_arg(args, BOOL);

			m_RasterizerDesc.MultisampleEnable = multisampleEnable;
		}
		break;

	case RS_ANTIALIASED_LINE_ENABLE:
		{
			BOOL antialiasedLineEnable = va_arg(args, BOOL);

			m_RasterizerDesc.AntialiasedLineEnable = antialiasedLineEnable;
		}
		break;
	}
}

void DirectXCore::CommitRasterizerChanges()
{
	m_pRasterizerState.Release();

	m_pDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState.p);

	m_pContext->RSSetState(m_pRasterizerState.p);
}

void DirectXCore::SetRenderTargets(/*const */ID3D11RenderTargetView*/*RenderTarget*/* pTargets, unsigned int unNumTargets)
{
	if(pTargets)
	{
		unsigned int currTarget = 0;
		m_unCurrNumRenderTargets = unNumTargets;
		for( ; currTarget < unNumTargets && currTarget < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++currTarget)
		{
			m_pTargetViews[currTarget] = pTargets[currTarget];// .GetRenderTargetView();
		}

		if(currTarget < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			do
			{
				m_pTargetViews[currTarget++] = nullptr;
			}
			while(currTarget < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);
		}
	}
	else
	{
		m_pTargetViews[0] = m_rtBackBuffer.GetRenderTargetView();
		m_unCurrNumRenderTargets = 1;

		for(unsigned int i = 1; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		{
			m_pTargetViews[i] = nullptr;
		}
	}
}

void DirectXCore::Clear(unsigned int unTargetClearFlags, UINT unDSClearFlags, float fClearDepth, UINT8 unClearStencil, const Vec4f color)
{
	if((unTargetClearFlags & CLEAR_RENDER_TARGETS))
	{
		for(unsigned int i = 0; i < m_unCurrNumRenderTargets; ++i)
			m_pContext->ClearRenderTargetView(m_pTargetViews[i], color.vector);
	}

	// if we are supposed to clear the 
	if(unTargetClearFlags & CLEAR_DS_BUFFER)
		m_pContext->ClearDepthStencilView(m_pCurrDepthStencil->GetDepthStencilView(), unDSClearFlags, fClearDepth, unClearStencil);
}

void DirectXCore::Present(UINT syncInterval, UINT flags)
{
	if(DXGI_STATUS_OCCLUDED == m_pSwapChain->Present(syncInterval, flags))
	{}//Sleep(500);
}