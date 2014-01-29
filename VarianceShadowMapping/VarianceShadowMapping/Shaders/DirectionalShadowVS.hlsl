#ifndef DIRECTIONALSHADOWVS_HLSL
#define DIRECTIONALSHADOWVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

DIRVSM_OUT main(float3 position : POSITION)
{
	DIRVSM_OUT output;

	output.position = mul(float4(position, 1.0f), gMVPMatrix);

	float depth = mul(float4(position, 1.0f), gModelViewMatrix).z;

	output.depth = (depth - gDirLight.m_fNearClip) / (gDirLight.m_fFarClip - gDirLight.m_fNearClip);

	return output;
}

#endif