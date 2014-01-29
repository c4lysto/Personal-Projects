#ifndef	DYNAMICTESSDISTDS_HLSL
#define DYNAMICTESSDISTDS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Tessellation.hlsl"

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

#define MAX_HEIGHT 0.25f

[domain("tri")]
VERTNORMTANUV_TESS_DS_OUT main(HS_CONSTANT_DATA_OUTPUT patchConst, float3 uvw : SV_DomainLocation, const OutputPatch<VERTNORMTANUV_TESS_VS_OUT, 3> input)
{
	VERTNORMTANUV_TESS_DS_OUT output;

	output.worldPos = input[0].position.xyz * uvw.x +
					  input[1].position.xyz * uvw.y +
					  input[2].position.xyz * uvw.z;

	output.texCoord = input[0].texCoord * uvw.x +
					  input[1].texCoord * uvw.y +
					  input[2].texCoord * uvw.z;

	/*float3 vNormal = input[0].normal * uvw.x +
					 input[1].normal * uvw.y +
					 input[2].normal * uvw.z;
	vNormal = normalize(vNormal);

	float3 vTangent = input[0].tangent * uvw.x +
					  input[1].tangent * uvw.y +
					  input[2].tangent * uvw.z;
	vTangent = normalize(vTangent);

	output.tbn = float3x3(vTangent, cross(vTangent, vNormal), vNormal);*/

	[unroll]
	for(int i = 0; i < 3; ++i)
	{
		output.tbn[i] = input[0].tbn[i] * uvw.x +
						input[1].tbn[i] * uvw.y +
						input[2].tbn[i] * uvw.z;
	}

	//output.normal = mul(NormalTexture.SampleLevel(WrapSampler, output.texCoord, 0).rgb, tbn);

	float fEdgeTessFactor = CalculateDistTessFactor(1, 60, length(output.worldPos - gCameraPos), 1, 25);
	float sampledHeight = DisplacementTexture.SampleLevel(WrapSampler, output.texCoord, CalculateDistMipLevel(fEdgeTessFactor)).r;

	if((gRenderFlags & (RF_DISTTESS | RF_SSTESS)))
	{
		// lerp the height base on the tessellation factor so that we get a smooth transition
		//float distLerp = 1 - saturate((distance(output.worldPos, gCameraPos) - gTessMinDist) / gTessDistRange);
		float tessLerp = saturate((fEdgeTessFactor - gMinTessFactor) / gTessFactorRange);
		float lerpHeight = tessLerp; //(distLerp + tessLerp) * 0.5f;

		output.worldPos += output.tbn[2] * (MAX_HEIGHT * sampledHeight * lerpHeight);
	}

	output.position = mul(float4(output.worldPos, 1), gViewProjectionMatrix);

	return output;
}

#endif