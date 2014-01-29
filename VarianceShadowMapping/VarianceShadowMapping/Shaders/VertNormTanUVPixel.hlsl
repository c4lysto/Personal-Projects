#ifndef	VERTNORMTANUVPIXEL_HLSL
#define VERTNORMTANUVPIXEL_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

IBaseLight gLights;//[MAX_LIGHTS];

float4 main(VERTNORMTANUV_OUT input) : SV_TARGET
{
	float4 objectColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);

	float3 surfaceNormal = NormalTexture.Sample(WrapSampler, input.texCoord).xyz;
	surfaceNormal = surfaceNormal * 2 - 1;
	surfaceNormal = mul(surfaceNormal, input.tbn);

	float specularIntensity = SpecularTexture.Sample(WrapSampler, input.texCoord).r;

	float3 lightAccumulation = (float3)0;

	//[unroll]
	//for(unsigned int i = 0; i < MAX_LIGHTS; ++i)
	//{
		lightAccumulation += gLights/*[i]*/.CalculateLightAccumulation(input.worldPos, surfaceNormal, specularIntensity);
	//}

	return objectColor * float4(lightAccumulation, 1.0f);
}

#endif