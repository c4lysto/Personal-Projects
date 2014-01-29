#ifndef HORIZONTALBLURPS_HLSL
#define HORIZONTALBLURPS_HLSL

#include "ShaderDeclarations.hlsl"

struct BlurVertexOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float2 positiveOffsets[3] : TEXCOORD1;
    float2 negativeOffsets[3] : TEXCOORD4;
};

static const float3 gBlurWeights = float3(0.242, 0.061f, 0.006f);
static const float gCenterBlurWeights = 0.383f;

float4 main(BlurVertexOut input) : SV_TARGET
{
	float4 color = DiffuseTexture.Sample(ClampSampler, input.texCoord) * gCenterBlurWeights;
	
	[unroll]
	for(int i = 0; i < 3; ++i)
	{
		color += DiffuseTexture.Sample(ClampSampler, input.positiveOffsets[i]) * gBlurWeights[i];
		color += DiffuseTexture.Sample(ClampSampler, input.negativeOffsets[i]) * gBlurWeights[i];
	}
	
	return color;
}

#endif