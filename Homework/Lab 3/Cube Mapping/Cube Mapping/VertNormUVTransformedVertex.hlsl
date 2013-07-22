#ifndef	VERTNORMUVTRANSFORMED_HLSL
#define VERTNORMUVTRANSFORMED_HLSL

#pragma pack_matrix(row_major)

#include "ShaderDeclarations.hlsl"

VERTNORMUV_OUT main(VERTNORMUV_IN input)
{
	VERTNORMUV_OUT output;

	output.position = mul(float4(input.position, 1.0f), MVPMatrix);

	output.normal = mul(float4(input.normal, 0.0f), WorldMatrix).xyz;

	output.texCoord = input.texCoord;

	return output;
}

#endif