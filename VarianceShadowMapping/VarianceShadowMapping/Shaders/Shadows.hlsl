#ifndef SHADOWS_HLSL
#define SHADOWS_HLSL

interface IBaseShadow
{
	float CalculateShadowContribution(float fOccluderDepth, float fReceiverDepth);
};

class ShadowMap : IBaseShadow
{
	float CalculateShadowContribution(float fOccluderDepth, float fReceiverDepth) {return (fReceiverDepth < fOccluderDepth) ? 1.0f : 0.0f;}
};

class VarianceShadowMap : IBaseShadow
{
};

#endif