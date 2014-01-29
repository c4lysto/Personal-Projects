#ifndef DIRECTIONALSHADOWPS_HLSL
#define DIRECTIONALSHADOWPS_HLSL

#include "Lights.hlsl"

float4 main(DIRVSM_OUT input) : SV_TARGET
{
	float depth = exp(ESM_CONSTANT * input.depth);

	float4 moments = (float4)1;
	moments.x = depth;

	float dx = ddx(depth);
	float dy = ddy(depth);

	moments.y = (depth * depth) + 0.25f * (dx * dx + dy * dy);

	return moments;
}

#endif