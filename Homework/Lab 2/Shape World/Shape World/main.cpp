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
using namespace VertexDecl;

using namespace std;

// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// TODO: PART 1 STEP 1b
#include <DirectXMath.h>
using namespace DirectX;

#include "VertClrTransformedVertex.csh"
#include "ColorPixel.csh"

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
	DirectXCore						coreObjects;
	
	CComPtr<ID3D11Buffer> pVertexBuffer;
	CComPtr<ID3D11Buffer> pIndexBuffer;
	CComPtr<ID3D11InputLayout> pInputLayout;

	CComPtr<ID3D11VertexShader> pVertexShader;
	CComPtr<ID3D11PixelShader> pPixelShader;

	GameObject mCube;
	Camera camera;

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
	
	mCube.GetModel()->CreateBoxVertClr(&coreObjects);
	
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
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(VertClrTransformedVertex, ARRAYSIZE(VertClrTransformedVertex), NULL, &pVertexShader.p)))
		MessageBox(window, L"Failed to create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(ColorPixel, ARRAYSIZE(ColorPixel), NULL, &pPixelShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	// TODO: PART 2 STEP 8a
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTCLR_LAYOUT;
	
	// TODO: PART 2 STEP 8b
	// NOTE: ask about 4th parameter
	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTCLR_ELEMENTS, VertClrTransformedVertex, ARRAYSIZE(VertClrTransformedVertex), &pInputLayout.p)))
		MessageBox(window, L"Failed to create Input Layout", L"", MB_OK | MB_ICONERROR);

	camera.Initialize(&coreObjects, 71.0f, 0.1f, 200.0f);

	coreObjects.ChangeRasterizerState(RS_CULL_MODE, D3D11_CULL_NONE);
	coreObjects.CommitRasterizerChanges();
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	camera.UpdateMatrices();

	coreObjects.Clear();

	
	// TODO: PART 5 STEP 4
	
	// TODO: PART 5 STEP 5
	
	// TODO: PART 5 STEP 6
	
	// TODO: PART 5 STEP 7
	
	// END PART 5
	
	// TODO: PART 3 STEP 5
	
	// TODO: PART 3 STEP 6
	
	// TODO: PART 2 STEP 9a
	UINT strides[] = {VERTCLR_SIZE};
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

	camera.SetMVPAndWorldMatrices(&mCube);
	
	// TODO: PART 2 STEP 10
	coreObjects.GetContext()->DrawIndexed(mCube.GetModel()->GetNumIndices(), 0, 0);

	// END PART 2

	// TODO: PART 1 STEP 8
	coreObjects.Present();
	
	// END OF PART 1
	return true; 
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