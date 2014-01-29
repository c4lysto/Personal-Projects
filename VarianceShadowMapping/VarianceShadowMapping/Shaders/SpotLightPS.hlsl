#ifndef	SPOTLIGHTPS_HLSL
#define SPOTLIGHTPS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

float4 main(VERTNORMTANUVSPOTSHADOW_OUT input) : SV_TARGET
{
	float4 objectColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);

	float3 surfaceNormal = NormalTexture.Sample(WrapSampler, input.texCoord).xyz;
	surfaceNormal = surfaceNormal * 2 - 1;
	surfaceNormal = normalize(mul(surfaceNormal, input.tbn));

	float specularIntensity = SpecularTexture.Sample(WrapSampler, input.texCoord).r;

	float shadowContribution = CalculateESMShadowContribution(ViewSpaceShadowMap.Sample(ClampSampler, input.shadowTexCoord.xy / input.shadowTexCoord.w).x,
									length(input.viewPos) / gSpotLight.m_fRadius);

	return shadowContribution * (objectColor * float4(gSpotLight.CalculateLightAccumulation(input.worldPos, surfaceNormal, specularIntensity), 1.0f));
}

#endif