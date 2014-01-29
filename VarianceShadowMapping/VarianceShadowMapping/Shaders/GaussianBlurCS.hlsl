#ifndef	GAUSSIANBLURCS_HLSL
#define GAUSSIANBLURCS_HLSL

#include "PostProcess.hlsl"

static const float3 gBlurWeights = float3(0.242, 0.061f, 0.006f);
static const float gCenterBlurWeights = 0.383f;

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID, uint GroupIndex : SV_GroupIndex)
{
	// 1920 x 1080 = 2073600
	// 2073600 / 1024 = 2025
	// 2025 / 45 = 45

	int2 initialIndex = GroupIndex * g_unPixelsPerThread;
	initialIndex.y /= g_unScreenWidth;
	initialIndex.x -= initialIndex.y * g_unScreenWidth;

	int2 currIndex = initialIndex;
	float4 pixelColor;
	uint i, neighborOffset;
	
	// Perform Horizontal Blur
	for(i = 0; i < g_unPixelsPerThread; ++i)
	{
		pixelColor = /*UnPackUint(*/SourceTexture.Load(int3(currIndex, 0));//);
		pixelColor *= gCenterBlurWeights;

		[unroll]
		for(neighborOffset = 1; neighborOffset < 4; ++neighborOffset)
		{
			pixelColor += SourceTexture.Load(int3(currIndex + int2(neighborOffset, 0), 0)) * gBlurWeights[neighborOffset - 1];//UnPackUint(SourceTexture[currIndex + uint2(neighborOffset, 0)]) * gBlurWeights[neighborOffset - 1];
			pixelColor += SourceTexture.Load(int3(currIndex - int2(neighborOffset, 0), 0)) * gBlurWeights[neighborOffset - 1];//UnPackUint(SourceTexture[currIndex - uint2(neighborOffset, 0)]) * gBlurWeights[neighborOffset - 1];
		}

		//TmpTexture[currIndex] = PackToUint(pixelColor);
		DestTexture[currIndex] = float4(pixelColor.rgb, 1);
		
		if(++currIndex.x == g_unScreenWidth)
		{
			currIndex.x = 0;
			++currIndex.y;
		}
	}

	//AllMemoryBarrierWithGroupSync();

	//currIndex = initialIndex;
	//// Perform Vertical Blur
	//for(i = 0; i < g_unPixelsPerThread; ++i, ++currIndex)
	//{
	//	pixelColor = UnPackUint(TmpTexture[currIndex];);
	//	pixelColor *= gCenterBlurWeights;

	//	for(neighborOffset = 1; neighborOffset < 4; ++neighborOffset)
	//	{
	//		pixelColor += UnPackUint(TmpTexture[currIndex + uint2(0, neighborOffset)]) * gBlurWeights[neighborOffset - 1];
	//		pixelColor += UnPackUint(TmpTexture[currIndex - uint2(0, neighborOffset)]) * gBlurWeights[neighborOffset - 1];
	//	}

	//	SourceTexture[currIndex] = PackToUint(pixelColor;);
	//}
}

#endif