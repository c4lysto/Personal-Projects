// DirectX Lab 1a Window Sample
// Author T.Gregg LS DRX FullSail University
// Basic Windows app setup.


#include <atlbase.h> // Contains the declaration of CComPtr. (better than SAFE_RELEASE)
#include "resource.h" // icon file

// initial window size will be detemined by the desired client area
#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	768

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

int WINAPI wWinMain(HINSTANCE hInstance,		// handle to application
				   HINSTANCE hPrevInstance,		// handle to previous app
				   LPTSTR lpCmdLine,			// command line string
				   int nCmdShow );				// show window value
				   
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
							&appWndProc );										// transmit our App's mem address to WndPrc

    ShowWindow( window, SW_SHOW );	// display our new window
	
	//************ END INTIALIZATION **********************************************************************************/
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
    // pass to default handler
    return DefWindowProc( hWnd, message, wParam, lParam );
}
