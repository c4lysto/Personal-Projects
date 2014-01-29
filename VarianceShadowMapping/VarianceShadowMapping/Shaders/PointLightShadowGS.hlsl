#ifndef	POINTLIGHTSHADOWGS_HLSL
#define POINTLIGHTSHADOWGS_HLSL

#include "Lights.hlsl"

struct GSOutput
{
	float4 viewPos : TEXCOORD0;
	float4 position : SV_POSITION;
	uint RTindex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle float4 input[3] : SV_POSITION, inout TriangleStream< GSOutput > output)
{
	GSOutput outVert;

	for(uint targetIndex = 0; targetIndex < 6; ++targetIndex)
	{
		outVert.RTindex = targetIndex;

		for (uint vertIndex = 0; vertIndex < 3; vertIndex++)
		{
			outVert.viewPos = (float4)1;//mul(input[vertIndex], gPointLight.m_ViewMatrices[targetIndex]);
			outVert.position = (float4)1;//mul(outVert.viewPos, gPointLight.m_ProjectionMatrix);
			output.Append(outVert);
		}
		output.RestartStrip();
	}
}

#endif