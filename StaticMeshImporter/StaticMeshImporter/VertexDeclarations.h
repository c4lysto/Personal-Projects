#ifndef VERTEXDECLARATIONS_H
#define VERTEXDECLARATIONS_H

#include <DirectXMath.h>
using namespace DirectX;

namespace VertexDecl
{
	struct VertNormUV
	{
		Vec3f position, normal;
	
		union
		{
			float UV[2];
		
			struct
			{
				Vec2f uv;
			};		
		};
	};

#define VERTNORMUV_SIZE sizeof(VertNormUV)
#define NUM_VERTNORMUV_ELEMENTS 3

#define VERTNORMUV_LAYOUT\
	{\
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
	}


	struct VertClr
	{
		Vec3f position;
		Vec4f color;
	};

#define VERTCLR_SIZE sizeof(VertClr)
#define NUM_VERTCLR_ELEMENTS 2

#define VERTCLR_LAYOUT\
	{\
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},\
	}
};

#endif