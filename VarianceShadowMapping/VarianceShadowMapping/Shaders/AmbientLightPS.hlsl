#ifndef	AMBIENTLIGHTPS_HLSL
#define AMBIENTLIGHTPS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

struct AMBIENT_OUT
{
	float4 ambientColor : SV_TARGET0;
	float4 godRayColor : SV_TARGET1;
};

//AMBIENT_OUT main(VERTNORMTANUV_OUT input)
//{
//	AMBIENT_OUT output;
//
//	output.ambientColor = DiffuseTexture.Sample(WrapSampler, input.texCoord) * float4(gAmbientLight.m_vColor, 1.0f);
//	output.godRayColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	return output;
//}

#define WIREFRAME_COLOR float4(0.0, 1.0f, 1.0f, 1.0f)

struct GSOutput
{
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3x3 tbn : TEXCOORD2;
	float3 distToEdges : TEXCOORD5;
	float4 position : SV_POSITION;
};

AMBIENT_OUT main(GSOutput input)
{
	AMBIENT_OUT output;

	float dist = min(input.distToEdges[0], min(input.distToEdges[1], input.distToEdges[2]));

	dist = max(dist - (gWireFrameWidth - 1), 0);
	dist *= dist;

	float I = exp2(-2 * dist);

	float3 vNormal = mul(NormalTexture.Sample(WrapSampler, input.texCoord).rgb * 2 - 1, input.tbn);
		float4 diffuseColor = DiffuseTexture.Sample(WrapSampler, input.texCoord);
		output.ambientColor = diffuseColor * float4(gAmbientLight.m_vColor, 1.0f);
		output.ambientColor += diffuseColor * float4(gDirLight.CalculateLightAccumulation(input.worldPos, vNormal, SpecularTexture.Sample(WrapSampler, input.texCoord).r), 1.0f);

		if(gRenderFlags & RF_SOLIDWIREFRAME)
			output.ambientColor = lerp(output.ambientColor, WIREFRAME_COLOR, I);

	output.godRayColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return output;
}

#endif