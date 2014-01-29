#ifndef PASSTHROUGHSCREENVS_HLSL
#define PASSTHROUGHSCREENVS_HLSL

#include "ShaderDeclarations.hlsl"

VERTUV main(VERTNORMTANUV_IN input)
{
	VERTUV output;

	output.position = float4(input.position, 1.0f);

	output.texCoord = input.texCoord + gHalfPixel;

	return output;
}

#endif