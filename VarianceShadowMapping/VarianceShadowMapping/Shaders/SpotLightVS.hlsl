#ifndef SPOTLIGHTVS_HLSL
#define SPOTLIGHTVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

VERTNORMTANUVSPOTSHADOW_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUVSPOTSHADOW_OUT output;

	float4 worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
	output.worldPos = worldPosition.xyz;

	output.position = mul(worldPosition, gViewProjectionMatrix);

	output.tbn[2] = normalize(mul(input.normal, (float3x3)gWorldMatrix));
	output.tbn[0] = normalize(mul(input.tangent, (float3x3)gWorldMatrix));
	output.tbn[1] = cross(output.tbn[2], output.tbn[0]);

	output.shadowTexCoord = mul(worldPosition, gSpotLight.m_ViewProjTextureMatrix);

	output.viewPos = worldPosition.xyz - gSpotLight.m_vPosition;//mul(worldPosition, gSpotLight.m_ViewMatrix).xyz;
	
	output.texCoord = input.texCoord;

	return output;
}

#endif