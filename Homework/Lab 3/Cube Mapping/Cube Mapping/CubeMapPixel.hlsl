#ifndef	CUBEMAPPIXEL_HLSL
#define CUBEMAPPIXEL_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(float3 texCoord : TEXCOORD0) : SV_TARGET
{
	return CubeMapDiffuse.Sample(WrapSampler, texCoord);
}

#endif