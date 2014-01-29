#ifndef	POINTLIGHTSHADOWPS_HLSL
#define POINTLIGHTSHADOWPS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

//struct PointLightShadowGSOutput
//{
//	float4 viewPos : TEXCOORD0;
//	float4 position : SV_POSITION;
//	uint RTindex : SV_RenderTargetArrayIndex;
//};

float4 main(VIEWVSM_OUT input/*PointLightShadowGSOutput input*/) : SV_TARGET
{
	float depth = exp(ESM_CONSTANT * (length(input.viewPos) / gPointLight.m_fRadius));

	float4 moments = (float4)1;
	moments.x = depth;

	float dx = ddx(depth);
	float dy = ddy(depth);

	moments.y = (depth * depth) + 0.25f * (dx * dx + dy * dy);

	return moments;
}

#endif