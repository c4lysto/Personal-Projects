#ifndef	CUBEMAPVERTEX_HLSL
#define CUBEMAPVERTEX_HLSL

#include "ShaderDeclarations.hlsl"

CUBEMAP_OUT main( float3 position : POSITION )
{
	CUBEMAP_OUT output;

	output.position = mul(float4(position + CameraPos, 1.0f), MVPMatrix);
	output.position.z = output.position.w;

	output.texCoord = position;

	return output;
}

#endif