#include "DirectXCore.h"

DirectXCore::DirectXCore(void)
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
	m_unScreenWidth = usWidth;
	m_unScreenHeight = usHeight;

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

#ifdef _DEBUG
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pContext)))
	{
		MessageBox(NULL, L"Failed to Create the Device and Swap Chain", L"", MB_OK | MB_ICONERROR);
		return false;
	}

	//DXGIGetDebugInterface(DXGI_DEBUG_ALL, (void**)&m_pDebugger);
#else
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pContext)))
	{
		MessageBox(NULL, L"Failed to Create the Device and Swap Chain", L"", MB_OK | MB_ICONERROR);
		return false;
	}
#endif

#pragma region Create BackBuffer
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	m_rtBackBuffer.Create(this, pBackBuffer);

	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = usWidth;
	m_Viewport.Height = usHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &m_Viewport);
#pragma endregion

#pragma region Create Depth Stencil
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_unScreenWidth;
	desc.Height = m_unScreenHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencilTexture;
	if(FAILED(m_pDevice->CreateTexture2D(&desc, NULL, &pDepthStencilTexture)))
	{
		MessageBox(NULL, L"Failed to Create the Depth Buffer Texture", L"", MB_OK | MB_ICONERROR);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Texture2D.MipSlice = 0;

	if(FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &depthStencilDesc, &m_pDepthStencil)))
	{
		MessageBox(NULL, L"Failed to Create the Depth Buffer View", L"", MB_OK | MB_ICONERROR);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if(FAILED(m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDepthStencilState.p)))
	{
		MessageBox(NULL, L"Failed to Create the DepthStencil State", L"", MB_OK | MB_ICONERROR);
		return false;
	}

	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.p, 1);

#pragma endregion

#pragma region Create Rasterizer Desc (Default Values according to MSDN)
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
		return false;
	}

	m_pContext->RSSetState(m_pRasterizerState.p);
#pragma endregion

	return true;
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

void DirectXCore::Clear(RenderTarget* const* pTargets, unsigned int unNumTargets, UINT unClearFlags, 
								float fClearDepth, UINT8 unStencilClear, XMFLOAT4 color)
{
	D3D11_RECT scissorRect = {m_Viewport.TopLeftX, m_Viewport.TopLeftY, m_Viewport.Width, m_Viewport.Height};
	m_pContext->RSSetScissorRects(1, &scissorRect);

	// is binding and setting the clear Rects Necessary???
	if(pTargets)
	{
		for(unsigned int i = 0; i < unNumTargets; ++i)
			m_pTargetViews[i] = m_rtBackBuffer;

		m_pContext->OMSetRenderTargets(unNumTargets, m_pTargetViews, m_pDepthStencil);

		for(unsigned int i = 0; i < unNumTargets; ++i)
			m_pContext->ClearRenderTargetView(m_pTargetViews[i], (const float*)&color);
	}
	else
	{
		ID3D11RenderTargetView* const pBackBuffer = m_rtBackBuffer.GetRenderTargetView();
		m_pContext->OMSetRenderTargets(1, &pBackBuffer, m_pDepthStencil);

		m_pContext->ClearRenderTargetView(m_rtBackBuffer, (const float*)&color);
	}	

	m_pContext->ClearDepthStencilView(m_pDepthStencil, unClearFlags, fClearDepth, unStencilClear);
}

void DirectXCore::Present(UINT syncInterval, UINT flags)
{
	m_pSwapChain->Present(syncInterval, flags);
}