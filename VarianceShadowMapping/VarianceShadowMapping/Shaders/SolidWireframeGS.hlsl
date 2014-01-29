#ifndef SOLIDWIREFRAMEGS_HLSL
#define SOLIDWIREFRAMEGS_HLSL

#include "ShaderDeclarations.hlsl"

struct GSOutput
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3x3 tbn : TEXCOORD2;
	float3 distToEdges : TEXCOORD5;
	float4 position : SV_POSITION;
};

[maxvertexcount(3)]
void main(triangle VERTNORMTANUV_TESS_DS_OUT/*VERTNORMTANUV_OUT*/ input[3], inout TriangleStream< GSOutput > output)
{
	float2 vert1 = g_unScreenWidth * (input[0].position.xy / input[0].position.w);
	float2 vert2 = g_unScreenWidth * (input[1].position.xy / input[1].position.w);
	float2 vert3 = g_unScreenWidth * (input[2].position.xy / input[2].position.w);

	float2 edge1 = vert3 - vert2;
	float2 edge2 = vert3 - vert1;
	float2 edge3 = vert2 - vert1;
	
	float area = abs(edge2.x * edge3.y - edge2.y * edge3.x);

	float3 distToEdges = float3(area / length(edge1), area / length(edge2), area / length(edge3));

	GSOutput vertex;
	[unroll]
	for(uint i = 0; i < 3; ++i)
	{
		vertex.distToEdges = (float3)0;

		vertex.texCoord = input[i].texCoord;
		vertex.worldPos = input[i].worldPos;
		vertex.tbn = input[i].tbn;
		//vertex.normal = input[i].normal;
		vertex.distToEdges[i] = distToEdges[i];
		vertex.position = input[i].position;

		output.Append(vertex);
	}
}

#endif