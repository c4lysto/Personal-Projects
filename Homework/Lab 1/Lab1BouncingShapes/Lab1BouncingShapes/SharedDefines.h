// This file is compiled both by C++ and HLSL
// It is used to define the data interchange between the shaders and the application.
// By using a few language sensitive macros we can avoid redundant structure and variable declarations.

// if compiling this file in C++ switch to appropriate types
// doing this will allow us to avoid re-defining structures shared between HLSL and C++
#ifdef __cplusplus
	typedef UINT	 uint;
	typedef XMFLOAT2 float2;
	typedef XMFLOAT3 float3;
	typedef XMFLOAT4 float4;
	typedef XMMATRIX float4x4;
	// allows us to attach semantics to HLSL variables without bugging C++
	#define SEMANTIC(s_name) /* : s_name */
	// In C++ constant buffers will be contained by a namespace instead
	#define CONSTANT_BUFFER_BEGIN(cb_name) namespace cb_name {
	#define CONSTANT_BUFFER_END }
#else
	// by switching our matrix packing to row-major we can avoid uneeded transposing of matricies to and from C++ 
	#pragma pack_matrix( row_major )
	// allows us to attach semantics to HLSL variables without bugging C++
	#define SEMANTIC(s_name) : s_name
	// In HLSL constant buffers will be identified by their name and sequential ordering
	#define CONSTANT_BUFFER_BEGIN(cb_name) cbuffer cb_name {
	#define CONSTANT_BUFFER_END }
#endif

// max shapes that can be drawn at once
#define MAX_SHAPES 32

// variables describing one shape instance
struct SHAPE_INFO
{
	float4x4 shape_matrix;
	float4 shape_color;
};
// variables defining the drawing enviroment
struct SCENE_INFO
{
	float2 backbuffer_dimensions;
};

// data for a set of < 32 shapes
CONSTANT_BUFFER_BEGIN(SHAPE_DATA)

	SHAPE_INFO allShapes[MAX_SHAPES];

CONSTANT_BUFFER_END // end shape data

// data describing the window
CONSTANT_BUFFER_BEGIN(SCENE_DATA)

	SCENE_INFO theScene;

CONSTANT_BUFFER_END // done with scene data

// format of an screen vertex HLSL and C++
struct SCREEN_VERTEX
{
	float3 pos	SEMANTIC(POSITION);
	float4 rgba	SEMANTIC(COLOR);
	uint index	SEMANTIC(TEXCOORD);	
};

