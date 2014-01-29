#ifndef POSTPROCESS_HLSL
#define POSTPROCESS_HLSL

cbuffer POSTPROCESS : register(b7)
{
	uint g_unScreenWidth;
	uint g_unScreenHeight;
	uint g_unPixelsPerThread;
	float g_fPostProcessFillData;
}

Texture2D SourceTexture : register(t0);
RWTexture2D/*RWStructuredBuffer*/<float4> DestTexture : register(u0);

int PackToUint(uint4 input)
{
	return uint((input.x & 0xFF) |
				((input.y & 0xFF) << 8) |
				((input.z & 0xFF) << 16) |
				((input.w & 0xFF) << 24));
}

int PackToUint(uint nR, uint nG, uint nB, uint nA)
{
	return uint((nR & 0xFF) |
				((nG & 0xFF) << 8) |
				((nB & 0xFF) << 16) |
				((nA & 0xFF) << 24));
}

void UnPackUint(in uint input, out uint nR, out uint nG, out uint nB, out uint nA)
{
	nR = (input & 0xFF);
	nG = ((input >> 8) & 0xFF);
	nB = ((input >> 16) & 0xFF);
	nA = ((input >> 24) & 0xFF);
}

uint4 UnPackUint(in uint input)
{
	return uint4((input & 0xFF), ((input >> 8) & 0xFF), ((input >> 16) & 0xFF), ((input >> 24) & 0xFF));
}

#endif