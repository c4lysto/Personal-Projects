#ifndef SHADERDECLARATIONS_HLSL
#define SHADERDECLARATIONS_HLSL

#pragma pack_matrix(row_major)

cbuffer CAMERA : register(b0)
{
	float4x4 ViewProjectionMatrix;
	float4x4 InvViewProjMatrix;
}

cbuffer OBJECT : register(b1)
{
	float4x4 MVPMatrix;
	float4x4 WorldMatrix;
}

struct VERTCLR_IN
{
	float3 position : POSITION0;
	float4 color : COLOR0;
};

struct VERTCLR_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

#endif