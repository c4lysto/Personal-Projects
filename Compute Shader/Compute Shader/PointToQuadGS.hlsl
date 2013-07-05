#ifndef POINTTOQUADGS_HLSL
#define POINTTOQUADGS_HLSL

#include "ShaderDeclarations.hlsl"

struct GSOutput
{
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
	float4 position : SV_POSITION;
};

[maxvertexcount(6)]
void main(point VERTCLR_OUT input[1], inout TriangleStream< GSOutput > output)
{
	float4 inputPos = input[0].position;

	// if this vertex is in clip space
	if(abs(inputPos.x) <= inputPos.w ||
		abs(inputPos.y) <= inputPos.w ||
		abs(inputPos.z) <= inputPos.w)
	{
		GSOutput verts[4] = 
		{
			{float2(0, 0), input[0].color, float4(input[0].position.x - 0.1f, input[0].position.y + 0.1f, input[0].position.zw)},
			{float2(1, 0), input[0].color, float4(input[0].position.x + 0.1f, input[0].position.y + 0.1f, input[0].position.zw)},
			{float2(1, 1), input[0].color, float4(input[0].position.x + 0.1f, input[0].position.y - 0.1f, input[0].position.zw)},
			{float2(0, 1), input[0].color, float4(input[0].position.x - 0.1f, input[0].position.y - 0.1f, input[0].position.zw)}
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
}

#endif