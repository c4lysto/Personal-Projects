#ifndef POINTTOQUADGS_HLSL
#define POINTTOQUADGS_HLSL

#include "ShaderDeclarations.hlsl"

[maxvertexcount(6)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<VERTUV> output)
{
	VERTUV verts[4] =
	{
																	// 1.7 ~= 16:9 aspect ratio
		{ float2(0, 0), float4(input[0].x - 0.1f, input[0].y + 0.1f * 1.7777777f, input[0].zw) },
		{ float2(1, 0), float4(input[0].x + 0.1f, input[0].y + 0.1f * 1.7777777f, input[0].zw) },
		{ float2(1, 1), float4(input[0].x + 0.1f, input[0].y - 0.1f * 1.7777777f, input[0].zw) },
		{ float2(0, 1), float4(input[0].x - 0.1f, input[0].y - 0.1f * 1.7777777f, input[0].zw) }
	};

	// Triangle One
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);

	output.RestartStrip();

	// Triangle Two
	output.Append(verts[0]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}

#endif