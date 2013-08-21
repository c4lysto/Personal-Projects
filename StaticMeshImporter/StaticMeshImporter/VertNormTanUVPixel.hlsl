#ifndef	VERTNORMTANUVPIXEL_HLSL
#define VERTNORMTANUVPIXEL_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(VERTNORMTANUV_OUT input) : SV_TARGET
{
	float4 diffuseColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);

	float3 surfaceNormal = NormalTexture.Sample(WrapSampler, input.texCoord);
	surfaceNormal = surfaceNormal * 2 - 1;
	surfaceNormal = mul(surfaceNormal, input.tbn);
	surfaceNormal = surfaceNormal * 0.5f + 0.5f;
	return float4(surfaceNormal, 1.0f);

	//return diffuseColor;
}

#endif