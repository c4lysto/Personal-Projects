#ifndef	VERTNORMTANUVPIXEL_HLSL
#define VERTNORMTANUVPIXEL_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

IBaseLight gLight;

float4 main(VERTNORMTANUV_OUT input) : SV_TARGET
{
	float4 objectColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);

	float3 surfaceNormal = NormalTexture.Sample(WrapSampler, input.texCoord).xyz;
	surfaceNormal = surfaceNormal * 2 - 1;
	surfaceNormal = mul(surfaceNormal, input.tbn);

	//lightAccumulation += gLight.CalculateDiffuseColor(input.worldPos, surfaceNormal);
	//lightAccumulation += gLight.CalculateSpecularColor(input.worldPos, surfaceNormal);

	return objectColor * float4(gLight.CalculateLightAccumulation(input.worldPos, surfaceNormal, SpecularTexture.Sample(WrapSampler, input.texCoord).r), 1.0f);
}

#endif