#ifndef TEXTUREDTINT_HLSL
#define TEXTUREDTINT_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(float2 texCoord : TEXCOORD0, float4 color : COLOR0) : SV_TARGET
{
	return DiffuseTexture1.Sample(WrapSampler, texCoord) * color;
}

#endif