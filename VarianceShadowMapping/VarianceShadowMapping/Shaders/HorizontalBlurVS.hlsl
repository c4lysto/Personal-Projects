#ifndef HORIZONTALBLURVS_HLSL
#define HORIZONTALBLURVS_HLSL

#include "ShaderDeclarations.hlsl"

struct BlurVertexOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float2 positiveOffsets[3] : TEXCOORD1;
    float2 negativeOffsets[3] : TEXCOORD4;
};

BlurVertexOut main(VERTNORMTANUV_IN input)
{
	BlurVertexOut output;

	output.position = float4(input.position, 1.0f);
	
	output.texCoord = input.texCoord;// + gHalfPixel;

	output.positiveOffsets[0] = output.texCoord + float2(gHalfPixel.x * 2, 0.0f);		
	output.negativeOffsets[0] = output.texCoord - float2(gHalfPixel.x * 2, 0.0f);
	
	output.positiveOffsets[1] = output.texCoord + float2(gHalfPixel.x * 4, 0.0f);		
	output.negativeOffsets[1] = output.texCoord - float2(gHalfPixel.x * 4, 0.0f);
	
	output.positiveOffsets[2] = output.texCoord + float2(gHalfPixel.x * 6, 0.0f);		
	output.negativeOffsets[2] = output.texCoord - float2(gHalfPixel.x * 6, 0.0f);

	return output;
}

#endif