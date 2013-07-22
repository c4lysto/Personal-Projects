// DirectX Lab 1a Bouncing Shapes
// Author L.Norri CD DRX FullSail University
// This Lab introduces students to the D3D11 Device & DXGI SwapChain interfaces,
// basic 2D screenspace rendering, (Shaders provided) and Multi Sample Anti-Aliasing 

// Define specifically for the console setup. NOT neccesary in a standard Windows app
#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <atlbase.h> // Contains the declaration of CComPtr. (better than SAFE_RELEASE)
#include "resource.h" // icon file

// D3D11 includes and libs
#include <d3d11.h>
#include <directxmath.h> // what used to be XNA math
#pragma comment(lib, "d3d11.lib")

using namespace std;
using namespace DirectX; // DirectX math classes and functions are in this namespace

// Both C++ and The HLSL shaders use the defined sizes and structures within (eliminates redundancy)
#include "SharedDefines.h"

// initial window size will be detemined by the desired client area
#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	768
// defines if the application forces fullscreen or not.
#define START_FULLSCREEN	FALSE

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************
int WINAPI wWinMain(HINSTANCE hInstance,		// handle to application
					HINSTANCE hPrevInstance,	// handle to previous app
					LPTSTR lpCmdLine,		// command line string
					int nCmdShow );			// show window value

LRESULT CALLBACK WndProc(HWND hWnd,				// handle to window
						 UINT message,			// incoming message
						 WPARAM wparam,			// message info
						 LPARAM lparam );		// message info


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	// Variables we need
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	//************ INTIALIZATION **************************************************************************************/
	//*****************************************************************************************************************/
	// Initializing What we need to create the window. This is best handled in a seperate application class.

	application = hInstance; // grab application handle
	appWndProc = (WNDPROC)WndProc; // grab application window procedure


	//************ CREATE WINDOW ********************/
	WNDCLASSEX  wndClass;
	// register a new type of window
	ZeroMemory( &wndClass, sizeof( wndClass ) );
	wndClass.cbSize         = sizeof( WNDCLASSEX );             // size of window structure
	wndClass.lpfnWndProc    = appWndProc;						// message callback
	wndClass.lpszClassName  = L"DirectXApplication";             // class name
	wndClass.hInstance      = application;		                // handle to application
	wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    // default cursor
	wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME );  // background brush
	wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx( &wndClass );

	//************ COMPUTE WINDOW SIZE FROM THE ACTUAL DRAWABLE PORTION OF THE WINDOW ********************/

	// calculate the required window dimensions based on desired backbuffer size.
	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	//************ CREATE THE WINDOW ********************/

	// create window of registered type
	window = CreateWindow(	L"DirectXApplication", L"Lab 1 Windows App Sample",	// class name, window title 
		WS_OVERLAPPEDWINDOW,								// window style
		CW_USEDEFAULT, CW_USEDEFAULT,						// x & y coordinates
		window_size.right-window_size.left,					// width of window 
		window_size.bottom-window_size.top,					// height of the window
		NULL, NULL,											// parent window & menu
		application,										// handle to application
		this );												// transmit our App's mem address to WndPrc

	ShowWindow( window, SW_SHOW );	// display our new window

	//************ END INTIALIZATION **********************************************************************************/
	//*****************************************************************************************************************/
	// Everything above is not related directly to MSAA, but is necessary for us to use MSAA.
	// The real work is done below.




	//************ ENABLE MSAA ****************************************************************************************/
	//*****************************************************************************************************************/
	// decouple the swap chain, free and then reallocate it.
	immediateContext->ClearState();
	DXGI_SWAP_CHAIN_DESC sd;
	swapChain->GetDesc(&sd); // record creation settings

	// release the swap chain, switch back to windowed mode if in fullscreen (microsoft recommendation)
	BOOL state;
	swapChain->GetFullscreenState(&state,0);
	if(state) swapChain->SetFullscreenState(false,0);
	swapChain.Release(); // delete the current swapchain

	// now we can re-allocate the swap chain with out desired MSAA mode	
	// use the requested multisample amount with default quality
	// We've hardcoded the value here, but that's not what you should do.
	// Also, if you want to disable MSAA, be sure to set Quality to 0.
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
	sd.Windowed = true; // should always be true even if you want to start fullscreen

	// Re-create our new swapchain with requested multisample state
	CComPtr<IDXGIDevice> pDXGIDevice;
	HRESULT hr = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);      
	CComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
	CComPtr<IDXGIFactory> pIDXGIFactory;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

	// with a handle to the original factory we should be able to make a swap chain
	hr = pIDXGIFactory->CreateSwapChain(d3dDevice, &sd, &swapChain);

	// with our new swap chain created we must rebulid our render target views
	// if our device was fullscreen, set the stae back to fullscreen. (WM_SIZE will call resize)
	if(state)
	{
		// Tell the fullscreen to be a specfic resolution
		DXGI_MODE_DESC md;
		// paramters of zero tell the swapChain to try and reuse existing settings.
		ZeroMemory(&md, sizeof(DXGI_MODE_DESC));
		md.Format = DXGI_FORMAT_UNKNOWN;
		md.Width = sd.BufferDesc.Width;
		md.Height = sd.BufferDesc.Height;
		md.Height = BACKBUFFER_HEIGHT;
		swapChain->ResizeTarget(&md);
		swapChain->SetFullscreenState(true,0);
	}
	else // otherwise we must insure our render target view is created. This stuff below can be contained in a function for reuse later (hint).
	{

		// only allow this operation if required objects are present
		if(!d3dDevice || !swapChain || !immediateContext) return 0;

		// make sure our device context is not connected to the swapChain
		immediateContext->ClearState();

		// release the current RT view if it exists and resize the buffers to match the new window
		renderTargetView.Release();

		// resize our swapChain to the requested size ( 0,0 means figure it out yourself )
		static bool initial = true; // remove this when they fix the bug
		if(!initial) // we don't resize the buffers the first time around because it breaks the Graphics Debugger
			swapChain->ResizeBuffers(0,0,0,DXGI_FORMAT_UNKNOWN,DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		initial = false;

		//************ SET THE SWAPCHAIN BACKBUFFER AS OUR RENDERTARGET ************/

		CComPtr<ID3D11Texture2D> pBackBuffer; // temp handle to our backbuffer
		swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		// Create a render-target view using our device
		d3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &renderTargetView );

		// Bind the view to the immediate context (use .p when you want to interpret a com ptr as an array)
		immediateContext->OMSetRenderTargets( 1, &renderTargetView.p, NULL );

		//************ DEFINE THE VIEWPORT COVERING THE ENTIRE RENDERTARGET ********/

		DXGI_SWAP_CHAIN_DESC curr_size;
		swapChain->GetDesc(&curr_size);
		// Setup the viewport covering the entire rendertarget
		D3D11_VIEWPORT vp;
		vp.Width = float(curr_size.BufferDesc.Width);
		vp.Height = float(curr_size.BufferDesc.Height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		immediateContext->RSSetViewports( 1, &vp );

		// if we have a constant buffer for window size update it here
		if(sceneData)
		{
			// update C++ variable
			SCENE_DATA::theScene.backbuffer_dimensions.x = float(curr_size.BufferDesc.Width);
			SCENE_DATA::theScene.backbuffer_dimensions.y = float(curr_size.BufferDesc.Height);
			// send our updated data to the shader, use "UpdateSubresource" on USAGE_DEFAULT buffers 
			immediateContext->UpdateSubresource(sceneData, 0, 0, &SCENE_DATA::theScene, 0, 0);
		}
	}
	
	//************ END OF MSAA ****************************************************************************************/
	//*****************************************************************************************************************/



	//************ MAIN LOOP *************************/
	// If we had an application class, this is where we would call it's Run/Update function.
	// use to catch windows messages
	MSG msg; ZeroMemory( &msg, sizeof( msg ) );
	while ( msg.message != WM_QUIT)
	{	
		// get any system messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &msg );
			DispatchMessage( &msg ); 
		}
	}



	//************ SHUTDOWN *************************/
	// Cleaning up the Windows related stuff. This is best handled in a Shutdown function of a class.
	UnregisterClass( L"DirectXApplication", application ); // unregister window


	return 0; // done, myApp destructor will run, CComPtrs will call Release
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    // attempt to handle
	if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;

    switch ( message )
    {
		case WM_CREATE:
			// embedded creation extra parameter into the window's user data. (thanks to Rob.M)
			SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
		break;

        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
    }

	// NOTE: We could handle our MSAA, fullscree, and resize stuff here... Just a thought.

    // pass to default handler
    return DefWindowProc( hWnd, message, wParam, lParam );
}
