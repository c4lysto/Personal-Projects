#ifndef SHADERDECLARATIONS_HLSL
#define SHADERDECLARATIONS_HLSL

#pragma pack_matrix(row_major)

cbuffer GAME : register(b0)
{
	float fElapsedTime;
	float3 gravityPos;
	float3 CameraToGravity;
	uint GameTime;
}

cbuffer CAMERA : register(b1)
{
	float4x4 gViewProjectionMatrix;
	float3 gCameraPos;
	float cameraFillData;
}

cbuffer OBJECT : register(b2)
{
	float4x4 gMVPMatrix;
	float4x4 gWorldMatrix;
}

texture2D DiffuseTexture : register(t0);
texture2D NormalTexture : register(t1);
texture2D SpecularTexture : register(t2);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

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

#endif