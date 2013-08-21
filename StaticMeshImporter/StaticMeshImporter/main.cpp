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

#include "VertNormTanUVTransformedVertex.csh"
#include "VertNormTanUVPixel.csh"

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

private:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	XTime							timer;
	DirectXCore						coreObjects;
	RenderController				renderController;
	GameShaderBuffer				m_GameConstBuffer;
	GameObject						m_Box;
	
	CComPtr<ID3D11Buffer> pGameConstBuffer;
	CComPtr<ID3D11Buffer> pStructuredBuffer;
	CComPtr<ID3D11BlendState> pBlendState;

	CComPtr<ID3D11Buffer> m_pVertexBuffer;
	CComPtr<ID3D11Buffer> m_pIndexBuffer;

	CComPtr<ID3D11InputLayout> m_pInputLayout;

	CComPtr<ID3D11VertexShader> m_pVertexShader;
	CComPtr<ID3D11PixelShader> m_pPixelShader;

	CComPtr<ID3D11ShaderResourceView> m_pDiffuseTexture;
	CComPtr<ID3D11ShaderResourceView> m_pNormalTexture;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera camera;

	void Update();

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateShaders();
	void CreateInputLayout();

	void CreateDiffuseTexture(const wchar_t* szFilename);
	void CreateNormalTexture(const wchar_t* szFilename);

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	void OnMouseMove(HWND hWnd, LPARAM lParam);
};

DEMO_APP myApp;
bool m_bRButtonDown = false;
bool m_bLButtonDown = false;
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

	m_Box.GetModel()->LoadModel("Box.smsh");

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateShaders();
	CreateInputLayout();

	CreateDiffuseTexture(L"crate.dds");
	CreateNormalTexture(L"NormalMap.dds");

	ZeroMemory(&m_GameConstBuffer, sizeof(GameShaderBuffer));

	timer.Restart();
}

void DEMO_APP::CreateVertexBuffer()
{
	// Create Vertex Buffer Description
	D3D11_BUFFER_DESC vertBuffDesc;
	ZeroMemory(&vertBuffDesc, sizeof(D3D11_BUFFER_DESC));
	vertBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBuffDesc.ByteWidth = m_Box.GetModel()->GetVertexStride() * m_Box.GetModel()->GetNumVertices();

	// Create Initial Vertex Data since the buffer is Immutable
	D3D11_SUBRESOURCE_DATA initialVertData;
	ZeroMemory(&initialVertData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialVertData.pSysMem = m_Box.GetModel()->GetVertices();

	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&vertBuffDesc, &initialVertData, &m_pVertexBuffer.p)))
		MessageBox(window, L"Failed To Create Vertex Buffer", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC indexBuffDesc;
	ZeroMemory(&indexBuffDesc, sizeof(D3D11_BUFFER_DESC));
	indexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.ByteWidth = m_Box.GetModel()->GetNumIndices() * sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA initialIndexData;
	ZeroMemory(&initialIndexData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialIndexData.pSysMem = m_Box.GetModel()->GetIndices().data();

	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&indexBuffDesc, &initialIndexData, &m_pIndexBuffer.p)))
		MessageBox(window, L"Failed To Create Index Buffer", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateShaders()
{
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex), NULL, &m_pVertexShader.p)))
		MessageBox(window, L"Failed To Create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(VertNormTanUVPixel, ARRAYSIZE(VertNormTanUVPixel), NULL, &m_pPixelShader.p)))
		MessageBox(window, L"Failed To Create Vertex Shader", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTNORMTANUV_LAYOUT;

	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTNORMTANUV_ELEMENTS, VertNormTanUVTransformedVertex, ARRAYSIZE(VertNormTanUVTransformedVertex), &m_pInputLayout.p)))
		MessageBox(window, L"Failed To Create Input Layout", L"", MB_OK | MB_ICONERROR);
}

void DEMO_APP::CreateDiffuseTexture(const wchar_t* szFilename)
{
	CreateDDSTextureFromFile(coreObjects.GetDevice(), szFilename, NULL, &m_pDiffuseTexture.p);
}

void DEMO_APP::CreateNormalTexture(const wchar_t* szFilename)
{
	CreateDDSTextureFromFile(coreObjects.GetDevice(), szFilename, NULL, &m_pNormalTexture.p);
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
	else if(m_bLButtonDown)
	{
		Vec3f result = camera.Unproject(currMousePos);

		result.normalize();// = XMVector4Normalize(result);

		//XMStoreFloat3(&m_GameConstBuffer.CameraToGravity, result);
		m_GameConstBuffer.CameraToGravity = result;

		result *= 40.0f;

		// camera pos
		result += camera.GetWorldMatrix().position;

		m_GameConstBuffer.gravityPos = result;
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
	UINT strides[] = {VERTNORMTANUV_SIZE};
	UINT offsets[] = {0};
	coreObjects.GetContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer.p, strides, offsets);
	coreObjects.GetContext()->IASetIndexBuffer(m_pIndexBuffer.p, DXGI_FORMAT_R32_UINT, 0);
	
	// Set Vertex and Pixel Shaders
	coreObjects.GetContext()->VSSetShader(m_pVertexShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(m_pPixelShader.p, 0, 0);

	// Set the Input Layout
	coreObjects.GetContext()->IASetInputLayout(m_pInputLayout.p);
	
	// Set Primitive Topology
	coreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set Textures
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &m_pDiffuseTexture.p);
	coreObjects.GetContext()->PSSetShaderResources(1, 1, &m_pNormalTexture.p);
	
	camera.SetMVPAndWorldMatrices(Matrix4f());

	// Draw Call
	coreObjects.GetContext()->DrawIndexed(m_Box.GetModel()->GetNumIndices(), 0, 0);

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