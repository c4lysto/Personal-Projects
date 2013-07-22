#include "DirectXCore.h"

DirectXCore::DirectXCore(void)
{
#ifdef _DEBUG
	//m_pDebugger = nullptr;
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
#ifdef _DEBUG
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL,
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
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencilTexture;
	if(FAILED(m_pDevice->CreateTexture2D(&desc, 0, &pDepthStencilTexture)))
	{
		MessageBox(NULL, L"Failed to Create the Depth Buffer Texture", L"", MB_OK | MB_ICONERROR);
		return false;
	}

	if(FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, 0, &m_pDepthStencil)))
	{
		MessageBox(NULL, L"Failed to Create the Depth Buffer View", L"", MB_OK | MB_ICONERROR);
		return false;
	}
#pragma endregion

	return true;
}

void DirectXCore::Clear(RenderTarget* const* pTargets, unsigned int unNumTargets, UINT unClearFlags, 
								float fClearDepth, UINT8 unStencilClear, XMFLOAT4 color)
{
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

	//D3D11_RECT scissorRect = {m_Viewport.TopLeftX, m_Viewport.TopLeftY, m_Viewport.Width, m_Viewport.Height};
	//m_pContext->RSSetScissorRects(1, &scissorRect);

	m_pContext->ClearDepthStencilView(m_pDepthStencil, unClearFlags, fClearDepth, unStencilClear);
}

void DirectXCore::Present(UINT syncInterval, UINT flags)
{
	m_pSwapChain->Present(syncInterval, flags);
}