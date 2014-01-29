#ifndef POINTLIGHTSHADOWVS_HLSL
#define POINTLIGHTSHADOWVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

//float4 main( float3 pos : POSITION ) : SV_POSITION
//{
//	return mul(float4(pos, 1.0f), gWorldMatrix);
//}

VIEWVSM_OUT main(float3 position : POSITION)
{
	VIEWVSM_OUT output;

	output.position = mul(float4(position, 1.0f), gMVPMatrix);

	output.viewPos = mul(float4(position, 1.0f), gModelViewMatrix).xyz;

	return output;
}

#endif //POINTLIGHTSHADOWVS_HLSL