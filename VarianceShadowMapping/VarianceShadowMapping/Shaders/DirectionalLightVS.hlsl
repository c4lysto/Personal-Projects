#ifndef DIRECTIONALLIGHTVS_HLSL
#define DIRECTIONALLIGHTVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

VERTNORMTANUVDIRSHADOW_OUT main(VERTNORMTANUV_IN input)
{
	VERTNORMTANUVDIRSHADOW_OUT output;

	float4 worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
	output.worldPos = worldPosition.xyz;

	output.position = mul(worldPosition, gViewProjectionMatrix);

	output.tbn[2] = normalize(mul(input.normal, (float3x3)gWorldMatrix));
	output.tbn[0] = normalize(mul(input.tangent, (float3x3)gWorldMatrix));
	output.tbn[1] = cross(output.tbn[2], output.tbn[0]);

	output.shadowTexCoord = mul(worldPosition, gDirLight.m_ViewProjTextureMatrix);

	float depth = mul(worldPosition, gDirLight.m_ViewMatrix).z;
	output.depth = (depth - gDirLight.m_fNearClip) / (gDirLight.m_fFarClip - gDirLight.m_fNearClip);

	output.texCoord = input.texCoord;

	return output;
}

#endif