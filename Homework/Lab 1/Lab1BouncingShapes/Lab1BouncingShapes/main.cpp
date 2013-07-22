
// DirectX Lab 1a Bouncing Shapes
// Author L.Norri CD DRX FullSail University
// This Lab introduces students to the D3D11 Device & DXGI SwapChain interfaces,
// basic 2D screenspace rendering, (Shaders provided) and Multi Sample Anti-Aliasing 

// Define specifically for the console setup. NOT neccesary in a standard Windows app
#include "stdafx.h"

// Defines for the shaders/loading of shaders
#include <iostream>
#include <fstream>
#include <atlbase.h> // Contains the declaration of CComPtr. (better than SAFE_RELEASE)

// D3D11 includes and libs
#include <d3d11.h>
#include <directxmath.h> // what used to be XNA math
#pragma comment(lib, "d3d11.lib")

using namespace std;
using namespace DirectX; // DirectX math classes and functions are in this namespace


int _tmain(int argc, _TCHAR* argv[])
{
	// Needed variables
	CComPtr<ID3D11Device>			d3dDevice;
	CComPtr<ID3D11DeviceContext>	immediateContext;
	CComPtr<ID3D11PixelShader>		pixelShader;
	CComPtr<ID3D11VertexShader>		vertexShader;

	
//************ SETUP THE DEVICE ***********************************************************************************************/

	// if running in debug we can turn on certain options to improve the debugging process.
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED; // (optimization) turn this off when multithreading D3D11
#ifdef _DEBUG
	// these two flags make debugging the application a bit easier. (requires Win8 SDK)
	flags = flags | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS;
	// FYI: "D3D11_CREATE_DEVICE_DEBUGGABLE" can be turned on if you are using D3D11.1 for imporved shader debugging.
	// FYI: "D3D11_CREATE_DEVICE_BGRA_SUPPORT" can be turned on to allow D3D11 resources to interop with the Direct2D API.
#endif
	

	cout << "Attempting to create device... ";

	// Create the device and only device. We're not drawing to the screen so we don't need a swap chain.
	D3D_FEATURE_LEVEL supported, requested = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = D3D11CreateDevice(	NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,  &requested, 1, D3D11_SDK_VERSION, &d3dDevice, &supported, &immediateContext );
	
	if(hr == 0){ cout << "Success! \n\n";}else{ cout << "Failed. Aborting.\n"; system("pause"); return 0;}

//************ LOAD PRE COMPILED SHADERS (COMPILED DIRECTLY INTO SOURCE IN RELEASE) *********************************************/
// Depending if we're in release or debug, we'll do this differently... pay close attention to whats going on here.

#ifdef _DEBUG 
	
	#ifdef _WIN64
		#define SHADER_BYTECODE_PATH "../x64/Debug/" 
	#else	
		#define SHADER_BYTECODE_PATH "../Debug/"
	#endif

	cout<<"DEBUG: Reading in vertex shader.\n\n";

	// Read the vertex shader into memory, all of the steps 
	// of this process can be done to load in the pixel shader as well.
	ifstream load; 	
	char *vs_byte_code = 0, *ps_byte_code = 0;


	load.open(SHADER_BYTECODE_PATH "screen2clip_VS.cso", ios_base::binary);
	load.seekg( 0, ios_base::end ); 

	size_t vs_byte_code_size = size_t(load.tellg());
	vs_byte_code = new char[vs_byte_code_size];

	load.seekg( 0, ios_base::beg ); 
	load.read( vs_byte_code, vs_byte_code_size );
	load.close();
	
	// again, similar steps can be taken to load the Pixel Shader into memory.

#else
	// this will embed the shaders into your executable, be warned it can signifcantly increases build time
	#ifdef _WIN64
		#include "../x64/Release/screen2clip_VS.vsh"
	#else
		#include "../Release/screen2clip_VS.vsh"
	#endif
	
	size_t vs_byte_code_size = sizeof(vs_byte_code);

#endif

	cout << "Loading Vertex Shader in vram. \n\n";

	// load vertex shader into vram
	hr = d3dDevice->CreateVertexShader(vs_byte_code, vs_byte_code_size, NULL, &vertexShader); 


#ifdef _DEBUG
	cout << "DEBUG: Releasing memory.\n\n";
	
	// free allocated shader bytecode memory
	if(vs_byte_code) delete [] vs_byte_code;
#endif	

	system("pause");
	
	// Shutdown our immediateContext
	if(immediateContext) 
		immediateContext->ClearState();

	return 0;
}

