#ifndef	TEXTURED_HLSL
#define	TEXTURED_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(float2 texCoord : TEXCOORD0) : SV_TARGET
{
	return DiffuseTexture1.Sample(WrapSampler, texCoord);
}

#endif