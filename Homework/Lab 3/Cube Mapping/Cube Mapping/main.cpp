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

#include "VertNormUVTransformedVertex.csh"
#include "Textured.csh"

#include "CubeMapVertex.csh"
#include "CubeMapPixel.csh"

#define BACKBUFFER_WIDTH	500
#define BACKBUFFER_HEIGHT	500

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	XTime							timer;
	DirectXCore						coreObjects;
	RenderController				renderController;
	
	CComPtr<ID3D11Buffer> pVertexBuffer;
	CComPtr<ID3D11Buffer> pIndexBuffer;
	CComPtr<ID3D11InputLayout> pInputLayout;

	CComPtr<ID3D11VertexShader> pVertexShader;
	CComPtr<ID3D11PixelShader> pPixelShader;

	CComPtr<ID3D11VertexShader> pCubeMapVertexShader;
	CComPtr<ID3D11PixelShader> pCubeMapPixelShader;

	CComPtr<ID3D11ShaderResourceView> pTextureView;
	CComPtr<ID3D11ShaderResourceView> pCubeMapTextureView;

	GameObject mCube, mCubeMap;
	XMMATRIX cubeMapMatrix;
	Camera camera;

	void Update();

public:
	
	
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
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
	
	mCube.GetModel()->CreateBoxVertNormUV(&coreObjects);
	
	D3D11_BUFFER_DESC vertBuffDesc;
	ZeroMemory(&vertBuffDesc, sizeof(D3D11_BUFFER_DESC));
	vertBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBuffDesc.CPUAccessFlags = 0;
	vertBuffDesc.ByteWidth = mCube.GetModel()->GetVertexStride() * mCube.GetModel()->GetNumVertices();
	
    // TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA initialVertData;
	ZeroMemory(&initialVertData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialVertData.pSysMem = mCube.GetModel()->GetVertices();

	// TODO: PART 2 STEP 3d
	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&vertBuffDesc, &initialVertData, &pVertexBuffer.p)))
		MessageBox(window, L"Failed to create Vertex Buffer", L"", MB_OK | MB_ICONERROR);

	D3D11_BUFFER_DESC indexBuffDesc;
	ZeroMemory(&indexBuffDesc, sizeof(D3D11_BUFFER_DESC));
	indexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.ByteWidth = mCube.GetModel()->GetNumIndices() * sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA initialIndexData;
	ZeroMemory(&initialIndexData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialIndexData.pSysMem = mCube.GetModel()->GetIndices().data();

	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&indexBuffDesc, &initialIndexData, &pIndexBuffer.p)))
		MessageBox(window, L"Failed to create Index Buffer", L"", MB_OK | MB_ICONERROR);
	
	// TODO: PART 2 STEP 7
	// NOTE: ask about 2nd parameter
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(VertNormUVTransformedVertex, ARRAYSIZE(VertNormUVTransformedVertex), NULL, &pVertexShader.p)))
		MessageBox(window, L"Failed to create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(Textured, ARRAYSIZE(Textured), NULL, &pPixelShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	// cube map shaders
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(CubeMapVertex, ARRAYSIZE(CubeMapVertex), NULL, &pCubeMapVertexShader.p)))
		MessageBox(window, L"Failed to create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(CubeMapPixel, ARRAYSIZE(CubeMapPixel), NULL, &pCubeMapPixelShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	// TODO: PART 2 STEP 8a
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTNORMUV_LAYOUT;
	
	// TODO: PART 2 STEP 8b
	// NOTE: ask about 4th parameter
	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTNORMUV_ELEMENTS, VertNormUVTransformedVertex, ARRAYSIZE(VertNormUVTransformedVertex), &pInputLayout.p)))
		MessageBox(window, L"Failed to create Input Layout", L"", MB_OK | MB_ICONERROR);

	camera.Initialize(&coreObjects, DEG_TO_RAD(71.0f), 0.1f, 200.0f);

	CreateDDSTextureFromFile(coreObjects.GetDevice(), _T("FullSailLogo.dds"), NULL, &pTextureView.p);

	CreateDDSTextureFromFile(coreObjects.GetDevice(), _T("CubeMap.dds"), NULL, &pCubeMapTextureView.p);

	cubeMapMatrix = XMMatrixIdentity();
	cubeMapMatrix *= XMMatrixScaling(20.0f, 20.0f, 20.0f);

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
	UINT strides[] = {VERTNORMUV_SIZE};
	UINT offsets[] = {0};
	coreObjects.GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer.p, strides, offsets);
	coreObjects.GetContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// TODO: PART 2 STEP 9b
	coreObjects.GetContext()->VSSetShader(pVertexShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(pPixelShader.p, 0, 0);
	
	// TODO: PART 2 STEP 9c
	coreObjects.GetContext()->IASetInputLayout(pInputLayout.p);
	
	// TODO: PART 2 STEP 9d
	coreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// NOTE: parameter 1 is the register number in the shader
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &pTextureView.p);
	camera.SetMVPAndWorldMatrices(&mCube);
	
	// TODO: PART 2 STEP 10
	coreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_BACK);
	coreObjects.CommitRasterizerChanges();
	coreObjects.GetContext()->DrawIndexed(mCube.GetModel()->GetNumIndices(), 0, 0);

	coreObjects.GetContext()->VSSetShader(pCubeMapVertexShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(pCubeMapPixelShader.p, 0, 0);

	camera.SetMVPAndWorldMatrices(cubeMapMatrix);

	coreObjects.GetContext()->PSSetShaderResources(2, 1, &pCubeMapTextureView.p);

	coreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_NONE);
	coreObjects.CommitRasterizerChanges();
	coreObjects.GetContext()->DrawIndexed(mCube.GetModel()->GetNumIndices(), 0, 0);

	// TODO: PART 1 STEP 8
	coreObjects.Present();
	
	// END OF PART 1
	return true; 
}

void DEMO_APP::Update()
{
	timer.Signal();
	double fElapsedTime = timer.Delta();

	if(GetAsyncKeyState('W'))
	{
		camera.MoveForward(fElapsedTime);
	}
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

	if(GetAsyncKeyState(VK_UP))
		camera.RotateUp(fElapsedTime);
	else if(GetAsyncKeyState(VK_DOWN))
		camera.RotateDown(fElapsedTime);

	if(GetAsyncKeyState(VK_LEFT))
		camera.RotateLeft(fElapsedTime);
	else if(GetAsyncKeyState(VK_RIGHT))
		camera.RotateRight(fElapsedTime);
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
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
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
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//