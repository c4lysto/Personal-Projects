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

#include <iostream>
#include <time.h>
#include "XTime.h"
#include "DirectXCore.h"
#include "GameObject.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "RenderController.h"
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

	struct ParticleData
	{
		Vec4f color;
		Vec3f position;
		Vec3f velocity;
	};

	struct ParticleVertex
	{
		Vec4f color;
		Vec3f position;
	};

private:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	XTime							timer;
	DirectXCore						coreObjects;
	RenderController				renderController;
	GameShaderBuffer				m_GameConstBuffer;
	
	CComPtr<ID3D11Buffer> pGameConstBuffer;
	CComPtr<ID3D11Buffer> pStructuredBuffer;
	CComPtr<ID3D11BlendState> pBlendState;

	CComPtr<ID3D11InputLayout> pInputLayout;

	CComPtr<ID3D11VertexShader> pVertexShader;
	CComPtr<ID3D11PixelShader> pPixelShader;

	CComPtr<ID3D11ShaderResourceView> pTextureView;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera camera;

	void Update();

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

DEMO_APP myApp;

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
	//********************* END WARNING ************************//

	coreObjects.Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, true);
	renderController.Initialize(&coreObjects);
	camera.Initialize(&coreObjects, DEG_TO_RAD(42.0f), 0.1f, 200.0f);

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

	// TODO: PART 2 STEP 8a
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTCLR_LAYOUT;
	
	// TODO: PART 2 STEP 8b
	// NOTE: ask about 4th parameter

	CreateDDSTextureFromFile(coreObjects.GetDevice(), _T("particle.dds"), NULL, &pTextureView.p);

	ZeroMemory(&m_GameConstBuffer, sizeof(GameShaderBuffer));

	timer.Restart();
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	Update();
	camera.UpdateMatrices();

	coreObjects.Clear();
	
	// TODO: PART 2 STEP 9a
	//UINT strides = 0;//[] = {VERTCLR_SIZE};
	//UINT offsets = 0;//[] = {0};
	//ID3D11Buffer* pNUllBuffer = NULL; 
	//coreObjects.GetContext()->IASetVertexBuffers(0, 1, &pNUllBuffer, &strides, &offsets);
	
	// TODO: PART 2 STEP 9b
	coreObjects.GetContext()->VSSetShader(pVertexShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(pPixelShader.p, 0, 0);

	
	// TODO: PART 2 STEP 9d
	coreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// NOTE: parameter 1 is the register number in the shader
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &pTextureView.p);
	
	camera.SetMVPAndWorldMatrices(Matrix4f());

	// TODO: PART 2 STEP 10
	coreObjects.GetContext()->Draw(0, 0);

	ID3D11ShaderResourceView *pNULLsrv = NULL;
	coreObjects.GetContext()->VSSetShaderResources(0, 1, &pNULLsrv);
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &pNULLsrv);

	// TODO: PART 1 STEP 8
	coreObjects.Present(PRESENT_VSYNC);
	
	// END OF PART 1
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
	// TODO: PART 1 STEP 6
	
	UnregisterClass( L"DirectXApplication", application ); 
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

		/*case WM_RBUTTONDOWN:
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
			break;*/
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//