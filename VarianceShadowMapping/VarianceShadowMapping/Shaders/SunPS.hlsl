#ifndef SUNPS_HLSL
#define SUNPS_HLSL

#include "ShaderDeclarations.hlsl"

struct SUNPS_OUT
{
	float4 mainColor : SV_TARGET0;
	float4 godRayColor : SV_TARGET1;
};

SUNPS_OUT main(VERTUV input)
{
	SUNPS_OUT output;

	output.mainColor = DiffuseTexture.Sample(ClampSampler, input.texCoord);
	output.godRayColor = output.mainColor;

	return output;
}

#endif