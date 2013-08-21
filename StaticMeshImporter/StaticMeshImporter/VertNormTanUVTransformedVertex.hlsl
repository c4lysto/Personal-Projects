#ifndef VERTNORMTANUVTRANSFORMEDVERTEX_HLSL
#define VERTNORMTANUVTRANSFORMEDVERTEX_HLSL

#include "ShaderDeclarations.hlsl"

VERTNORMTANUV_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUV_OUT output;

	output.position = mul(float4(input.position, 1.0f), MVPMatrix);

	output.tbn[2] = mul(input.normal, (float3x3)WorldMatrix);
	output.tbn[0] = mul(input.tangent, (float3x3)WorldMatrix);
	output.tbn[1] = cross(output.tbn[0], output.tbn[2]);

	output.texCoord = input.texCoord;

	return output;
}

#endif