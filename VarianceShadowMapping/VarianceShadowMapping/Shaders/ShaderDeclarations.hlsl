#ifndef SHADERDECLARATIONS_HLSL
#define SHADERDECLARATIONS_HLSL

#pragma pack_matrix(row_major)

#define RF_SOLIDWIREFRAME 0x1
#define RF_DISTTESS 0x2
#define RF_SSTESS 0x4

cbuffer GAME : register(b0)
{
	float2 gHalfPixel;
	uint gRenderFlags;
	float gWireFrameWidth;
}

cbuffer CAMERA : register(b1)
{
	float4x4 gViewMatrix;
	float4x4 gViewProjectionMatrix;
	float3 gCameraPos;
	float CAMERAFillData;
	float3 gViewDir;
	float CAMERAFillData2;
}

cbuffer OBJECT : register(b2)
{
	float4x4 gMVPMatrix : WORLDVIEWPROJECTION;
	float4x4 gModelViewMatrix : WORLDVIEW;
	float4x4 gWorldMatrix : WORLD;
}

cbuffer POSTPROCESS : register(b7)
{
	uint g_unScreenWidth;
	uint g_unScreenHeight;
	uint g_unPixelsPerThread;
	float g_fPostProcessFillData;
}

texture2D DiffuseTexture : register(t0);
texture2D NormalTexture : register(t1);
texture2D DisplacementTexture : register(t2);
texture2D SpecularTexture : register(t3);
texture2D ViewSpaceShadowMap : register(t4);
textureCUBE PointLightShadowMap : register(t5);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);
SamplerState ClampPointSampler : register(s2);

struct VERTUV
{
	float2 texCoord : TEXCOORD0;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUV_IN
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float2 texCoord : TEXCOORD0;
};

struct VERTNORMTANUV_OUT
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3x3 tbn : TEXCOORD2;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUVDIRSHADOW_OUT
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float depth : TEXCOORD2;
	float4 shadowTexCoord : TEXCOORD3;
	float3x3 tbn : TEXCOORD4;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUVSPOTSHADOW_OUT
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 viewPos : TEXCOORD2;
	float4 shadowTexCoord : TEXCOORD3;
	float3x3 tbn : TEXCOORD4;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUVPOINTSHADOW_OUT
{
	/*float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3x3 tbn : TEXCOORD2;
	float4 position : SV_POSITION;*/

	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	//float3 viewPos : TEXCOORD2;
	float3 lightToPixel : TEXCOORD2;
	float3x3 tbn : TEXCOORD3;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUV_TESS_VS_OUT
{
	float2 texCoord : TEXCOORD0;
	float3x3 tbn : TEXCOORD1;
	float4 position : SV_POSITION;
};

struct VERTNORMTANUV_TESS_DS_OUT
{
	float2 texCoord : TEXCOORD0;
	float3x3 tbn : TEXCOORD1;
	float3 worldPos : TEXCOORD4;
	float4 position : SV_POSITION;
};

#endif