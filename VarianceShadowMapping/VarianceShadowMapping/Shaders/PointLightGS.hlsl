#ifndef	POINTLIGHTGS_HLSL
#define POINTLIGHTGS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

struct GSOutput
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 viewPos : TEXCOORD2;
	float3x3 tbn : TEXCOORD3;
	float4 position : SV_POSITION;
	uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle VERTNORMTANUVPOINTSHADOW_OUT input[3], inout TriangleStream< GSOutput > output)
{
	//GSOutput verts[3];

	//for(uint i = 0; i < 3; ++i)
	//{
	//	verts[i].texCoord = input[i].texCoord;
	//	verts[i].worldPos = input[i].worldPos;
	//	verts[i].tbn = input[i].tbn;
	//	verts[i].position = input[i].position;
	//}

	//for(uint targetIndex = 0; targetIndex < 6; ++targetIndex)
	//{
	//	for (uint vertIndex = 0; vertIndex < 3; ++vertIndex)
	//	{
	//		//verts[vertIndex].RTIndex = targetIndex;
	//		float4 modelView = mul(input[vertIndex].position, gPointLight.m_ViewMatrices[targetIndex]);
	//		verts[vertIndex].viewPos = modelView.xyz;
	//		verts[vertIndex].position = mul(modelView, gPointLight.m_ProjectionMatrix);
	//		output.Append(verts[vertIndex]);
	//	}
	//	output.RestartStrip();
	//}

	GSOutput outVert;

	for(uint targetIndex = 0; targetIndex < 6; ++targetIndex)
	{
		outVert.RTIndex = targetIndex;
		for (uint vertIndex = 0; vertIndex < 3; ++vertIndex)
		{
			outVert.texCoord = input[vertIndex].texCoord;
			outVert.worldPos = input[vertIndex].worldPos;
			outVert.tbn = input[vertIndex].tbn;
			outVert.RTIndex = targetIndex;
			outVert.viewPos = (float3)1;
			//outVert.viewPos = mul(input[vertIndex].position, gPointLight.m_ViewMatrices[targetIndex]).xyz;
			//outVert.viewPos = outVert.position.xyz;
			outVert.position = mul(input[vertIndex].position, gViewProjectionMatrix);
			output.Append(outVert);
		}
		output.RestartStrip();
	}
}

#endif