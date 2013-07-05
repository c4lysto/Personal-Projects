#ifndef VERTCLRTRANSFORMEDVERTEX_HLSL
#define VERTCLRTRANSFORMEDVERTEX_HLSL

#pragma pack_matrix(row_major)

#include "ShaderDeclarations.hlsl"

StructuredBuffer<ParticleData> particles : register(t0);

VERTCLR_OUT main(uint ID : SV_VertexID)
{
	VERTCLR_OUT output;

	output.position = mul(float4(particles[ID].position, 1.0f), MVPMatrix);

	output.color = particles[ID].color;

	return output;
}

#endif