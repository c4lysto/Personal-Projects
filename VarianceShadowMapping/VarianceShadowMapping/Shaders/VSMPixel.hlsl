#ifndef VSMPIXEL_HLSL
#define VSMPIXEL_HLSL

#include "Lights.hlsl"

float4 main(VIEWVSM_OUT input) : SV_TARGET
{
	float depth = length(input.viewPos);

	float4 moments = (float4)1;
	moments.x = depth;

	float dx = ddx(depth);
	float dy = ddy(depth);

	moments.y = (depth * depth) + 0.25f * (dx * dx + dy * dy);

	return moments;
}

#endif