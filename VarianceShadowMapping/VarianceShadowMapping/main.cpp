// D3D LAB 1a "Line Land".
// Author: L.Norri CD DRX, FullSail University

// Introduction:
// Welcome to the first lab of the Direct3D Graphics Programming class.
// This is the ONLY guided lab in this course! 
// Future labs will demand the habits & foundation you develop right now!  
// It is CRITICAL that you follow each and every step. ESPECIALLY THE READING!!!

// TO BEGIN: Open the word document that acompanies this lab and start from the top.

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
#include "PixelShaderDynamic.h"
#include "VertexShader.h"
using namespace VertexDecl;

#include "DDSTextureLoader.h"

using namespace std;

// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// TODO: PART 1 STEP 1b
#include <DirectXMath.h>
using namespace DirectX;

#ifdef _DEBUG
#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	720
#else
#define BACKBUFFER_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define BACKBUFFER_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#endif

#define GAME_CONST_BUFF_REGISTER 0

#include "VertNormTanUVTransformedVertex.csh"
#include "VertNormTanUVPixel.csh"

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
private:
	struct GameShaderBuffer
	{
		float fElapsedTime;
		Vec3f gravityPos; // find something to fill this with if you can
		Vec3f CameraToGravity;
		DWORD GameTime;
	};

private:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	XTime							timer;
	DirectXCore						coreObjects;
	RenderController				renderController;
	GameShaderBuffer				m_GameConstBuffer;

	GameObject						m_Box;
	GameObject						m_Ground;
	
	CComPtr<ID3D11Buffer> pGameConstBuffer;
	CComPtr<ID3D11Buffer> pStructuredBuffer;
	CComPtr<ID3D11BlendState> pBlendState;

	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

	CComPtr<ID3D11InputLayout> m_pInputLayout;

	VertexShader m_VertexShader;
	PixelShaderDynamic m_LightingShader;

	PointLight m_ptLight;
	DirectionalLight m_dirLight;
	SpotLight m_SpotLight;

	CComPtr<ID3D11ShaderResourceView> m_pCrateTexture;
	CComPtr<ID3D11ShaderResourceView> m_pNormalTexture;

	CComPtr<ID3D11ShaderResourceView> m_pGroundTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundNormalTexture;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera camera;

	void Update();

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateShaders();
	void CreateInputLayout();
	void InitLights();

	void CreateDiffuseTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateNormalTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	void OnMouseMove(HWND hWnd, LPARAM lParam);
};

DEMO_APP myApp;
bool m_bRButtonDown = false;
POINT lockedMousePos;

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

void DEMO_APP::Initialize(HINSTANCE hinst, WNDPROC proc)
{
	// ****************** BEGIN WARNING ***********************// 
	// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY! 
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
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(	L"DirectXApplication", L"Lab 1a Line Land",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );

	DebugWindow::Initialize();
	//********************* END WARNING ************************//

	coreObjects.Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, true);
	renderController.Initialize(&coreObjects);
	camera.Initialize(&coreObjects, DEG_TO_RAD(42.0f), 0.1f, 200.0f);

	ILight::CreateClassLinkage(&coreObjects);
	PointLight::CreateConstantBufferAndClassLinkage(coreObjects.GetDevice(), "gPointLight");
	DirectionalLight::CreateConstantBufferAndClassLinkage(coreObjects.GetDevice(), "gDirLight");
	SpotLight::CreateConstantBufferAndClassLinkage(coreObjects.GetDevice(), "gSpotLight");

	D3D11_BUFFER_DESC shaderConstBuffDesc;
	ZeroMemory(&shaderConstBuffDesc, sizeof(D3D11_BUFFER_DESC));
	shaderConstBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderConstBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderConstBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderConstBuffDesc.ByteWidth = sizeof(GameShaderBuffer);

	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&shaderConstBuffDesc, 0, &pGameConstBuffer.p)))
		MessageBox(NULL, L"Failed to create Game Constant Buffer" , L"", MB_OK | MB_ICONERROR);

	coreObjects.GetContext()->CSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &pGameConstBuffer.p);
	coreObjects.GetContext()->CSSetConstantBuffers(CAMERA_CONST_BUFF_REGISTER, 1, &camera.GetCamConstBuff().p);

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateShaders();
	CreateInputLayout();
	InitLights();

	m_Box.GetModel()->LoadModel("Models/Box.smsh", &coreObjects, &m_VertexBuffer, &m_IndexBuffer);

	m_Ground.GetModel()->LoadModel("Models/Quad5.smsh", &coreObjects, &m_VertexBuffer, &m_IndexBuffer);
	m_Ground.GetMatrix().SetScale(20.0f);

	// Setup Box Textures
	CreateDiffuseTexture(L"Textures/crate.dds", m_Box.GetModel()->GetMesh(), &m_pCrateTexture.p);
	CreateNormalTexture(L"Textures/NormalMap.dds", m_Box.GetModel()->GetMesh(), &m_pNormalTexture.p);

	// Setup Ground Textures
	CreateDiffuseTexture(L"Textures/Aphalt_Diffuse.dds", m_Ground.GetModel()->GetMesh(), &m_pGroundTexture.p);
	CreateNormalTexture(L"Textures/Asphalt_Normal.dds", m_Ground.GetModel()->GetMesh(), &m_pGroundNormalTexture.p);

	ZeroMemory(&m_GameConstBuffer, sizeof(GameShaderBuffer));

	timer.Restart();
}

void DEMO_APP::CreateVertexBuffer()
{
	m_VertexBuffer.Initialize(&coreObjects, VERTNORMTANUV_SIZE);
}

void DEMO_APP::CreateIndexBuffer()
{
	m_IndexBuffer.Initialize(&coreObjects);
}

void DEMO_APP::CreateShaders()
{
	m_VertexShader.CreateVertexShader(coreObjects.GetContext(), coreObjects.GetDevice(), VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex));

	m_LightingShader.CreatePixelShader(coreObjects.GetContext(), coreObjects.GetDevice(), VertNormTanUVPixel, ARRAYSIZE(VertNormTanUVPixel));
	m_LightingShader.SetClassInstance("gSpotLight", m_LightingShader.AddInterfaceByName("gLight"));
}

void DEMO_APP::InitLights()
{
	m_ptLight.SetPosition(0.0f, 3.0f, 0.0f);
	m_ptLight.SetRadius(7.5f);
	m_ptLight.SetSpecularPower(10.0f);
	m_ptLight.SetConstantBufferPS(coreObjects.GetContext());
	m_ptLight.UpdateConstantBuffer(coreObjects.GetContext());

	m_SpotLight.SetPosition(10.0f, 5.0f, 0.0f);
	m_SpotLight.SetDirection(-1.0f, -1.0f, 0.0f);
	m_SpotLight.SetSpecularPower(10.0f);
	m_SpotLight.SetInnerConeRatio(0.95f);
	m_SpotLight.SetOuterConeRatio(0.9f);
	m_SpotLight.SetRadius(15.0f);
	m_SpotLight.SetConstantBufferPS(coreObjects.GetContext());
	m_SpotLight.UpdateConstantBuffer(coreObjects.GetContext());

	m_dirLight.SetDirection(1.0f, -1.0f, 1.0f);
	m_dirLight.SetSpecularPower(10.0f);
	m_dirLight.SetConstantBufferPS(coreObjects.GetContext());
	m_dirLight.UpdateConstantBuffer(coreObjects.GetContext());
}

void DEMO_APP::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTNORMTANUV_LAYOUT;

	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTNORMTANUV_ELEMENTS, VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex), &m_pInputLayout.p)))
		MessageBox(window, L"Failed To Create Input Layout", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateDiffuseTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(coreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_DIFFUSE);
}

void DEMO_APP::CreateNormalTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(coreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_NORMAL);
}

void DEMO_APP::OnMouseMove(HWND hWnd, LPARAM lParam)
{
	POINT currMousePos = {LOWORD(lParam), HIWORD(lParam)};

	if(m_bRButtonDown)
	{
		POINT mouseMovement = {m_PrevMousePos.x - currMousePos.x, m_PrevMousePos.y - currMousePos.y};

		camera.RotateCameraMouseMovement(mouseMovement, timer.Delta());

		POINT screenPos = lockedMousePos;
		ClientToScreen(hWnd, &screenPos);
		SetCursorPos(screenPos.x, screenPos.y);

		m_PrevMousePos = lockedMousePos;

		return;
	}
		
	m_PrevMousePos = currMousePos;
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	Update();
	camera.UpdateMatrices();

	coreObjects.Clear();
	
	// Set Vertex and Index Buffers
	m_VertexBuffer.PrepareVertexBuffer();
	m_IndexBuffer.PrepareIndexBuffer();
	
	// Set Vertex and Pixel Shaders
	m_VertexShader.SetVertexShader();
	m_LightingShader.SetPixelShader();

	// Set the Input Layout
	coreObjects.GetContext()->IASetInputLayout(m_pInputLayout.p);

	m_Box.Render(&coreObjects, &camera);
	m_Ground.Render(&coreObjects, &camera);

	// Present To Screen
	coreObjects.Present(PRESENT_VSYNC);
	
	return true; 
}

void DEMO_APP::Update()
{
	timer.Signal();
	double fElapsedTime = timer.Delta();

	if(GetAsyncKeyState('W'))
		camera.MoveForward(fElapsedTime);
	else if(GetAsyncKeyState('S'))
		camera.MoveBackward(fElapsedTime);

	if(GetAsyncKeyState('A'))
		camera.MoveLeft(fElapsedTime);
	else if(GetAsyncKeyState('D'))
		camera.MoveRight(fElapsedTime);

	if(GetAsyncKeyState('E'))
		camera.MoveUp(fElapsedTime);
	else if(GetAsyncKeyState('Q'))
		camera.MoveDown(fElapsedTime);
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{	
	SpotLight::ReleaseConstantBufferAndClassLinkage();
	DirectionalLight::ReleaseConstantBufferAndClassLinkage();
	PointLight::ReleaseConstantBufferAndClassLinkage();
	ILight::ReleaseClassLinkage();

	UnregisterClass( L"DirectXApplication", application ); 
	DebugWindow::Shutdown();
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!
	
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
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

		case WM_RBUTTONDOWN:
			{
				m_bRButtonDown = true;
				
				lockedMousePos.x = LOWORD(lParam);
				lockedMousePos.y = HIWORD(lParam);
			}
			break;

		case WM_RBUTTONUP:
			{
				m_bRButtonDown = false;
			}
			break;

		case WM_MOUSEMOVE:
			{
				myApp.OnMouseMove(hWnd, lParam);
			}
			break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//