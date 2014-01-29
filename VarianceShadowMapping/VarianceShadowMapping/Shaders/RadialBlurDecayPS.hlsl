#ifndef RADIALBLURDECAYPS_HLSL
#define RADIALBLURDECAYPS_HLSL

#include "ShaderDeclarations.hlsl"

cbuffer BLURFACTORS : register(b8)
{
	float2 gFocusPoint;
	float fDensity;
	float fDecay;
	float fWeight;
	float fExposure;
	float2 BLURFACTORFILLDATA;
}

#define USE_NVIDIAS_IMPLEMENTATION 1


#define NUM_SAMPLES 128
static const float2 focusPoint = { 0.5f, 0.5f };
static const float fBlurStart = 0.95f;
static const float fBlurFactor = 0.25f;
static const float fDenominator = float(NUM_SAMPLES - 1);

float4 main(VERTUV input) : SV_TARGET
{
#if USE_NVIDIAS_IMPLEMENTATION
	float2 currTexCoord = input.texCoord;
	float2 deltaTexCoord = currTexCoord - gFocusPoint;
	deltaTexCoord *= 1.0f / float(NUM_SAMPLES) * fDensity;
	float4 finalColor = DiffuseTexture.Sample(ClampSampler, currTexCoord);
	float illuminationDecay = 1.0f;
	float4 currSample;

	for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		currTexCoord -= deltaTexCoord;

		currSample = DiffuseTexture.Sample(WrapSampler, currTexCoord);

		currSample *= illuminationDecay * fWeight;

		finalColor += currSample;

		illuminationDecay *= fDecay;
	}

	finalColor *= fExposure;

	finalColor.a = 1;

	return finalColor;

#else
	float2 texCoord = input.texCoord - gFocusPoint;
	float4 finalColor = 0.0f;

	float scale;

	[unroll(NUM_SAMPLES)]for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		scale = fBlurStart + fBlurFactor * (i / fDenominator);
		finalColor += DiffuseTexture.Sample(WrapSampler, texCoord * scale + gFocusPoint);
	}

	finalColor /= NUM_SAMPLES;

	finalColor.a = 1.0f;

	return finalColor;
#endif
}

#endif