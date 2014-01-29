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

#include "VertClrTransformedVertex.csh"
#include "TexturedTint.csh"
//#include "ColorPixel.csh"
#include "ComputeParticle.csh"
#include "PointToQuadGS.csh"
#include "ParticleFloatCS.csh"
#include "ExplodeCS.csh"

#ifdef _DEBUG
#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	720
#else
#define BACKBUFFER_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define BACKBUFFER_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#endif

#define GAME_CONST_BUFF_REGISTER 0
#define MAX_PARTICLES 1024000
#define MAX_COMPUTE_THREADS D3D11_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP

#define PARTICLE_FADE_TIME 20.0f
#define FAST_CAMERA_MODIFIER 3.0f

#define GRAVITY_OFFSET 50.0f

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
		Vec3f prevPos;
		Vec3f force;
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

	CComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;

	CComPtr<ID3D11InputLayout> pInputLayout;

	CComPtr<ID3D11VertexShader> pVertexShader;
	CComPtr<ID3D11PixelShader> pPixelShader;
	CComPtr<ID3D11GeometryShader> pGeometryShader;

	CComPtr<ID3D11ComputeShader> pComputeShader;
	CComPtr<ID3D11ComputeShader> pParticleFloatCS;
	CComPtr<ID3D11ComputeShader> pExplodeCS;

	CComPtr<ID3D11ShaderResourceView> pTextureView;
	CComPtr<ID3D11ShaderResourceView> m_ParticleSRV;

	POINT m_PrevMousePos;
	float m_fParticleFadeTimer;

	Camera camera;

	void Update();
	void UpdateGameConstBuff();

public:
	
	
	DEMO_APP() {}
	void Initialize(HINSTANCE hinst, WNDPROC proc);
	void OnMouseMove();
	void Explode(LPARAM lParam);
	void ResetParticleFadeTimer();
	bool Run();
	bool ShutDown();

	void SetCurrentMousePos(POINT mousePos) {m_PrevMousePos = mousePos;}
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
	camera.Initialize(&coreObjects, DEG_TO_RAD(42.0f), 0.1f, 500.0f);
	m_fParticleFadeTimer = PARTICLE_FADE_TIME;

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

	std::vector<ParticleData> particles;
	particles.resize(MAX_PARTICLES);
	for(unsigned int i = 0; i < MAX_PARTICLES; ++i)
	{
		particles[i].position.x = (rand() % 1501 - 500) * 0.01f;
		particles[i].position.y = (rand() % 1501 - 500) * 0.01f;
		particles[i].position.z = (rand() % 1501 - 500) * 0.01f;

		particles[i].prevPos = particles[i].position;

		particles[i].color.x = (rand() % 101 + 1) * 0.01f;
		particles[i].color.y = (rand() % 101 + 1) * 0.01f;
		particles[i].color.z = (rand() % 101 + 1) * 0.01f;
		particles[i].color.w = 1.0f;

		particles[i].force.x = (rand() % 501 - 250) * 0.1f;
		particles[i].force.y = (rand() % 501 - 250) * 0.1f;
		particles[i].force.z = (rand() % 501 - 250) * 0.1f;
	}
	
	// TODO: PART 2 STEP 7
	// NOTE: ask about 2nd parameter
	if(FAILED(coreObjects.GetDevice()->CreateVertexShader(VertClrTransformedVertex, ARRAYSIZE(VertClrTransformedVertex), NULL, &pVertexShader.p)))
		MessageBox(window, L"Failed to create Vertex Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreatePixelShader(TexturedTint, ARRAYSIZE(TexturedTint), NULL, &pPixelShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreateGeometryShader(PointToQuadGS, ARRAYSIZE(PointToQuadGS), NULL, &pGeometryShader.p)))
		MessageBox(window, L"Failed to create Pixel Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreateComputeShader(ComputeParticle, ARRAYSIZE(ComputeParticle), NULL, &pComputeShader.p)))
		MessageBox(window, L"Failed to create ComputeParticle Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreateComputeShader(ParticleFloatCS, ARRAYSIZE(ParticleFloatCS), NULL, &pParticleFloatCS.p)))
		MessageBox(window, L"Failed to create ParticleFloat Shader", L"", MB_OK | MB_ICONERROR);

	if(FAILED(coreObjects.GetDevice()->CreateComputeShader(ExplodeCS, ARRAYSIZE(ExplodeCS), NULL, &pExplodeCS.p)))
		MessageBox(window, L"Failed to create Explode Shader", L"", MB_OK | MB_ICONERROR);

	// TODO: PART 2 STEP 8a
	D3D11_INPUT_ELEMENT_DESC vLayout[] = VERTCLR_LAYOUT;
	
	// TODO: PART 2 STEP 8b
	// NOTE: ask about 4th parameter
	if(FAILED(coreObjects.GetDevice()->CreateInputLayout(vLayout, NUM_VERTCLR_ELEMENTS, VertClrTransformedVertex, ARRAYSIZE(VertClrTransformedVertex), &pInputLayout.p)))
		MessageBox(window, L"Failed to create Input Layout", L"", MB_OK | MB_ICONERROR);

	CreateDDSTextureFromFile(coreObjects.GetDevice(), _T("particle.dds"), NULL, &pTextureView.p);

	D3D11_BUFFER_DESC structuredBuffDesc;
	ZeroMemory(&structuredBuffDesc, sizeof(D3D11_BUFFER_DESC));
	structuredBuffDesc.StructureByteStride = sizeof(ParticleData);
	structuredBuffDesc.ByteWidth = sizeof(ParticleData) * MAX_PARTICLES;
	structuredBuffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	structuredBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; 

	D3D11_SUBRESOURCE_DATA structuredBuffData;
	ZeroMemory(&structuredBuffData, sizeof(D3D11_SUBRESOURCE_DATA));
	structuredBuffData.pSysMem = particles.data();

	particles.clear();

	if(FAILED(coreObjects.GetDevice()->CreateBuffer(&structuredBuffDesc, &structuredBuffData, &pStructuredBuffer.p)))
		MessageBox(window, L"Failed to create Structured Buffer", L"", MB_OK | MB_ICONERROR);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = MAX_PARTICLES;
	coreObjects.GetDevice()->CreateShaderResourceView(pStructuredBuffer.p, &srvDesc, &m_ParticleSRV.p);


	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	coreObjects.GetDevice()->CreateBlendState(&blendDesc, &pBlendState.p);

	//NOTE!!! Dont forget to finish this
	coreObjects.GetContext()->OMSetBlendState(pBlendState.p, NULL, UINT_MAX);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = structuredBuffDesc.ByteWidth / structuredBuffDesc.StructureByteStride;

	if(FAILED(coreObjects.GetDevice()->CreateUnorderedAccessView(pStructuredBuffer.p, &UAVDesc, &m_UnorderedAccessView.p)))
		MessageBox(window, L"Failed to create Unordered Access View", L"", MB_OK | MB_ICONERROR);

	ZeroMemory(&m_GameConstBuffer, sizeof(GameShaderBuffer));

	timer.Restart();
}

void DEMO_APP::OnMouseMove()
{
	POINT ptCurrMousePos;
	GetCursorPos(&ptCurrMousePos);

	if(m_bRButtonDown)
	{
		POINT mouseMovement = {m_PrevMousePos.x - ptCurrMousePos.x, m_PrevMousePos.y - ptCurrMousePos.y};

		camera.RotateCameraMouseMovement(mouseMovement, timer.Delta());

		POINT screenPos = lockedMousePos;
		SetCursorPos(screenPos.x, screenPos.y);

		m_PrevMousePos = lockedMousePos;

		return;
	}
	if(m_bLButtonDown)
	{
		POINT ptClientPos = ptCurrMousePos;
		ScreenToClient(window, &ptClientPos);
		Vec3f result = camera.Unproject(ptCurrMousePos);

		result.normalize();// = XMVector4Normalize(result);

		//XMStoreFloat3(&m_GameConstBuffer.CameraToGravity, result);
		m_GameConstBuffer.CameraToGravity = result;

		result *= GRAVITY_OFFSET;

		// camera pos
		result += camera.GetWorldMatrix().position;

		m_GameConstBuffer.gravityPos = result;
	}
}

void DEMO_APP::ResetParticleFadeTimer()
{
	m_fParticleFadeTimer = PARTICLE_FADE_TIME;
}				

void DEMO_APP::UpdateGameConstBuff()
{
	//if(m_bLButtonDown)
		m_GameConstBuffer.fElapsedTime = (float)timer.Delta();
	/*else if(m_fParticleFadeTimer > 0.0f)
	{
		m_fParticleFadeTimer = max(m_fParticleFadeTimer - (float)timer.Delta(), 0.0f);

		if(!m_fParticleFadeTimer)
			int x = 0;

		m_GameConstBuffer.fElapsedTime = (float)timer.Delta() * (m_fParticleFadeTimer / PARTICLE_FADE_TIME);
	}*/

	m_GameConstBuffer.GameTime = GetTickCount();

	D3D11_MAPPED_SUBRESOURCE gameSubresource;

	if(FAILED(coreObjects.GetContext()->Map(pGameConstBuffer.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &gameSubresource)))
		MessageBox(window, L"Failed to Map Globals", L"", MB_OK | MB_ICONERROR);

		memcpy(gameSubresource.pData, &m_GameConstBuffer, sizeof(GameShaderBuffer));

	coreObjects.GetContext()->Unmap(pGameConstBuffer.p, 0);
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	Update();
	camera.UpdateMatrices();

#pragma region Setup And Run Compute Shader
	coreObjects.GetContext()->CSSetUnorderedAccessViews(0, 1, &m_UnorderedAccessView.p, 0);

	if(m_bLButtonDown)
	{
		coreObjects.GetContext()->CSSetShader(pComputeShader.p, 0, 0);
	}
	else
	{
		//if(m_fParticleFadeTimer > 0.0f)
			coreObjects.GetContext()->CSSetShader(pParticleFloatCS.p, 0, 0);
		//else
			//coreObjects.GetContext()->CSSetShader(NULL, 0, 0);
	}

	UINT threadgroups = MAX_PARTICLES / MAX_COMPUTE_THREADS;
	coreObjects.GetContext()->Dispatch(threadgroups, 1, 1);

	ID3D11UnorderedAccessView *pNULLuav = NULL;
	coreObjects.GetContext()->CSSetUnorderedAccessViews(0, 1, &pNULLuav, 0);
#pragma endregion

	coreObjects.Clear();
	
	// TODO: PART 2 STEP 9a
	//UINT strides = 0;//[] = {VERTCLR_SIZE};
	//UINT offsets = 0;//[] = {0};
	//ID3D11Buffer* pNUllBuffer = NULL; 
	//coreObjects.GetContext()->IASetVertexBuffers(0, 1, &pNUllBuffer, &strides, &offsets);
	
	// TODO: PART 2 STEP 9b
	coreObjects.GetContext()->VSSetShader(pVertexShader.p, 0, 0);
	coreObjects.GetContext()->GSSetShader(pGeometryShader.p, 0, 0);
	coreObjects.GetContext()->PSSetShader(pPixelShader.p, 0, 0);
	
	// TODO: PART 2 STEP 9c
	//coreObjects.GetContext()->IASetInputLayout(pInputLayout.p);
	//ID3D11InputLayout *pNULLInputLayout = NULL;
	//coreObjects.GetContext()->IASetInputLayout(pNULLInputLayout);
	coreObjects.GetContext()->VSSetShaderResources(0, 1, &m_ParticleSRV.p);
	
	// TODO: PART 2 STEP 9d
	coreObjects.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// NOTE: parameter 1 is the register number in the shader
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &pTextureView.p);
	
	camera.SetMVPAndWorldMatrices(Matrix4f());

	// TODO: PART 2 STEP 10
	coreObjects.GetContext()->Draw(MAX_PARTICLES, 0);

	ID3D11ShaderResourceView *pNULLsrv = NULL;
	coreObjects.GetContext()->VSSetShaderResources(0, 1, &pNULLsrv);
	coreObjects.GetContext()->PSSetShaderResources(0, 1, &pNULLsrv);

	// TODO: PART 1 STEP 8
	coreObjects.Present(PRESENT_VSYNC);
	
	// END OF PART 1
	return true; 
}

void DEMO_APP::Explode(LPARAM lParam)
{
	POINT currMousePos = {LOWORD(lParam), HIWORD(lParam)};

	Vec3f result = camera.Unproject(currMousePos);

	result.normalize();// = XMVector4Normalize(result);

	//XMStoreFloat3(&m_GameConstBuffer.CameraToGravity, result);
	m_GameConstBuffer.CameraToGravity = result;

	result *= GRAVITY_OFFSET;

	// camera pos
	result += camera.GetWorldMatrix().position;

	m_GameConstBuffer.gravityPos = result;

	ResetParticleFadeTimer();
	UpdateGameConstBuff();

	coreObjects.GetContext()->CSSetUnorderedAccessViews(0, 1, &m_UnorderedAccessView.p, 0);

	coreObjects.GetContext()->CSSetShader(pExplodeCS.p, 0, 0);

	UINT threadgroups = MAX_PARTICLES / MAX_COMPUTE_THREADS;
	coreObjects.GetContext()->Dispatch(threadgroups, 1, 1);

	ID3D11UnorderedAccessView *pNULLuav = NULL;
	coreObjects.GetContext()->CSSetUnorderedAccessViews(0, 1, &pNULLuav, 0);
}

void DEMO_APP::Update()
{
	timer.Signal();
	double fElapsedTime = timer.Delta();

	OnMouseMove();

	UpdateGameConstBuff();

	if(GetAsyncKeyState(VK_SPACE))
		fElapsedTime *= FAST_CAMERA_MODIFIER;

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

				ClientToScreen(hWnd, &lockedMousePos);

				myApp.SetCurrentMousePos(lockedMousePos);
			}
			break;

		case WM_RBUTTONUP:
			{
				m_bRButtonDown = false;
			}
			break;

		case WM_LBUTTONDOWN:
			{
				m_bLButtonDown = true;
			}
			break;

		case WM_LBUTTONUP:
			{
				m_bLButtonDown = false;
				myApp.ResetParticleFadeTimer();
			}
			break;

		case WM_MBUTTONDOWN:
			{
				myApp.Explode(lParam);
			}
			break;

		case WM_MOUSEWHEEL:
			{
				short x = (short)HIWORD(wParam);
			}
			break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//