#ifndef	TESSELLATIONVS_HLSL
#define TESSELLATIONVS_HLSL

#include "ShaderDeclarations.hlsl"

VERTNORMTANUV_TESS_VS_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUV_TESS_VS_OUT output;

	output.position = mul(float4(input.position, 1.0f), gWorldMatrix);

	output.tbn[2] = normalize(mul(input.normal, (float3x3)gWorldMatrix));
	output.tbn[0] = normalize(mul(input.tangent, (float3x3)gWorldMatrix));
	output.tbn[1] = cross(output.tbn[0], output.tbn[2]);



	output.texCoord = input.texCoord;

	return output;
}

#endif