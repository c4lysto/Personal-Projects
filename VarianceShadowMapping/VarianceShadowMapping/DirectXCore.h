#ifndef DIRECTXCORE_H
#define DIRECTXCORE_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <RMMath.h>
using namespace RMMath;

#include <atlbase.h>
#include <vector>

#include "RenderTarget.h"
#include "DepthStencilTarget.h"

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#define PRESENT_IMMEDIATE 0
#define PRESENT_VSYNC 1
#define PRESENT_HALF_VSYNC 2

#define POSTPROCESS_REGISTER 7

enum RasterizerStateEnum {RS_FILL_MODE, RS_CULL_MODE, RS_FRONT_CCW, RS_DEPTH_BIAS,
						RS_SLOPE_SCALED_DEPTH_BIAS, RS_DEPTH_BIAS_CLAMP, RS_DEPTH_CLIP_ENABLE,
						RS_SCISSOR_ENABLE, RS_MULTISAMPLE_ENABLE, RS_ANTIALIASED_LINE_ENABLE};

enum TargetClearFlags {CLEAR_RENDER_TARGETS = 1, CLEAR_DS_BUFFER = 2};

class DirectXCore
{
private:
	struct PostProcessData
	{
		unsigned int m_unScreenWidth;
		unsigned int m_unScreenHeight;
		unsigned int m_unPixelsPerThread;
		float m_fPostProcessFillData;
	};

private:
	CComPtr<ID3D11Device> m_pDevice;
	CComPtr<IDXGISwapChain> m_pSwapChain;
	CComPtr<ID3D11DeviceContext> m_pContext;
	CComPtr<ID3D11Buffer> m_pPostProcessConstBuffer;
	//CComPtr<ID3D11DepthStencilView> m_pDepthStencil;
	DepthStencilTarget m_DepthStencil;
	ID3D11RenderTargetView* m_pTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	std::vector<D3D11_VIEWPORT> m_vViewports;
	unsigned int m_unCurrNumRenderTargets;
	DepthStencilTarget* m_pCurrDepthStencil;
	CComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	CComPtr<ID3D11RasterizerState> m_pRasterizerState;
	D3D11_RASTERIZER_DESC m_RasterizerDesc;
	RenderTarget	m_rtBackBuffer;
	D3D11_VIEWPORT m_Viewport;
	PostProcessData m_PostProcessData;

#ifdef _DEBUG
	IDXGIDebug* m_pDebugger;
#endif

private:
	HRESULT CreateDeviceAndSwapChain(HWND hWnd, bool bWindowed);
	void InitBackBufferAndViewport(unsigned short usWidth, unsigned short usHeight);
	void InitDepthStencilBuffer(unsigned short usWidth, unsigned short usHeight);
	void InitRasterizer();

public:
	bool Initialize(HWND hWnd, unsigned short usWidth, unsigned short usHeight, bool bWindowed);

	ID3D11Device* GetDevice() const {return m_pDevice;}
	IDXGISwapChain* GetSwapChain() const {return m_pSwapChain;}
	ID3D11DeviceContext* GetContext() const {return m_pContext;}
	const D3D11_VIEWPORT& GetViewport() const {return m_Viewport;}
	RenderTarget* GetBackBuffer() {return &m_rtBackBuffer;}

	unsigned int GetScreenWidth() const {return m_PostProcessData.m_unScreenWidth;}
	unsigned int GetScreenHeight() const {return m_PostProcessData.m_unScreenHeight;}

	void SetPostProcessDetails(unsigned int unWidth = 0, unsigned int unHeight = 0);

	void ChangeRasterizerState(RasterizerStateEnum eState, ...);
	void CommitRasterizerChanges();

	void SetRenderTargets(/*const */ID3D11RenderTargetView/*RenderTarget*/** pTargets, unsigned int unNumTargets = 1);
	void SetRenderTargets(std::vector</*const */ID3D11RenderTargetView/*RenderTarget*/*>& vTargets) {SetRenderTargets(vTargets.data(), vTargets.size());}
	void SetMainRenderTarget(ID3D11RenderTargetView* pMainTarget) { SetRenderTargets((pMainTarget) ? &pMainTarget : NULL); }

	int SetNextRenderTarget(/*const */ID3D11RenderTargetView* pTarget) 
	{
		if (pTarget && m_unCurrNumRenderTargets < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			m_pTargetViews[m_unCurrNumRenderTargets++] = pTarget;
			return m_unCurrNumRenderTargets;
		}
		else
			return -1;
	}

	inline void SetDepthStencilTarget(DepthStencilTarget* pDepthStencil) 
	{m_pCurrDepthStencil = (pDepthStencil) ? pDepthStencil : &m_DepthStencil;}

	inline void CommitRenderTargetAndDepthStencil() 
	{m_pContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_pTargetViews, m_pCurrDepthStencil->GetDepthStencilView());}

	inline void SetViewports(D3D11_VIEWPORT* pViewPorts, unsigned int unNumViewports = 1)
	{(pViewPorts) ? m_pContext->RSSetViewports(unNumViewports, pViewPorts) : m_pContext->RSSetViewports(1, &m_Viewport);}


	void Clear(unsigned int unTargetClearFlags = CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER,
		UINT unDSClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float fClearDepth = 1.0f, 
		UINT8 unClearStencil = 0, const Vec4f color = Vec4f(0.5f, 0.5f, 0.5f, 1.0f));

	void Present(UINT syncInterval = PRESENT_VSYNC, UINT flags = 0);

	DirectXCore(void);
	~DirectXCore(void);
};

#endif