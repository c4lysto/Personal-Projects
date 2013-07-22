#ifndef VERTCLRTRANSFORMEDVERTEX_HLSL
#define VERTCLRTRANSFORMEDVERTEX_HLSL

#pragma pack_matrix(row_major)

#include "ShaderDeclarations.hlsl"

VERTCLR_OUT main(VERTCLR_IN input)
{
	VERTCLR_OUT output;

	output.position = mul(float4(input.position, 1.0f), MVPMatrix);

	output.color = input.color;

	return output;
}

#endif