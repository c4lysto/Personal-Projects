#ifndef SPOTLIGHTSHADOWVS_HLSL
#define	SPOTLIGHTSHADOWVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

VIEWVSM_OUT main( float3 position : POSITION )
{
	VIEWVSM_OUT output;

	output.position = mul(float4(position, 1.0f), gMVPMatrix);

	output.viewPos = mul(float4(position, 1.0f), gModelViewMatrix).xyz;

	return output;
}

#endif //SPOTLIGHTSHADOWVS_HLSL