#ifndef DYNAMICTESSDISTHS_HLSL
#define DYNAMICTESSDISTHS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Tessellation.hlsl"

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT DynamicDistPatchConstant(InputPatch<VERTNORMTANUV_TESS_VS_OUT, 3> input, uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT output;

	// If the triangle is facing the camera
	if(!IsForwardFacingTriangle(input[0].tbn[2], input[1].tbn[2], input[2].tbn[2], input[0].position.xyz - gCameraPos, BACKFACE_CULL_THRESHHOLD))
	{
		float3 vert1 = input[0].position.xyz;
		float3 vert2 = input[1].position.xyz;
		float3 vert3 = input[2].position.xyz;

		// In this particular implementation the size are as follows:		v1
		// side1 == vert2 <-> vert3											/\
		// side2 == vert1 <-> vert3						ex:			    s2 /  \ s3
		// side3 == vert1 <-> vert2										  /____\
		//															   v3   s1   v2	

		if(gRenderFlags & RF_DISTTESS) // Distance Tessellation
		{		
			precise float3 vEdge1MidPoint = (vert2 + vert3) * 0.5f;
			precise float3 vEdge2MidPoint = (vert1 + vert3) * 0.5f;
			precise float3 vEdge3MidPoint = (vert1 + vert2) * 0.5f;

			output.EdgeTessFactor[0] = CalculateDistTessFactor(gMinTessFactor, gMaxTessFactor, distance(vEdge1MidPoint, gCameraPos), gTessMinDist, gTessMaxDist);
			output.EdgeTessFactor[1] = CalculateDistTessFactor(gMinTessFactor, gMaxTessFactor, distance(vEdge2MidPoint, gCameraPos), gTessMinDist, gTessMaxDist);
			output.EdgeTessFactor[2] = CalculateDistTessFactor(gMinTessFactor, gMaxTessFactor, distance(vEdge3MidPoint, gCameraPos), gTessMinDist, gTessMaxDist);
		}
		else if(gRenderFlags & RF_SSTESS) // Screen Space Tessellation
		{
			output.EdgeTessFactor[0] = CalculateScreenSpaceTessFactor(vert2, vert3, gViewProjectionMatrix, gPixelsPerScreenHeight);
			output.EdgeTessFactor[1] = CalculateScreenSpaceTessFactor(vert1, vert3, gViewProjectionMatrix, gPixelsPerScreenHeight);
			output.EdgeTessFactor[2] = CalculateScreenSpaceTessFactor(vert1, vert2, gViewProjectionMatrix, gPixelsPerScreenHeight);
		}
		else
		{
			output.EdgeTessFactor[0] =
			output.EdgeTessFactor[1] =
			output.EdgeTessFactor[2] = 1;
		}

		output.InsideTessFactor = (output.EdgeTessFactor[0] + output.EdgeTessFactor[1] + output.EdgeTessFactor[2]) * 0.33333333f;
	}
	else // cull this patch and leave
	{
		output.EdgeTessFactor[0] =
		output.EdgeTessFactor[1] =
		output.EdgeTessFactor[2] =
		output.InsideTessFactor = 0.0f;

		return output;
	}

	return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("DynamicDistPatchConstant")]
VERTNORMTANUV_TESS_VS_OUT main(InputPatch<VERTNORMTANUV_TESS_VS_OUT, 3> input, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
	VERTNORMTANUV_TESS_VS_OUT output;

	// Insert code to compute Output here
	output.texCoord = input[i].texCoord;
	output.tbn = input[i].tbn;
	//output.normal = input[i].normal;
	//output.tangent = input[i].tangent;
	output.position = input[i].position;

	return output;
}

#endif