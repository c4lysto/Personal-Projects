//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

/*	NOTE	*/
/* Only way i could figure out how to get the
/	IID_ID3D11ShaderReflection GUID to work
/	properly for Dynamic Shader Linkage	*/
#include <windows.h>
#include <initguid.h>
#include <d3dcompiler.h>
/* END NOTE	*/

#include <iostream>
#include <time.h>
#include "XTime.h"
#include "DirectXCore.h"
#include "GameObject.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "RenderController.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "DebugWindow.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "AmbientLight.h"
#include "PixelShaderDynamic.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "DepthStencilTargetCube.h"
#include "RenderTargetCube.h"
#include "UnorderedAccessView.h"
#include "ComputeShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include <Timer.h>
using namespace VertexDecl;

#include "DDSTextureLoader.h"

using namespace std;

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>
using namespace DirectX;

#if 1//def _DEBUG
#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	720
#else
#define BACKBUFFER_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define BACKBUFFER_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#endif

#define GAME_CONST_BUFF_REGISTER 0
#define VIEW_SHADOW_RENDER_TARGET_REGISTER 4
#define POINT_SHADOW_RENDER_TARGET_REGISTER 5
#define BLUR_CONST_BUFF_REGISTER 8
#define TESS_CONST_BUFF_REGISTER 9

#include "VertNormTanUVTransformedVertex.csh"
#include "VertNormTanUVPixel.csh"
#include "AmbientLightPS.csh"

#include "DirectionalLightVS.csh"
#include "DirectionalLightPS.csh"
#include "DirectionalShadowVS.csh"
#include "DirectionalShadowPS.csh"

#include "SpotLightPS.csh"
#include "SpotLightVS.csh"
#include "SpotLightShadowVS.csh"
#include "SpotLightShadowPS.csh"

#include "PointLightVS.csh"
#include "PointLightGS.csh"
#include "PointLightPS.csh"
#include "PointLightShadowVS.csh"
#include "PointLightShadowGS.csh"
#include "PointLightShadowPS.csh"

#include "SkyDomeVS.csh"
#include "SkyDomePS.csh"

#include "PassThroughScreenVS.csh"
#include "PassThroughPS.csh"

#include "GaussianBlurCS.csh"

#include "HorizontalBlurVS.csh"
#include "HorizontalBlurPS.csh"

#include "RadialBlurDecayPS.csh"

#include "SunVS.csh"
#include "SunPS.csh"
#include "PointToQuadGS.csh"

#include "SolidWireframeGS.csh"

#include "TessellationVS.csh"
#include "DynamicTessDistHS.csh"
#include "DynamicTessDistDS.csh"

VertexShader g_HorizontalBlurVertex;
PixelShader g_HorizontalBlurPixel;
bool g_bBlur = false;
bool g_bTessellate = true;

struct BlurFactors
{
	Vec2f gFocusPoint;
	float fDensity;
	float fDecay;
	float fWeight;
	float fExposure;
	Vec2f BlurFactorsFillData;
};

struct TessShaderBuffer
{
	float m_fMinTessFactor;
	float m_fMaxTessFactor;
	float m_fTessFactorRange; // should be fMaxTessFactor - fMinTessFactor

	// Screen Space Tessellation Variables
	float m_fDesiredPixelsPerEdge;
	float m_fPixelsPerScreenHeight;

	// Distance Tessellation Variables
	float m_fTessMinDist;
	float m_fTessMaxDist;
	float m_fTessDistRange; // should be gTessMaxDist - gTessMinDist

	void SetTessFactors(float fMinTessFactor, float fMaxTessFactor)
	{
		m_fMinTessFactor = fMinTessFactor;
		m_fMaxTessFactor = fMaxTessFactor;
		m_fTessFactorRange = m_fMaxTessFactor - m_fMinTessFactor;
	}

	void SetDesiredPixelsPerEdge(float fPixelsPerEdge, unsigned int nScreenHeight)
	{
		m_fDesiredPixelsPerEdge = fPixelsPerEdge;
		m_fPixelsPerScreenHeight = nScreenHeight / fPixelsPerEdge;
	}

	void SetTessellationDistRange(float fMinDist, float fMaxDist)
	{
		m_fTessMinDist = fMinDist;
		m_fTessMaxDist = fMaxDist;
		m_fTessDistRange = m_fTessMaxDist - m_fTessMinDist;
	}
};

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
private:
	enum RenderFlagEnum {RF_SOLIDWIREFRAME = 0x1, RF_DISTTESS = 0x2, RF_SSTESS = 0x4};

private:
	struct GameShaderBuffer
	{
		Vec2f halfPixel;
		unsigned int unRenderFlags;
		float gWireFrameWidth;

		void SetRenderFlag(RenderFlagEnum eFlag, bool bBit)
		{
			bBit ? unRenderFlags |= eFlag : unRenderFlags &= ~eFlag;
		}

		bool GetRenderFlag(RenderFlagEnum eFlag)
		{
			return (unRenderFlags & eFlag);
		}

		void ToggleRenderFlag(RenderFlagEnum eFlag)
		{
			unRenderFlags ^= eFlag;
		}

		void SetTessellationType(RenderFlagEnum eType)
		{
			// zero out flags
			unRenderFlags &= ~(RF_DISTTESS | RF_SSTESS);
			// turn on appropriate flags
			unRenderFlags |= eType & (RF_DISTTESS | RF_SSTESS);
		}

		unsigned int GetTessellationType()
		{
			return unRenderFlags & (RF_DISTTESS | RF_SSTESS);
		}
	};

private:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	Timer							m_Timer;
	DirectXCore						m_CoreObjects;
	RenderController				m_RenderController;
	GameShaderBuffer				m_GameShaderBuffer;
	TessShaderBuffer				m_TessShaderBuffer;

	GameObject*						m_pBoxes[10][10];
	GameObject*						m_pGround;
	GameObject*						m_pSphere;
	GameObject*						m_pSkyDome;
	GameObject*						m_pScreenQuad;

	BlurFactors					m_BlurFactors;

	std::vector<GameObject*> m_vRenderScene;
	
	CComPtr<ID3D11Buffer> m_pGameConstBuffer;
	CComPtr<ID3D11Buffer> m_pBlurFactorConstBuffer;
	CComPtr<ID3D11Buffer> m_pTessellationConstBuffer;
	CComPtr<ID3D11Buffer> pStructuredBuffer;
	CComPtr<ID3D11BlendState> pBlendState;

	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

	ComputeShader m_GaussianBlurCompute;

	CComPtr<ID3D11InputLayout> m_pInputLayout;

	DepthStencilTarget m_ShadowDepthStencil;
	RenderTarget m_ShadowRenderTarget;
	RenderTargetCube m_PointLightRenderTarget;
	DepthStencilTargetCube m_PointLightDepthStencilCube;
	D3D11_VIEWPORT m_ViewShadowViewport;
	DepthStencilTarget m_PointDepthStencil;

	RenderTarget m_PostProcessTarget1;
	RenderTarget m_PostProcessTarget2;
	RenderTarget m_rtGodRaysTarget;

	UnorderedAccessView m_PostProcessUAV1;
	UnorderedAccessView m_PostProcessUAV2;

	VertexShader m_VertexShader;

	VertexShader m_DirLightVertex;
	VertexShader m_DirShadowVertex;

	VertexShader m_SpotLightVertex;
	VertexShader m_SpotShadowVertex;

	VertexShader m_PointShadowVertex;
	VertexShader m_PointLightVertex;

	VertexShader m_SkyDomeVertex;
	VertexShader m_PassThroughScreenVertex;
	VertexShader m_SunVertex;

	VertexShader m_TessVertex;

	HullShader m_DistTessHull;

	DomainShader m_DistTessDomain;

	GeometryShader m_PointShadowGeometry;
	GeometryShader m_PointLightGeometry;
	GeometryShader m_SunGeometry;
	GeometryShader m_SolidWireframeGeometry;

	PixelShader m_AmbientPixel;
	PixelShader m_DirectionalPixel;
	PixelShader m_PointLightPixel;
	PixelShader m_SpotLightPixel;
	//PixelShaderDynamic m_LightingShader;

	PixelShader m_DirShadowPixel;
	PixelShader m_SpotShadowPixel;
	PixelShader m_PointShadowPixel;

	PixelShader m_SkyDomePixel;
	PixelShader m_PassThroughPixel;

	PixelShader m_RadialBlurPixel;
	PixelShader m_SunPixel;

	AmbientLight m_AmbientLight;
	PointLight m_ptLight;
	DirectionalLight m_dirLight;
	SpotLight m_SpotLight;

	CComPtr<ID3D11ShaderResourceView> m_pShadowMapTexture;

	CComPtr<ID3D11ShaderResourceView> m_pCrateTexture;
	CComPtr<ID3D11ShaderResourceView> m_pCrateNormalTexture;
	CComPtr<ID3D11ShaderResourceView> m_pCrateSpecularTexture;

	CComPtr<ID3D11ShaderResourceView> m_pGroundTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundNormalTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundDisplacementTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundSpecularTexture;

	CComPtr<ID3D11ShaderResourceView> m_pSphereTexture;
	CComPtr<ID3D11ShaderResourceView> m_pSphereNormalTexture;
	CComPtr<ID3D11ShaderResourceView> m_pSphereSpecularTexture;

	CComPtr<ID3D11ShaderResourceView> m_pSunDiffuseTexture;


	CComPtr<ID3D11ShaderResourceView> m_pSkyDomeTexture;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera m_Camera;

	void Update();
	void UpdateConstBuff(ID3D11Resource* pResource, void* pData, size_t unCopySize);

	void PrepareAmbientPass();
	void PrepareDirShadowPass();
	void PrepareDirLightPass();
	void PrepareSpotShadowPass();
	void PrepareSpotLightPass();

	void PreparePointShadowPass();
	void PreparePointLightPass();

	void RevertRenderTargetAndDepthStencil();

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateShaders();
	void CreateInputLayout();
	void InitLights();

	void CreateVertexShaders();
	void CreateHullShaders();
	void CreateGeometryShaders();
	void CreateDomainShaders();
	void CreatePixelShaders();

	void CreateScreenQuad();
	void CreateBox();
	void CreateGround();
	void CreateSphere();
	void CreateSkyDome();

	void CreateShadowDepthStencilAndRenderTarget();

	void RenderShadowPass(ILight* pLight, bool bRenderCubeFaces = false);

	void RenderLightPass();

	void RenderSkyDome();
	void RenderSun();

	void RenderGaussianBlur();
	void ExtendGodRays();

	void RenderScreenQuad(VertexShader* pVertexShader, PixelShader* pPixelShader, ID3D11ShaderResourceView* pTexture, RenderTarget* pRenderTarget = NULL);

	void CreateDiffuseTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateNormalTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateDisplacementTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateSpecularTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);

	void ResetPipelineShaders();

	void SetupShaderConstBuffers();

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	void OnMouseMove(HWND hWnd, LPARAM lParam);
	void OnKeyPress(WPARAM wParam);

	void SetCurrentMousePos(POINT mousePos) {m_PrevMousePos = mousePos;}
};

DEMO_APP myApp;
bool m_bRButtonDown = false;
bool g_RenderPtCubeFaces = true;
bool g_bRenderWireframe = false;
POINT lockedMousePos;

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

void DEMO_APP::Initialize(HINSTANCE hinst, WNDPROC proc)
{
	application = hinst; 
	appWndProc = proc; 

	WNDCLASSEX  wndClass;
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );             
    wndClass.lpfnWndProc    = appWndProc;						
    wndClass.lpszClassName  = L"DirectXApplication";            
	wndClass.hInstance      = application;		               
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    
    wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME );
    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(	L"DirectXApplication", L"Variance Shadow Mapping",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );

	DebugWindow::Initialize();
	//********************* END WARNING ************************//

	m_CoreObjects.Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, true);
	m_RenderController.Initialize(&m_CoreObjects);
	m_Camera.Initialize(&m_CoreObjects, DEG_TO_RAD(42.0f), 0.1f, 50.0f);
	SetupShaderConstBuffers();

	ILight::CreateClassLinkage(&m_CoreObjects);
	AmbientLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gAmbientLight");
	PointLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gPointLight");
	DirectionalLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gDirLight");
	SpotLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gSpotLight");
	IShader::SetDeviceContext(m_CoreObjects.GetContext());

	CreateShadowDepthStencilAndRenderTarget();

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateShaders();
	CreateInputLayout();
	InitLights();

	CreateScreenQuad();
	CreateBox();
	CreateGround();
	CreateSphere();
	CreateSkyDome();

	ZeroMemory(&m_GameShaderBuffer, sizeof(GameShaderBuffer));

	m_GameShaderBuffer.halfPixel.x = 0.5f / m_CoreObjects.GetScreenWidth();
	m_GameShaderBuffer.halfPixel.y = 0.5f / m_CoreObjects.GetScreenHeight();
	m_GameShaderBuffer.gWireFrameWidth = 1.0f;
	m_GameShaderBuffer.SetTessellationType(RF_DISTTESS);

	UpdateConstBuff(m_pGameConstBuffer.p, &m_GameShaderBuffer, sizeof(m_GameShaderBuffer));

	m_Timer.Reset();
}

void DEMO_APP::SetupShaderConstBuffers()
{
	D3D11_BUFFER_DESC shaderConstBuffDesc;
	ZeroMemory(&shaderConstBuffDesc, sizeof(D3D11_BUFFER_DESC));
	shaderConstBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderConstBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderConstBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderConstBuffDesc.ByteWidth = sizeof(GameShaderBuffer);

	// Game Const Buffer
	if(FAILED(m_CoreObjects.GetDevice()->CreateBuffer(&shaderConstBuffDesc, NULL, &m_pGameConstBuffer.p)))
		MessageBox(NULL, L"Failed to create Game Constant Buffer", L"", MB_OK | MB_ICONERROR);

	m_CoreObjects.GetContext()->CSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &m_pGameConstBuffer.p);
	m_CoreObjects.GetContext()->VSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &m_pGameConstBuffer.p);
	m_CoreObjects.GetContext()->HSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &m_pGameConstBuffer.p);
	m_CoreObjects.GetContext()->DSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &m_pGameConstBuffer.p);
	m_CoreObjects.GetContext()->PSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &m_pGameConstBuffer.p);


	// God Ray Blur Factors
	shaderConstBuffDesc.ByteWidth = sizeof(BlurFactors);

	if(FAILED(m_CoreObjects.GetDevice()->CreateBuffer(&shaderConstBuffDesc, NULL, &m_pBlurFactorConstBuffer.p)))
		MessageBox(NULL, L"Failed to create BlurFactors Const Buffer", L"", MB_OK | MB_ICONERROR);

	m_BlurFactors.fDensity = 1.0f;// 0.95f;
	m_BlurFactors.fDecay = 1.0f;// 0.95f;
	m_BlurFactors.fWeight = 0.75f;// 5.65f;// 1.0f;
	m_BlurFactors.fExposure = 0.0255f;// 0.15f;


	// Tessellation Const Buffer
	shaderConstBuffDesc.ByteWidth = sizeof(TessShaderBuffer);

	if(FAILED(m_CoreObjects.GetDevice()->CreateBuffer(&shaderConstBuffDesc, NULL, &m_pTessellationConstBuffer.p)))
		MessageBox(NULL, L"Failed to create BlurFactors Const Buffer", L"", MB_OK | MB_ICONERROR);

	m_TessShaderBuffer.SetTessFactors(1.0f, 64.0f);
	m_TessShaderBuffer.SetDesiredPixelsPerEdge(20.0f, m_CoreObjects.GetScreenHeight());
	m_TessShaderBuffer.SetTessellationDistRange(0.1f, 25.0f);

	m_CoreObjects.GetContext()->HSSetConstantBuffers(TESS_CONST_BUFF_REGISTER, 1, &m_pTessellationConstBuffer.p);
	m_CoreObjects.GetContext()->DSSetConstantBuffers(TESS_CONST_BUFF_REGISTER, 1, &m_pTessellationConstBuffer.p);

	UpdateConstBuff(m_pTessellationConstBuffer.p, &m_TessShaderBuffer, sizeof(m_TessShaderBuffer));
}

void DEMO_APP::CreateVertexBuffer()
{
	m_VertexBuffer.Initialize(&m_CoreObjects, VERTNORMTANUV_SIZE);
}

void DEMO_APP::CreateIndexBuffer()
{
	m_IndexBuffer.Initialize(&m_CoreObjects);
}

void DEMO_APP::CreateShaders()
{
	CreateVertexShaders();
	CreateHullShaders();
	CreateDomainShaders();
	CreateGeometryShaders();
	CreatePixelShaders();	

	m_GaussianBlurCompute.CreateComputeShader(m_CoreObjects.GetDevice(), GaussianBlurCS, ARRAYSIZE(GaussianBlurCS));
}

void DEMO_APP::CreateVertexShaders()
{
	m_VertexShader.CreateVertexShader(m_CoreObjects.GetDevice(), VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex));
	m_PassThroughScreenVertex.CreateVertexShader(m_CoreObjects.GetDevice(), PassThroughScreenVS, ARRAYSIZE(PassThroughScreenVS));

	m_DirShadowVertex.CreateVertexShader(m_CoreObjects.GetDevice(), DirectionalShadowVS, ARRAYSIZE(DirectionalShadowVS));
	m_DirLightVertex.CreateVertexShader(m_CoreObjects.GetDevice(), DirectionalLightVS, ARRAYSIZE(DirectionalLightVS));

	m_SpotLightVertex.CreateVertexShader(m_CoreObjects.GetDevice(), SpotLightVS, ARRAYSIZE(SpotLightVS));
	m_SpotShadowVertex.CreateVertexShader(m_CoreObjects.GetDevice(), SpotLightShadowVS, ARRAYSIZE(SpotLightShadowVS));

	m_PointShadowVertex.CreateVertexShader(m_CoreObjects.GetDevice(), PointLightShadowVS, ARRAYSIZE(PointLightShadowVS));
	m_PointLightVertex.CreateVertexShader(m_CoreObjects.GetDevice(), PointLightVS, ARRAYSIZE(PointLightVS));

	m_SkyDomeVertex.CreateVertexShader(m_CoreObjects.GetDevice(), SkyDomeVS, ARRAYSIZE(SkyDomeVS));

	m_SunVertex.CreateVertexShader(m_CoreObjects.GetDevice(), SunVS, ARRAYSIZE(SunVS));

	g_HorizontalBlurVertex.CreateVertexShader(m_CoreObjects.GetDevice(), HorizontalBlurVS, ARRAYSIZE(HorizontalBlurVS));

	m_TessVertex.CreateVertexShader(m_CoreObjects.GetDevice(), TessellationVS, ARRAYSIZE(TessellationVS));
}

void DEMO_APP::CreateHullShaders()
{
	m_DistTessHull.CreateHullShader(m_CoreObjects.GetDevice(), DynamicTessDistHS, ARRAYSIZE(DynamicTessDistHS));
}

void DEMO_APP::CreateGeometryShaders()
{
	m_PointShadowGeometry.CreateGeometryShader(m_CoreObjects.GetDevice(), PointLightShadowGS, ARRAYSIZE(PointLightShadowGS));
	m_PointLightGeometry.CreateGeometryShader(m_CoreObjects.GetDevice(), PointLightGS, ARRAYSIZE(PointLightGS));
	m_SunGeometry.CreateGeometryShader(m_CoreObjects.GetDevice(), PointToQuadGS, ARRAYSIZE(PointToQuadGS));
	m_SolidWireframeGeometry.CreateGeometryShader(m_CoreObjects.GetDevice(), SolidWireframeGS, ARRAYSIZE(SolidWireframeGS));
}

void DEMO_APP::CreateDomainShaders()
{
	m_DistTessDomain.CreateDomainShader(m_CoreObjects.GetDevice(), DynamicTessDistDS, ARRAYSIZE(DynamicTessDistDS));
}

void DEMO_APP::CreatePixelShaders()
{
	m_AmbientPixel.CreatePixelShader(m_CoreObjects.GetDevice(), AmbientLightPS, ARRAYSIZE(AmbientLightPS));
	m_PassThroughPixel.CreatePixelShader(m_CoreObjects.GetDevice(), PassThroughPS, ARRAYSIZE(PassThroughPS));

	m_DirectionalPixel.CreatePixelShader(m_CoreObjects.GetDevice(), DirectionalLightPS, ARRAYSIZE(DirectionalLightPS));
	m_DirShadowPixel.CreatePixelShader(m_CoreObjects.GetDevice(), DirectionalShadowPS, ARRAYSIZE(DirectionalShadowPS));

	m_PointLightPixel.CreatePixelShader(m_CoreObjects.GetDevice(), PointLightPS, ARRAYSIZE(PointLightPS));

	m_SpotLightPixel.CreatePixelShader(m_CoreObjects.GetDevice(), SpotLightPS, ARRAYSIZE(SpotLightPS));
	m_SpotShadowPixel.CreatePixelShader(m_CoreObjects.GetDevice(), SpotLightShadowPS, ARRAYSIZE(SpotLightShadowPS));

	m_PointShadowPixel.CreatePixelShader(m_CoreObjects.GetDevice(), PointLightShadowPS, ARRAYSIZE(PointLightShadowPS));

	m_SkyDomePixel.CreatePixelShader(m_CoreObjects.GetDevice(), SkyDomePS, ARRAYSIZE(SkyDomePS));

	m_SunPixel.CreatePixelShader(m_CoreObjects.GetDevice(), SunPS, ARRAYSIZE(SunPS));

	g_HorizontalBlurPixel.CreatePixelShader(m_CoreObjects.GetDevice(), HorizontalBlurPS, ARRAYSIZE(HorizontalBlurPS));
	m_RadialBlurPixel.CreatePixelShader(m_CoreObjects.GetDevice(), RadialBlurDecayPS, ARRAYSIZE(RadialBlurDecayPS));

	//m_LightingShader.CreatePixelShader(m_CoreObjects.GetContext(), m_CoreObjects.GetDevice(), VertNormTanUVPixel, ARRAYSIZE(VertNormTanUVPixel));
	//m_LightingShader.SetClassInstance("gAmbientLight", m_LightingShader.AddInterfaceByName("gLights"));

	//m_LightingShader.SetClassInstance("gAmbientLight", m_LightingShader.AddInterfaceByName("gLights"));
	//m_LightingShader.SetClassInstance("gDirLight", m_LightingShader.AddInterfaceByName("gLights", 1));
	//m_LightingShader.SetClassInstance("gPointLight", m_LightingShader.AddInterfaceByName("gLights", 2));
	//m_LightingShader.SetClassInstance("gSpotLight", m_LightingShader.AddInterfaceByName("gLights", 3));
}

void DEMO_APP::CreateShadowDepthStencilAndRenderTarget()
{
	m_ShadowRenderTarget.Create(&m_CoreObjects, SHADOW_RESOLUTION, SHADOW_RESOLUTION, DXGI_FORMAT_R32G32_FLOAT);
	m_ShadowDepthStencil.Create(m_CoreObjects.GetDevice(), SHADOW_RESOLUTION, SHADOW_RESOLUTION, DXGI_FORMAT_R32_TYPELESS);
	m_rtGodRaysTarget.Create(&m_CoreObjects);

	m_PointLightRenderTarget.Create(&m_CoreObjects, POINT_LIGHT_SHADOW_RESOLUTION, POINT_LIGHT_SHADOW_RESOLUTION, DXGI_FORMAT_R32G32_FLOAT);
	m_PointLightDepthStencilCube.Create(m_CoreObjects.GetDevice(), POINT_LIGHT_SHADOW_RESOLUTION, POINT_LIGHT_SHADOW_RESOLUTION, DXGI_FORMAT_R32_TYPELESS);
	m_PointDepthStencil.Create(m_CoreObjects.GetDevice(), POINT_LIGHT_SHADOW_RESOLUTION, POINT_LIGHT_SHADOW_RESOLUTION, DXGI_FORMAT_R32_TYPELESS);

	m_PostProcessTarget1.Create(&m_CoreObjects);
	m_PostProcessUAV1.CreateUAV(m_CoreObjects.GetDevice(), m_PostProcessTarget1.GetRenderTexture());
	m_CoreObjects.SetMainRenderTarget(m_PostProcessTarget1.GetRenderTargetView());

	m_PostProcessTarget2.Create(&m_CoreObjects);
	m_PostProcessUAV2.CreateUAV(m_CoreObjects.GetDevice(), m_PostProcessTarget2.GetRenderTexture());

	m_ViewShadowViewport.Width = SHADOW_RESOLUTION;
	m_ViewShadowViewport.Height = SHADOW_RESOLUTION;
	m_ViewShadowViewport.MaxDepth = 1.0f;
	m_ViewShadowViewport.MinDepth = 0.0f;
	m_ViewShadowViewport.TopLeftX = 0.0f;
	m_ViewShadowViewport.TopLeftY = 0.0f;
}

void DEMO_APP::InitLights()
{
	m_AmbientLight.SetLightColor(0.25f, 0.25f, 0.25f);// 0.025f, 0.025f, 0.025f);
	m_AmbientLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_AmbientLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_ptLight.SetLightColor(1.0f, 0.0f, 0.0f);
	m_ptLight.SetPosition(0.0f, 3.0f, 7.0f);
	m_ptLight.SetRadius(15.0f);
	m_ptLight.SetSpecularPower(66.0f);
	m_ptLight.UpdateViewProjMatrix(NULL);
	m_ptLight.SetConstantBufferVS(m_CoreObjects.GetContext());
	//m_ptLight.SetConstantBufferGS(m_CoreObjects.GetContext());
	m_ptLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_ptLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_SpotLight.SetLightColor(0.0f, 1.0f, 0.0f);
	m_SpotLight.SetPosition(10.0f, 10.0f, 0.0f);
	m_SpotLight.SetLookAtPos(Vec3f());
	m_SpotLight.SetSpecularPower(50.0f);
	m_SpotLight.SetInnerConeAngle(DEG_TO_RAD(10));
	m_SpotLight.SetOuterConeAngle(DEG_TO_RAD(15));
	m_SpotLight.SetRadius(30.0f);
	m_SpotLight.BuildViewAndProjectionMatrices();
	m_SpotLight.SetConstantBufferVS(m_CoreObjects.GetContext());
	m_SpotLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_SpotLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_dirLight.SetLightColor(0.752941f, 0.749019f, 0.678431f);
	m_dirLight.SetDirection(0.80233564f, -0.457333148f, 0.383493602f);
	m_dirLight.SetSpecularPower(50.0f);
	m_dirLight.InitializeViewProjMatrix(&m_Camera);
	m_dirLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_dirLight.SetConstantBufferVS(m_CoreObjects.GetContext());
	m_dirLight.UpdateConstantBuffer(m_CoreObjects.GetContext());
}

void DEMO_APP::CreateScreenQuad()
{
	m_pScreenQuad = new GameObject;
	m_pScreenQuad->LoadModel("Models/ScreenQuad.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);
}

void DEMO_APP::CreateBox()
{
	GameObject tmpBox;
	tmpBox.LoadModel("Models/Box.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);

	// Setup Box Textures
	CreateDiffuseTexture(L"Textures/crate.dds", tmpBox.GetModel()->GetMesh(), &m_pCrateTexture.p);
	CreateNormalTexture(L"Textures/Crate_Normal.dds", tmpBox.GetModel()->GetMesh(), &m_pCrateNormalTexture.p);
	CreateSpecularTexture(L"Textures/Crate_Specular.dds", tmpBox.GetModel()->GetMesh(), &m_pCrateSpecularTexture.p);

	/*Vec3f currPos(-9.5f, 0.5f, -9.5f);
	for(unsigned int i = 0; i < 10; ++i)
	{
		for(unsigned int j = 0; j < 10; ++j)
		{
			m_pBoxes[i][j] = new GameObject;
			m_pBoxes[i][j]->SetModel(tmpBox.GetModel());
			m_pBoxes[i][j]->GetMatrix().position = currPos;
			currPos.x += 2.0f;
			m_vRenderScene.push_back(m_pBoxes[i][j]);
		}
		currPos.z += 2.0f;
		currPos.x = -9.5f;
	}*/
}

void DEMO_APP::CreateGround()
{
	m_pGround = new GameObject;
	m_pGround->LoadModel("Models/Quad2_Sub10x10.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);
	m_pGround->GetMatrix().SetScale(20.0f);

	// Setup Ground Textures
	CreateDiffuseTexture(L"Textures/Brick_Diffuse.dds", m_pGround->GetModel()->GetMesh(), &m_pGroundTexture.p);
	CreateNormalTexture(L"Textures/Brick_Normal.dds", m_pGround->GetModel()->GetMesh(), &m_pGroundNormalTexture.p);
	CreateDisplacementTexture(L"Textures/Brick_Displacement.dds", m_pGround->GetModel()->GetMesh(), &m_pGroundDisplacementTexture.p);
	CreateSpecularTexture(L"Textures/Brick_Specular.dds", m_pGround->GetModel()->GetMesh(), &m_pGroundSpecularTexture.p);

	m_pGround->GetModel()->GetMesh()->SetTopologyType(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	m_vRenderScene.push_back(m_pGround);
}

void DEMO_APP::CreateSphere()
{
	m_pSphere = new GameObject;
	m_pSphere->LoadModel("Models/Sphere.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);
	m_pSphere->GetMatrix().position = Vec3f(-7.5f, 5.0f, 0.0f);

	CreateDiffuseTexture(L"Textures/Earth_Diffuse.dds", m_pSphere->GetModel()->GetMesh(), &m_pSphereTexture.p);
	CreateNormalTexture(L"Textures/Earth_Normal.dds", m_pSphere->GetModel()->GetMesh(), &m_pSphereNormalTexture.p);
	CreateSpecularTexture(L"Textures/Earth_Specular.dds", m_pSphere->GetModel()->GetMesh(), &m_pSphereSpecularTexture.p);

	//m_vRenderScene.push_back(m_pSphere);
}

void DEMO_APP::CreateSkyDome()
{
	m_pSkyDome = new GameObject;
	m_pSkyDome->LoadModel("Models/Sphere_10x10.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);

	CreateDiffuseTexture(L"Textures/SkyDome_NoSun_Diffuse.dds", m_pSkyDome->GetModel()->GetMesh(), &m_pSkyDomeTexture.p);

	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), L"Textures/Sun.dds", NULL, &m_pSunDiffuseTexture.p);
}

void DEMO_APP::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTNORMTANUV_LAYOUT;

	if(FAILED(m_CoreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTNORMTANUV_ELEMENTS, VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex), &m_pInputLayout.p)))
		MessageBox(window, L"Failed To Create Input Layout", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateDiffuseTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_DIFFUSE);
}

void DEMO_APP::CreateNormalTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_NORMAL);
}

void DEMO_APP::CreateDisplacementTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_DISPLACEMENT);
}

void DEMO_APP::CreateSpecularTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_SPECULAR);
}

void DEMO_APP::ResetPipelineShaders()
{
	VertexShader::ResetVertexShader();
	HullShader::ResetHullShader();
	DomainShader::ResetDomainShader();
	GeometryShader::ResetGeometryShader();
	PixelShader::ResetPixelShader();
}

void DEMO_APP::OnMouseMove(HWND hWnd, LPARAM lParam)
{
	POINT currMousePos = {LOWORD(lParam), HIWORD(lParam)};

	if(m_bRButtonDown)
	{
		POINT mouseMovement = {m_PrevMousePos.x - currMousePos.x, m_PrevMousePos.y - currMousePos.y};

		m_Camera.RotateCameraMouseMovement(mouseMovement, m_Timer.GetDeltaTimeF());

		POINT screenPos = lockedMousePos;
		ClientToScreen(hWnd, &screenPos);
		SetCursorPos(screenPos.x, screenPos.y);

		m_PrevMousePos = lockedMousePos;

		return;
	}
		
	m_PrevMousePos = currMousePos;
}

void DEMO_APP::PrepareAmbientPass()
{
	ResetPipelineShaders();

	m_CoreObjects.SetNextRenderTarget(m_rtGodRaysTarget.GetRenderTargetView());
	m_CoreObjects.CommitRenderTargetAndDepthStencil();

	m_CoreObjects.Clear(CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER, D3D11_CLEAR_DEPTH, 1, 0, Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

	m_RenderController.SetBlendState(BlendModeEnum::NO_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_STENCIL_DEFAULT);
	//m_LightingShader.SetClassInstance("gAmbientLight", "gLights");

	if(g_bTessellate)
	{
		m_TessVertex.SetVertexShader();
		m_DistTessHull.SetHullShader();
		m_DistTessDomain.SetDomainShader();
		m_SolidWireframeGeometry.SetGeometryShader();
	}
	else
	{
		m_VertexShader.SetVertexShader();
	}

	m_AmbientPixel.SetPixelShader();
}

void DEMO_APP::RenderLightPass()
{
	for(std::vector<GameObject*>::iterator iter = m_vRenderScene.begin(); iter != m_vRenderScene.end(); ++iter)
	{
		m_Camera.SetMVPAndWorldMatrices((*iter)->GetMatrix());
		(*iter)->RenderIndexed(&m_CoreObjects);
	}
}

void DEMO_APP::PrepareDirShadowPass()
{
	ResetPipelineShaders();

	m_RenderController.SetBlendState(BlendModeEnum::NO_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_STENCIL_DEFAULT);

	m_CoreObjects.SetMainRenderTarget(m_ShadowRenderTarget.GetRenderTargetView());
	m_CoreObjects.SetDepthStencilTarget(&m_ShadowDepthStencil);

	ID3D11ShaderResourceView* tmp = NULL;
	m_CoreObjects.GetContext()->PSSetShaderResources(VIEW_SHADOW_RENDER_TARGET_REGISTER, 1, &tmp);
	m_CoreObjects.CommitRenderTargetAndDepthStencil();
	m_CoreObjects.Clear(CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER, D3D11_CLEAR_DEPTH, 1.0f,
							0, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	m_CoreObjects.SetViewports(&m_ViewShadowViewport);

	m_DirShadowVertex.SetVertexShader();
	m_CoreObjects.GetContext()->GSSetShader(NULL, NULL, 0);
	m_DirShadowPixel.SetPixelShader();
}

void DEMO_APP::RenderShadowPass(ILight* pLight, bool bRenderCubeFaces)
{
	Matrix4f lightViewMatrix;
	Matrix4f lightViewProjMatrix;

	if(!bRenderCubeFaces)
	{
		lightViewMatrix = pLight->GetViewMatrix();
		lightViewProjMatrix = pLight->GetViewProjectionMatrix();

		for(std::vector<GameObject*>::iterator iter = m_vRenderScene.begin(); iter != m_vRenderScene.end(); ++iter)
		{
			const Matrix4f& objMat = (*iter)->GetMatrix();
			m_Camera.SetShaderObjectBuffer(objMat, lightViewMatrix, lightViewProjMatrix);
			(*iter)->RenderIndexedNoTex(&m_CoreObjects);
			//(*iter)->RenderShadowPass(pLight, &m_CoreObjects, &m_Camera);
		}
	}
	else
	{
		for(unsigned int i = 0; i < 6; ++i)
		{
			lightViewMatrix = pLight->GetViewMatrix(i);
			lightViewProjMatrix = pLight->GetViewProjectionMatrix(i);

			m_CoreObjects.SetMainRenderTarget(m_PointLightRenderTarget.GetFaceRenderTarget((D3D11_TEXTURECUBE_FACE)i).GetRenderTargetView());
		
			m_CoreObjects.CommitRenderTargetAndDepthStencil();
			m_CoreObjects.Clear(CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER, D3D11_CLEAR_DEPTH, 1.0f,
				0, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

			for(std::vector<GameObject*>::iterator iter = m_vRenderScene.begin(); iter != m_vRenderScene.end(); ++iter)
			{
				const Matrix4f& objMat = (*iter)->GetMatrix();
				m_Camera.SetShaderObjectBuffer(objMat, lightViewMatrix, lightViewProjMatrix);
				(*iter)->RenderIndexedNoTex(&m_CoreObjects);
				//(*iter)->RenderShadowPass(pLight, &m_CoreObjects, &m_Camera);
			}
		}
	}
}

void DEMO_APP::PrepareDirLightPass()
{
	ResetPipelineShaders();

	RevertRenderTargetAndDepthStencil();

	m_RenderController.SetBlendState(BlendModeEnum::ADDITIVE_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_LESSEQUAL_NO_STENCIL);

	ID3D11ShaderResourceView* shadowRenderTarget = m_ShadowRenderTarget.GetShaderResourceView();
	m_CoreObjects.GetContext()->PSSetShaderResources(VIEW_SHADOW_RENDER_TARGET_REGISTER, 1, &shadowRenderTarget);

	m_DirLightVertex.SetVertexShader();
	m_DirectionalPixel.SetPixelShader();
}

void DEMO_APP::PrepareSpotShadowPass()
{
	ResetPipelineShaders();

	m_RenderController.SetBlendState(BlendModeEnum::NO_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_STENCIL_DEFAULT);

	m_CoreObjects.SetMainRenderTarget(m_ShadowRenderTarget.GetRenderTargetView());
	m_CoreObjects.SetDepthStencilTarget(&m_ShadowDepthStencil);

	ID3D11ShaderResourceView* tmp = NULL;
	m_CoreObjects.GetContext()->PSSetShaderResources(VIEW_SHADOW_RENDER_TARGET_REGISTER, 1, &tmp);
	m_CoreObjects.CommitRenderTargetAndDepthStencil();
	m_CoreObjects.Clear(CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER, D3D11_CLEAR_DEPTH, 1.0f,
							0, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	m_CoreObjects.SetViewports(&m_ViewShadowViewport);

	m_SpotShadowVertex.SetVertexShader();
	m_SpotShadowPixel.SetPixelShader();
}

void DEMO_APP::PrepareSpotLightPass()
{
	ResetPipelineShaders();

	RevertRenderTargetAndDepthStencil();

	m_RenderController.SetBlendState(BlendModeEnum::ADDITIVE_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_LESSEQUAL_NO_STENCIL);

	ID3D11ShaderResourceView* shadowRenderTarget = m_ShadowRenderTarget.GetShaderResourceView();
	m_CoreObjects.GetContext()->PSSetShaderResources(VIEW_SHADOW_RENDER_TARGET_REGISTER, 1, &shadowRenderTarget);

	m_SpotLightVertex.SetVertexShader();
	m_SpotLightPixel.SetPixelShader();
}

void DEMO_APP::PreparePointShadowPass()
{
	ResetPipelineShaders();

	m_RenderController.SetBlendState(BlendModeEnum::NO_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_STENCIL_DEFAULT);

	ID3D11ShaderResourceView* tmp = NULL;
	m_CoreObjects.GetContext()->PSSetShaderResources(POINT_SHADOW_RENDER_TARGET_REGISTER, 1, &tmp);
	m_CoreObjects.GetContext()->PSSetShaderResources(VIEW_SHADOW_RENDER_TARGET_REGISTER, 1, &tmp);

	m_PointShadowVertex.SetVertexShader();
	m_PointShadowPixel.SetPixelShader();

	if(!g_RenderPtCubeFaces)
	{
		m_CoreObjects.SetMainRenderTarget(m_PointLightRenderTarget.GetRenderTargetView());
		m_CoreObjects.SetDepthStencilTarget(&m_PointLightDepthStencilCube);
		
		m_CoreObjects.CommitRenderTargetAndDepthStencil();
		m_CoreObjects.Clear(CLEAR_RENDER_TARGETS | CLEAR_DS_BUFFER, D3D11_CLEAR_DEPTH, 1.0f,
			0, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

		m_PointShadowGeometry.SetGeometryShader();
	}
	else
	{
		m_CoreObjects.SetDepthStencilTarget(&m_PointDepthStencil);

		m_CoreObjects.GetContext()->GSSetShader(NULL, NULL, 0);
	}

	D3D11_VIEWPORT ptLightViewPort;
	ptLightViewPort.Width = POINT_LIGHT_SHADOW_RESOLUTION;
	ptLightViewPort.Height = POINT_LIGHT_SHADOW_RESOLUTION;
	ptLightViewPort.MinDepth = 0.0f;
	ptLightViewPort.MaxDepth = 1.0f;
	ptLightViewPort.TopLeftX = 0.0f;
	ptLightViewPort.TopLeftY = 0.0f;
	m_CoreObjects.SetViewports(&ptLightViewPort);
}

void DEMO_APP::PreparePointLightPass()
{
	ResetPipelineShaders();

	RevertRenderTargetAndDepthStencil();

	m_RenderController.SetBlendState(BlendModeEnum::ADDITIVE_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_LESSEQUAL_NO_STENCIL);

	ID3D11ShaderResourceView* shadowRenderTarget = m_PointLightRenderTarget.GetShaderResourceView();
	m_CoreObjects.GetContext()->PSSetShaderResources(POINT_SHADOW_RENDER_TARGET_REGISTER, 1, &shadowRenderTarget);

	m_PointLightVertex.SetVertexShader();
	//m_PointLightGeometry.SetGeometryShader();
	m_PointLightPixel.SetPixelShader();
}

void DEMO_APP::RevertRenderTargetAndDepthStencil()
{
	m_CoreObjects.SetMainRenderTarget(m_PostProcessTarget1.GetRenderTargetView());
	m_CoreObjects.SetDepthStencilTarget(nullptr);
	m_CoreObjects.CommitRenderTargetAndDepthStencil();
	m_CoreObjects.SetViewports(NULL);
}

void DEMO_APP::RenderSkyDome()
{
	ResetPipelineShaders();

	//m_CoreObjects.SetNextRenderTarget(m_rtGodRaysTarget.GetRenderTargetView());
	//m_CoreObjects.CommitRenderTargetAndDepthStencil();

	m_RenderController.SetBlendState(BlendModeEnum::NO_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_READ_NO_WRITE);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_NONE);
	m_CoreObjects.CommitRasterizerChanges();

	m_SkyDomeVertex.SetVertexShader();
	m_SkyDomePixel.SetPixelShader();

	m_pSkyDome->RenderIndexed(&m_CoreObjects);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_BACK);
	m_CoreObjects.CommitRasterizerChanges();

	RevertRenderTargetAndDepthStencil();
}

void DEMO_APP::RenderSun()
{
	ResetPipelineShaders();

	m_CoreObjects.SetNextRenderTarget(m_rtGodRaysTarget.GetRenderTargetView());
	m_CoreObjects.CommitRenderTargetAndDepthStencil();

	m_RenderController.SetBlendState(ALPHA_BLEND_MODE);
	m_RenderController.SetDepthStencilState(DepthStencilStateEnum::DEPTH_READ_NO_WRITE);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_NONE);
	m_CoreObjects.CommitRasterizerChanges();

	ID3D11InputLayout* pCurrInputLayout;
	m_CoreObjects.GetContext()->IAGetInputLayout(&pCurrInputLayout);

	ID3D11InputLayout* pNULLLayout = NULL;
	m_CoreObjects.GetContext()->IASetInputLayout(pNULLLayout);
	

	D3D11_PRIMITIVE_TOPOLOGY currTopology;
	m_CoreObjects.GetContext()->IAGetPrimitiveTopology(&currTopology);
	m_CoreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_SunVertex.SetVertexShader();
	m_SunGeometry.SetGeometryShader();
	m_SunPixel.SetPixelShader();

	m_CoreObjects.GetContext()->PSSetShaderResources(0, 1, &m_pSunDiffuseTexture.p);
	m_CoreObjects.GetContext()->Draw(1, 0);

	m_CoreObjects.GetContext()->IASetPrimitiveTopology(currTopology);
	m_CoreObjects.GetContext()->IASetInputLayout(pCurrInputLayout);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_BACK);
	m_CoreObjects.CommitRasterizerChanges();

	RevertRenderTargetAndDepthStencil();
}

bool DEMO_APP::Run()
{
	Update();

	bool bDirty = m_Camera.Dirty();

	m_Camera.UpdateMatrices();

	if(bDirty)
	{
		m_dirLight.UpdateViewProjMatrix(&m_Camera);
		m_dirLight.UpdateConstantBuffer(m_CoreObjects.GetContext());
	}

	RevertRenderTargetAndDepthStencil();
	//m_CoreObjects.Clear();
	
	// Set Vertex and Index Buffers
	m_VertexBuffer.PrepareVertexBuffer();
	m_IndexBuffer.PrepareIndexBuffer();

	// Set the Input Layout
	m_CoreObjects.GetContext()->IASetInputLayout(m_pInputLayout.p);

	PrepareAmbientPass();
	RenderLightPass();

	/*PrepareDirShadowPass();
	RenderShadowPass(&m_dirLight);

	PrepareDirLightPass();
	RenderLightPass();

	PrepareSpotShadowPass();
	RenderShadowPass(&m_SpotLight);

	PrepareSpotLightPass();
	RenderLightPass();

	PreparePointShadowPass();
	RenderShadowPass(&m_ptLight, g_RenderPtCubeFaces);

	PreparePointLightPass();
	RenderLightPass();*/

	RenderSkyDome();
	RenderSun();



	m_RenderController.SetBlendState(NO_BLEND_MODE);
	RenderScreenQuad(&m_PassThroughScreenVertex, &m_PassThroughPixel, m_PostProcessTarget1.GetShaderResourceView());

	ExtendGodRays();

	/*if(g_bBlur)
	{
		RenderGaussianBlur();
		RenderScreenQuad(&m_PassThroughScreenVertex, &m_PassThroughPixel, m_PostProcessTarget2.GetShaderResourceView());
	}
	else
	{
		RenderScreenQuad(&m_PassThroughScreenVertex, &m_PassThroughPixel, m_PostProcessTarget1.GetShaderResourceView());
	}*/

	m_CoreObjects.GetContext()->VSSetShader(NULL, NULL, 0);
	m_CoreObjects.GetContext()->GSSetShader(NULL, NULL, 0);
	m_CoreObjects.GetContext()->PSSetShader(NULL, NULL, 0);

	// Present To Screen
	m_CoreObjects.Present(PRESENT_VSYNC);
	
	return true; 
}

void DEMO_APP::RenderGaussianBlur()
{
	return RenderScreenQuad(&g_HorizontalBlurVertex, &g_HorizontalBlurPixel, m_PostProcessTarget1.GetShaderResourceView(), &m_PostProcessTarget2);

	m_CoreObjects.SetRenderTargets(NULL);
	m_CoreObjects.CommitRenderTargetAndDepthStencil();

	ID3D11ShaderResourceView* pSRVs[] = {m_PostProcessTarget1.GetShaderResourceView()};
	m_CoreObjects.GetContext()->CSSetShaderResources(0, sizeof(pSRVs) / sizeof(*pSRVs), pSRVs);

	ID3D11UnorderedAccessView* pUAVs[] = {m_PostProcessUAV2.GetUAV()};
	m_CoreObjects.GetContext()->CSSetUnorderedAccessViews(0, sizeof(pUAVs) / sizeof(*pUAVs), pUAVs, NULL);

	m_GaussianBlurCompute.SetComputeShader();
	m_GaussianBlurCompute.Dispatch();

	ID3D11ShaderResourceView* pNULLSRVs[] = {NULL};
	m_CoreObjects.GetContext()->CSSetShaderResources(0, sizeof(pNULLSRVs) / sizeof(*pNULLSRVs), pNULLSRVs);

	ID3D11UnorderedAccessView* pNULLUAVs[] = {NULL};
	m_CoreObjects.GetContext()->CSSetUnorderedAccessViews(0, sizeof(pNULLUAVs) / sizeof(*pNULLUAVs), pNULLUAVs, NULL);
}

void DEMO_APP::ExtendGodRays()
{
	// first determine if the sun is in front of the camera
	if(m_Camera.GetWorldMatrix().zAxis.dot_product(m_dirLight.GetDirection()) < 0.0f)
	{
		Vec4f sunPos(m_Camera.GetWorldMatrix().position - m_dirLight.GetDirection(), 1.0f);

		//XMVECTOR screenPos = /*XMVector3Unproject(XMLoadFloat4((XMFLOAT4*)sunPos.vector), 0, 0, 1280, 720, 0, 1.0f, XMLoadFloat4x4((XMFLOAT4X4*)m_Camera.GetProjectionMatrix().m), XMLoadFloat4x4((XMFLOAT4X4*)camera.GetViewMatrix().m), XMMatrixIdentity());*/XMVector3Project(XMLoadFloat4((XMFLOAT4*)sunPos.vector), 0, 0, 1280, 720, 0, 1.0f, XMLoadFloat4x4((XMFLOAT4X4*)camera.GetProjectionMatrix().m), XMLoadFloat4x4((XMFLOAT4X4*)camera.GetViewMatrix().m), XMMatrixIdentity());

		sunPos *= m_Camera.GetViewProjectionMatrix();
		sunPos /= sunPos.w;

		if (abs(sunPos.x) <= 1.25f && abs(sunPos.y) <= 1.25f)
		{
			sunPos.x = sunPos.x * 0.5f + 0.5f;
			sunPos.y = 1 - (sunPos.y * 0.5f + 0.5f);

			m_BlurFactors.gFocusPoint = Vec2f(sunPos.x, sunPos.y);

			UpdateConstBuff(m_pBlurFactorConstBuffer.p, &m_BlurFactors, sizeof(m_BlurFactors));

			m_CoreObjects.GetContext()->PSSetConstantBuffers(BLUR_CONST_BUFF_REGISTER, 1, &m_pBlurFactorConstBuffer.p);

			m_CoreObjects.SetRenderTargets(NULL);

			m_RenderController.SetBlendState(BlendModeEnum::ADDITIVE_BLEND_MODE);

			RenderScreenQuad(&m_PassThroughScreenVertex, &m_RadialBlurPixel, m_rtGodRaysTarget.GetShaderResourceView());
		}
	}
}

void DEMO_APP::RenderScreenQuad(VertexShader* pVertexShader, PixelShader* pPixelShader, ID3D11ShaderResourceView* pTexture, RenderTarget* pRenderTarget)
{
	ResetPipelineShaders();

	m_CoreObjects.SetMainRenderTarget((pRenderTarget) ? pRenderTarget->GetRenderTargetView() : NULL);
	m_CoreObjects.CommitRenderTargetAndDepthStencil();

	m_RenderController.SetDepthStencilState(DEPTH_NONE_STENCIL_NONE);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_NONE);
	m_CoreObjects.CommitRasterizerChanges();


	pVertexShader->SetVertexShader();
	pPixelShader->SetPixelShader();

	m_CoreObjects.GetContext()->PSSetShaderResources(0, 1, &pTexture);

	m_pScreenQuad->RenderIndexedNoTex(&m_CoreObjects);

	ID3D11ShaderResourceView* pNULLSRVs[] = {NULL};
	m_CoreObjects.GetContext()->PSSetShaderResources(0, sizeof(pNULLSRVs) / sizeof(*pNULLSRVs), pNULLSRVs);

	m_CoreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_BACK);
	m_CoreObjects.CommitRasterizerChanges();

	m_RenderController.SetDepthStencilState(DEPTH_STENCIL_DEFAULT);
}

void DEMO_APP::Update()
{
	m_Timer.Update();
	double fElapsedTime = m_Timer.GetDeltaTimeD();

	if(GetAsyncKeyState('W'))
		m_Camera.MoveForward(fElapsedTime);
	else if(GetAsyncKeyState('S'))
		m_Camera.MoveBackward(fElapsedTime);

	if(GetAsyncKeyState('A'))
		m_Camera.MoveLeft(fElapsedTime);
	else if(GetAsyncKeyState('D'))
		m_Camera.MoveRight(fElapsedTime);

	if(GetAsyncKeyState('E'))
		m_Camera.MoveUp(fElapsedTime);
	else if(GetAsyncKeyState('Q'))
		m_Camera.MoveDown(fElapsedTime);

	// removes camera choppiness as opposed to using the window proc.
	if(m_bRButtonDown)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(window, &mousePos);

		OnMouseMove(window, LPARAM(mousePos.x | (mousePos.y << 16)));
	}

	m_pSphere->GetMatrix().Rotate_LocalY_Radians(PI * float(fElapsedTime * 0.5f));

	Vec4f ptLightPos(m_ptLight.GetPosition(), 1.0f);
	Matrix4f rotationMatrix;
	rotationMatrix.Rotate_GlobalY_Radians(float(PI_OVER_2 * fElapsedTime));
	ptLightPos *= rotationMatrix;
	m_ptLight.SetPosition(ptLightPos.position);
	m_ptLight.UpdateViewProjMatrix(NULL);
	m_ptLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	Vec4f spotLightPos(m_SpotLight.GetPosition(), 1.0f);
	rotationMatrix.make_identity();
	rotationMatrix.Rotate_LocalY_Radians(float(-PI_OVER_2 * fElapsedTime));
	spotLightPos *= rotationMatrix;
	m_SpotLight.SetPosition(spotLightPos.position);
	m_SpotLight.SetLookAtPos(Vec3f());
	m_SpotLight.BuildViewAndProjectionMatrices();
	m_SpotLight.UpdateConstantBuffer(m_CoreObjects.GetContext());
}

void DEMO_APP::UpdateConstBuff(ID3D11Resource* pResource, void* pData, size_t unCopySize)
{
	D3D11_MAPPED_SUBRESOURCE subresourceData;
	m_CoreObjects.GetContext()->Map(pResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresourceData);
		memcpy(subresourceData.pData, pData, unCopySize);
	m_CoreObjects.GetContext()->Unmap(pResource, 0);
}

void DEMO_APP::OnKeyPress(WPARAM wParam)
{
	switch(wParam)
	{
	case 'P':
		{
			g_RenderPtCubeFaces = !g_RenderPtCubeFaces;
		}
		break;

	case 'V':
		{
			if(g_bRenderWireframe)
				m_CoreObjects.ChangeRasterizerState(RS_FILL_MODE, D3D11_FILL_SOLID);
			else
				m_CoreObjects.ChangeRasterizerState(RS_FILL_MODE, D3D11_FILL_WIREFRAME);

			g_bRenderWireframe = !g_bRenderWireframe;
			m_CoreObjects.CommitRasterizerChanges();
		}
		break;

	case 'B':
		{
			g_bBlur = !g_bBlur;
		}
		break;

	case 'T':
		{
			//g_bTessellate = !g_bTessellate;

			unsigned int unNewTessType = 0;

			switch(m_GameShaderBuffer.GetTessellationType())
			{
			case RF_DISTTESS:
				{
					unNewTessType = RF_SSTESS;
				}
				break;

			case 0: // no tess
				{
					unNewTessType = RF_DISTTESS;
				}
				break;
			}

			m_GameShaderBuffer.SetTessellationType((RenderFlagEnum)unNewTessType);
			UpdateConstBuff(m_pGameConstBuffer.p, &m_GameShaderBuffer, sizeof(m_GameShaderBuffer));
		}
		break;

	case 0x31: // 1 key
		{
			m_GameShaderBuffer.ToggleRenderFlag(RF_SOLIDWIREFRAME);
			UpdateConstBuff(m_pGameConstBuffer.p, &m_GameShaderBuffer, sizeof(m_GameShaderBuffer));
		}
		break;

	case 0x32: // 2 key
		{
			m_GameShaderBuffer.gWireFrameWidth = max(m_GameShaderBuffer.gWireFrameWidth - 5.0f * m_Timer.GetDeltaTimeF(), 0.0f);
			UpdateConstBuff(m_pGameConstBuffer.p, &m_GameShaderBuffer, sizeof(m_GameShaderBuffer));
		}
		break;

	case 0x33: // 3 key
		{
			m_GameShaderBuffer.gWireFrameWidth += 5.0f * m_Timer.GetDeltaTimeF();
			UpdateConstBuff(m_pGameConstBuffer.p, &m_GameShaderBuffer, sizeof(m_GameShaderBuffer));
		}
		break;

	case VK_OEM_PLUS:
		{
						m_TessShaderBuffer.SetTessellationDistRange(m_TessShaderBuffer.m_fTessMinDist, min(m_TessShaderBuffer.m_fTessMaxDist + 10.0f * m_Timer.GetDeltaTimeF(), m_Camera.GetFarClip()));
			UpdateConstBuff(m_pTessellationConstBuffer.p, &m_TessShaderBuffer, sizeof(m_TessShaderBuffer));
		}
		break;

	case VK_OEM_MINUS:
		{
						 m_TessShaderBuffer.SetTessellationDistRange(m_TessShaderBuffer.m_fTessMinDist, max(m_TessShaderBuffer.m_fTessMaxDist - 10.0f * m_Timer.GetDeltaTimeF(), m_Camera.GetNearClip()));
			UpdateConstBuff(m_pTessellationConstBuffer.p, &m_TessShaderBuffer, sizeof(m_TessShaderBuffer));
		}
		break;

	case VK_OEM_4: // LBracket
		{
			m_TessShaderBuffer.SetTessFactors(m_TessShaderBuffer.m_fMinTessFactor, max(m_TessShaderBuffer.m_fMaxTessFactor - 15.0f * m_Timer.GetDeltaTimeF(), m_TessShaderBuffer.m_fMinTessFactor));
			UpdateConstBuff(m_pTessellationConstBuffer.p, &m_TessShaderBuffer, sizeof(m_TessShaderBuffer));
		}
		break;

	case VK_OEM_6: // RBracket
		{
			m_TessShaderBuffer.SetTessFactors(m_TessShaderBuffer.m_fMinTessFactor, min(m_TessShaderBuffer.m_fMaxTessFactor + 15.0f * m_Timer.GetDeltaTimeF(), 64));
			UpdateConstBuff(m_pTessellationConstBuffer.p, &m_TessShaderBuffer, sizeof(m_TessShaderBuffer));
		}
		break;

	default:
		return;
	}
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{	
	SpotLight::ReleaseConstantBufferAndClassLinkage();
	DirectionalLight::ReleaseConstantBufferAndClassLinkage();
	PointLight::ReleaseConstantBufferAndClassLinkage();
	AmbientLight::ReleaseConstantBufferAndClassLinkage();
	ILight::ReleaseClassLinkage();

	for(unsigned int i = 0; i < m_vRenderScene.size(); ++i)
	{
		if(m_vRenderScene[i])
		{
			delete m_vRenderScene[i];
		}
	}

	SAFE_DELETE(m_pSkyDome);
	SAFE_DELETE(m_pScreenQuad);

	UnregisterClass( L"DirectXApplication", application ); 
	DebugWindow::Shutdown();
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!
					   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);

	srand(unsigned int(time(0)));
	myApp.Initialize(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run() )
    {	
	    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	myApp.ShutDown(); 
	return 0; 
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;

		case WM_KEYDOWN:
			{
				myApp.OnKeyPress(wParam);
			}
			break;

		case WM_RBUTTONDOWN:
			{
				m_bRButtonDown = true;
				
				lockedMousePos.x = LOWORD(lParam);
				lockedMousePos.y = HIWORD(lParam);

				myApp.SetCurrentMousePos(lockedMousePos);
			}
			break;

		case WM_RBUTTONUP:
			{
				m_bRButtonDown = false;
			}
			break;

		// Causes camera movement to be very choppy when
		// moving while turning. Hmmmm....
		/*case WM_MOUSEMOVE:
			{
				myApp.OnMouseMove(hWnd, lParam);
			}
			break;*/
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//