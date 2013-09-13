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
#include "AmbientLight.h"
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
	DirectXCore						m_CoreObjects;
	RenderController				m_RenderController;
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

	AmbientLight m_AmbientLight;
	PointLight m_ptLight;
	DirectionalLight m_dirLight;
	SpotLight m_SpotLight;

	CComPtr<ID3D11ShaderResourceView> m_pCrateTexture;
	CComPtr<ID3D11ShaderResourceView> m_pNormalTexture;

	CComPtr<ID3D11ShaderResourceView> m_pGroundTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundNormalTexture;
	CComPtr<ID3D11ShaderResourceView> m_pGroundSpecularTexture;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera camera;

	void Update();

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateShaders();
	void CreateInputLayout();
	void InitLights();

	void RenderMultipass(GameObject* pObject);

	void RenderAmbientPass(GameObject* pObject);
	void RenderDirectionalLightPass(GameObject* pObject);
	void RenderPointLightPass(GameObject* pObject);
	void RenderSpotLightPass(GameObject* pObject);

	void CreateDiffuseTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateNormalTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);
	void CreateSpecularTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV);

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	void OnMouseMove(HWND hWnd, LPARAM lParam);

	void SetCurrentMousePos(POINT mousePos) {m_PrevMousePos = mousePos;}
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

	m_CoreObjects.Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, true);
	m_RenderController.Initialize(&m_CoreObjects);
	camera.Initialize(&m_CoreObjects, DEG_TO_RAD(42.0f), 0.1f, 200.0f);

	ILight::CreateClassLinkage(&m_CoreObjects);
	AmbientLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gAmbientLight");
	PointLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gPointLight");
	DirectionalLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gDirLight");
	SpotLight::CreateConstantBufferAndClassLinkage(m_CoreObjects.GetDevice(), "gSpotLight");

	D3D11_BUFFER_DESC shaderConstBuffDesc;
	ZeroMemory(&shaderConstBuffDesc, sizeof(D3D11_BUFFER_DESC));
	shaderConstBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderConstBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderConstBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderConstBuffDesc.ByteWidth = sizeof(GameShaderBuffer);

	if(FAILED(m_CoreObjects.GetDevice()->CreateBuffer(&shaderConstBuffDesc, 0, &pGameConstBuffer.p)))
		MessageBox(NULL, L"Failed to create Game Constant Buffer" , L"", MB_OK | MB_ICONERROR);

	m_CoreObjects.GetContext()->CSSetConstantBuffers(GAME_CONST_BUFF_REGISTER, 1, &pGameConstBuffer.p);
	m_CoreObjects.GetContext()->CSSetConstantBuffers(CAMERA_CONST_BUFF_REGISTER, 1, &camera.GetCamConstBuff().p);

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateShaders();
	CreateInputLayout();
	InitLights();

	m_Box.GetModel()->LoadModel("Models/Box.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);

	m_Ground.GetModel()->LoadModel("Models/Quad5.smsh", &m_CoreObjects, &m_VertexBuffer, &m_IndexBuffer);
	m_Ground.GetMatrix().SetScale(20.0f);

	// Setup Box Textures
	CreateDiffuseTexture(L"Textures/crate.dds", m_Box.GetModel()->GetMesh(), &m_pCrateTexture.p);
	CreateNormalTexture(L"Textures/NormalMap.dds", m_Box.GetModel()->GetMesh(), &m_pNormalTexture.p);

	// Setup Ground Textures
	CreateDiffuseTexture(L"Textures/Ground_Diffuse.dds", m_Ground.GetModel()->GetMesh(), &m_pGroundTexture.p);
	CreateNormalTexture(L"Textures/Ground_Normal.dds", m_Ground.GetModel()->GetMesh(), &m_pGroundNormalTexture.p);
	CreateSpecularTexture(L"Textures/Ground_Specular.dds", m_Ground.GetModel()->GetMesh(), &m_pGroundSpecularTexture.p);

	ZeroMemory(&m_GameConstBuffer, sizeof(GameShaderBuffer));

	timer.Restart();
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
	m_VertexShader.CreateVertexShader(m_CoreObjects.GetContext(), m_CoreObjects.GetDevice(), VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex));

	m_LightingShader.CreatePixelShader(m_CoreObjects.GetContext(), m_CoreObjects.GetDevice(), VertNormTanUVPixel, ARRAYSIZE(VertNormTanUVPixel));
	m_LightingShader.SetClassInstance("gAmbientLight", m_LightingShader.AddInterfaceByName("gLight"));
}

void DEMO_APP::InitLights()
{
	m_AmbientLight.SetLightColor(0.1f, 0.1f, 0.1f);
	m_AmbientLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_AmbientLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_ptLight.SetLightColor(1.0f, 0.0f, 0.0f);
	m_ptLight.SetPosition(0.0f, 3.0f, 0.0f);
	m_ptLight.SetRadius(7.5f);
	m_ptLight.SetSpecularPower(10.0f);
	m_ptLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_ptLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_SpotLight.SetLightColor(0.0f, 1.0f, 0.0f);
	m_SpotLight.SetPosition(10.0f, 5.0f, 0.0f);
	m_SpotLight.SetDirection(-1.0f, -1.0f, 0.0f);
	m_SpotLight.SetSpecularPower(10.0f);
	m_SpotLight.SetInnerConeRatio(0.95f);
	m_SpotLight.SetOuterConeRatio(0.9f);
	m_SpotLight.SetRadius(15.0f);
	m_SpotLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_SpotLight.UpdateConstantBuffer(m_CoreObjects.GetContext());

	m_dirLight.SetLightColor(0.0f, 0.0f, 1.0f);
	m_dirLight.SetDirection(1.0f, -1.0f, 1.0f);
	m_dirLight.SetSpecularPower(10.0f);
	m_dirLight.SetConstantBufferPS(m_CoreObjects.GetContext());
	m_dirLight.UpdateConstantBuffer(m_CoreObjects.GetContext());
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

void DEMO_APP::CreateSpecularTexture(const wchar_t* szFilename, Mesh* pMesh, ID3D11ShaderResourceView** pSRV)
{
	CreateDDSTextureFromFile(m_CoreObjects.GetDevice(), szFilename, NULL, pSRV);

	pMesh->SetTexture(*pSRV, MESH_TEX_SPECULAR);
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

bool DEMO_APP::Run()
{
	Update();
	camera.UpdateMatrices();

	m_CoreObjects.Clear();
	
	// Set Vertex and Index Buffers
	m_VertexBuffer.PrepareVertexBuffer();
	m_IndexBuffer.PrepareIndexBuffer();
	
	// Set Vertex and Pixel Shaders
	m_VertexShader.SetVertexShader();
	m_LightingShader.SetPixelShader();

	// Set the Input Layout
	m_CoreObjects.GetContext()->IASetInputLayout(m_pInputLayout.p);

	RenderMultipass(&m_Box);
	RenderMultipass(&m_Ground);

	// Present To Screen
	m_CoreObjects.Present(PRESENT_VSYNC);
	
	return true; 
}

void DEMO_APP::RenderMultipass(GameObject* pObject)
{
	RenderAmbientPass(pObject);

	m_RenderController.SetBlendState(ADDITIVE_BLEND_MODE);
	RenderDirectionalLightPass(pObject);
	RenderPointLightPass(pObject);
	RenderSpotLightPass(pObject);
}

void DEMO_APP::RenderAmbientPass(GameObject* pObject)
{
	m_RenderController.SetBlendState(NO_BLEND_MODE);
	m_LightingShader.SetClassInstance("gAmbientLight", "gLight");
	pObject->Render(&m_CoreObjects, &camera);
}

void DEMO_APP::RenderDirectionalLightPass(GameObject* pObject)
{
	m_LightingShader.SetClassInstance("gDirLight", "gLight");
	pObject->Render(&m_CoreObjects, &camera);
}

void DEMO_APP::RenderPointLightPass(GameObject* pObject)
{
	m_LightingShader.SetClassInstance("gPointLight", "gLight");
	pObject->Render(&m_CoreObjects, &camera);
}

void DEMO_APP::RenderSpotLightPass(GameObject* pObject)
{
	m_LightingShader.SetClassInstance("gSpotLight", "gLight");
	pObject->Render(&m_CoreObjects, &camera);
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

	// removes camera choppiness as opposed to using the window proc.
	if(m_bRButtonDown)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(window, &mousePos);

		OnMouseMove(window, LPARAM(mousePos.x | (mousePos.y << 16)));
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