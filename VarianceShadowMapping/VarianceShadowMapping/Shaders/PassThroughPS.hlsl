#ifndef PASSTHROUGHPS_HLSL
#define PASSTHROUGHPS_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(float2 texCoord : TEXCOORD0) : SV_TARGET
{
	return DiffuseTexture.Sample(ClampPointSampler, texCoord);
}

#endif