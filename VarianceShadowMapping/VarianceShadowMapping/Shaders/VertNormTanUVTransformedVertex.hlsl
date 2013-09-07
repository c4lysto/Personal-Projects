#ifndef VERTNORMTANUVTRANSFORMEDVERTEX_HLSL
#define VERTNORMTANUVTRANSFORMEDVERTEX_HLSL

#include "ShaderDeclarations.hlsl"

VERTNORMTANUV_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUV_OUT output;

	float4 worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
	output.worldPos = worldPosition.xyz;

	output.position = mul(worldPosition, gViewProjectionMatrix);

	output.tbn[2] = normalize(mul(input.normal, (float3x3)gWorldMatrix));
	output.tbn[0] = normalize(mul(input.tangent, (float3x3)gWorldMatrix));
	output.tbn[1] = cross(output.tbn[0], output.tbn[2]);

	output.texCoord = input.texCoord;

	return output;
}

#endif