#ifndef COLORPIXEL_HLSL
#define COLORPIXEL_HLSL

#include "ShaderDeclarations.hlsl"

float4 main(VERTCLR_OUT input) : SV_TARGET
{
	return input.color;
}

#endif