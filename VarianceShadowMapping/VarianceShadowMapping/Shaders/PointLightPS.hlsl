#ifndef	POINTLIGHTPS_HLSL
#define POINTLIGHTPS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

//struct GSOutput
//{
//	float2 texCoord : TEXCOORD0;
//	float3 worldPos : TEXCOORD1;
//	float3 viewPos : TEXCOORD2;
//	float3x3 tbn : TEXCOORD3;
//	float4 position : SV_POSITION;
//	uint RTIndex : SV_RenderTargetArrayIndex;
//};

float4 main(VERTNORMTANUVPOINTSHADOW_OUT/*GSOutput*/ input) : SV_TARGET
{
	float4 objectColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);

	float3 surfaceNormal = NormalTexture.Sample(WrapSampler, input.texCoord).xyz;
	surfaceNormal = surfaceNormal * 2 - 1;
	surfaceNormal = normalize(mul(surfaceNormal, input.tbn));

	float specularIntensity = SpecularTexture.Sample(WrapSampler, input.texCoord).r;

	float shadowContribution = CalculateESMShadowContribution(PointLightShadowMap.Sample(ClampPointSampler, input.lightToPixel).x, length(input.lightToPixel) / gPointLight.m_fRadius);

	return shadowContribution * (objectColor * float4(gPointLight.CalculateLightAccumulation(input.worldPos, surfaceNormal, specularIntensity), 1.0f));
}

#endif