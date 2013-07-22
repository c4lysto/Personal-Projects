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

using namespace std;

// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// TODO: PART 1 STEP 1b
#include <DirectXMath.h>
using namespace DirectX;

// TODO: PART 2 STEP 6
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"

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
	
	// TODO: PART 2 STEP 2
	CComPtr<ID3D11Buffer> pVertexBuffer;
	CComPtr<ID3D11InputLayout> pInputLayout;
	
	// BEGIN PART 5
	// TODO: PART 5 STEP 1
	
	// TODO: PART 2 STEP 4
	CComPtr<ID3D11VertexShader> pVertexShader;
	CComPtr<ID3D11PixelShader> pPixelShader;
	
	// BEGIN PART 3
	// TODO: PART 3 STEP 1

	// TODO: PART 3 STEP 2b
	
	// TODO: PART 3 STEP 4a

public:
	// BEGIN PART 2
	// TODO: PART 2 STEP 1
	struct SIMPLE_VERTEX
	{
		XMFLOAT2 position;
	};
	
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
	
	// TODO: PART 2 STEP 3a
	SIMPLE_VERTEX circle[360];
	for(unsigned int i = 0; i < 360; ++i)
	{
		circle[i].position.x = cos(DEG_TO_RAD(i));
		circle[i].position.y = sin(DEG_TO_RAD(i));
	}
	
	// BEGIN PART 4
	// TODO: PART 4 STEP 1

	// TODO: PART 2 STEP 3b
	D3D11_BUFFER_DESC vertBuffDesc;
	ZeroMemory(&vertBuffDesc, sizeof(D3D11_BUFFER_DESC));
	vertBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBuffDesc.CPUAccessFlags = 0;
	vertBuffDesc.StructureByteStride = sizeof(SIMPLE_VERTEX);
	vertBuffDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 360;
	
    // TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA initialVertData;
	ZeroMemory(&initialVertData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialVertData.pSysMem = circle;

	// TODO: PART 2 STEP 3d
	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&vertBuffDesc, &initialVertData, &pVertexBuffer.p)))
		MessageBox(window, L"Failed to create Vertex Buffer", L"", MB_OK | MB_ICONERROR);
	
	// TODO: PART 5 STEP 2a
	
	// TODO: PART 5 STEP 2b
	
	// TODO: PART 5 STEP 3
		
	// TODO: PART 2 STEP 5
	// ADD SHADERS TO PROJECT, SET BUILD OPTIONS & COMPILE
	
	// TODO: PART 2 STEP 7
	// NOTE: ask about 2nd parameter
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(Trivial_VS, ARRAYSIZE(Trivial_VS), NULL, &pVertexShader.p)))
		MessageBox(window, L"Failed to create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(Trivial_PS, ARRAYSIZE(Trivial_PS), NULL, &pPixelShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	// TODO: PART 2 STEP 8a
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}//,
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	// TODO: PART 2 STEP 8b
	// NOTE: ask about 4th parameter
	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, 1, Trivial_VS, ARRAYSIZE(Trivial_VS), &pInputLayout.p)))
		MessageBox(window, L"Failed to create Input Layout", L"", MB_OK | MB_ICONERROR);
    
	// TODO: PART 3 STEP 3

	// TODO: PART 3 STEP 4b

}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	// TODO: PART 4 STEP 2	
	
	// TODO: PART 4 STEP 3
	
	// TODO: PART 4 STEP 5
	
	// END PART 4

	coreObjects.Clear();

	
	// TODO: PART 5 STEP 4
	
	// TODO: PART 5 STEP 5
	
	// TODO: PART 5 STEP 6
	
	// TODO: PART 5 STEP 7
	
	// END PART 5
	
	// TODO: PART 3 STEP 5
	
	// TODO: PART 3 STEP 6
	
	// TODO: PART 2 STEP 9a
	// NOTE: ask about more efficient way to do this
	UINT strides[] = {8};
	UINT offsets[] = {0};
	coreObjects.GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer.p, strides, offsets);
	
	// TODO: PART 2 STEP 9b
	coreObjects.GetContext()->VSSetShader(pVertexShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(pPixelShader.p, 0, 0);
	
	// TODO: PART 2 STEP 9c
	coreObjects.GetContext()->IASetInputLayout(pInputLayout.p);
	
	// TODO: PART 2 STEP 9d
	coreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	// TODO: PART 2 STEP 10
	coreObjects.GetContext()->Draw(360, 0);

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