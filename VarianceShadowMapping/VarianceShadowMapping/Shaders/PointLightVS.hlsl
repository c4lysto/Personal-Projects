#ifndef POINTLIGHTVS_HLSL
#define POINTLIGHTVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

VERTNORMTANUVPOINTSHADOW_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUVPOINTSHADOW_OUT output;

	//output.position = mul(float4(input.position, 1.0f), gWorldMatrix);
	//output.worldPos = output.position.xyz;

	float4 worldPos = mul(float4(input.position, 1.0f), gWorldMatrix);
	output.worldPos = worldPos.xyz;

	output.position = mul(worldPos, gViewProjectionMatrix);

	output.lightToPixel = worldPos.xyz - gPointLight.m_vPosition;

	output.tbn[2] = normalize(mul(input.normal, (float3x3)gWorldMatrix));
	output.tbn[0] = normalize(mul(input.tangent, (float3x3)gWorldMatrix));
	output.tbn[1] = cross(output.tbn[2], output.tbn[0]);
	
	output.texCoord = input.texCoord;

	return output;
}

#endif