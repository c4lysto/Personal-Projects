#ifndef LIGHTS_HLSL
#define LIGHTS_HLSL

#include "ShaderDeclarations.hlsl"

#define MAX_LIGHTS 4
#define MIN_VARIANCE 0.000001f
#define MIN_THRESHOLD 0.2f
#define ESM_CONSTANT 88.0f

interface IBaseLight
{
	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity);

	float4 CalculateMVPPos(float3 position);
	float CalculateLightDepth(float3 position);
};

class AmbientLight : IBaseLight
{
	float3 m_vColor;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity) {return m_vColor;}

	float4 CalculateMVPPos(float3 position) {return (float4)0;}
	float CalculateLightDepth(float3 position) {return 0.0f;}
};

class PointLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vPosition;
	float m_fRadius;
	//float4x4 m_ViewMatrices[6];
	//float4x4 m_ProjectionMatrix;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity)
	{
		float3 lightAccumulation = (float3)0;

		float3 toLight = m_vPosition - vPosition;
		float toLightLength = length(toLight);

		if(toLightLength < m_fRadius)
		{
			toLight = normalize(toLight);

			float NdotL = saturate(dot(vNormal, toLight));

									// apply linear attenuation (pow(linearAttenuation, 2) makes it quadratic)
			float attenuation = pow(saturate(1.0f - toLightLength / m_fRadius), 2);

			// Diffuse Calculation
			lightAccumulation += (NdotL * attenuation);

			// Calculate Specular Color
			float3 directionToCam = normalize(gCameraPos - vPosition);
			float3 halfVec = normalize(directionToCam + toLight);

			lightAccumulation += pow(max(0, dot(halfVec, vNormal)), m_fSpecularPower) * attenuation * fSpecIntensity;

			lightAccumulation *= m_vColor;
		}

		return lightAccumulation;
	}
};

class SpotLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vPosition;
	float m_fRadius;
	float4x4 m_ViewProjTextureMatrix;
	float4x4 m_ViewMatrix;
	float3 m_vDirection;
	float m_fInnerConeRatio;
	float m_fOuterConeRatio;
	float2 m_fSpotLightFillData;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity)
	{
		float3 lightAccumulation = (float3)0;

		float3 toLight = m_vPosition - vPosition;
		float toLightLength = length(toLight);
		toLight = normalize(toLight);
		float surfaceRatio = dot(-toLight, m_vDirection);

		// check to see if pixel is within the outer cone of the spotlight
		if(surfaceRatio > m_fOuterConeRatio || toLightLength > m_fRadius)
		{		
			float radiusAttenuation = pow(saturate(1.0f - toLightLength / m_fRadius), 2);
											// apply linear attenuation (pow(linearAttenuation, 2) makes it quadratic)
			float coneAttenuation = pow(1.0f - saturate((m_fInnerConeRatio - surfaceRatio) / (m_fInnerConeRatio - m_fOuterConeRatio)), 2);

			float NdotL = saturate(dot(vNormal, toLight));

			// Diffuse
			lightAccumulation += (NdotL * radiusAttenuation * coneAttenuation);

			float3 directionToCam = normalize(gCameraPos - vPosition);
			float3 halfVec = normalize(directionToCam + toLight);

			lightAccumulation += pow(max(0, dot(halfVec, vNormal)), m_fSpecularPower) * radiusAttenuation * coneAttenuation * fSpecIntensity;

			lightAccumulation *= m_vColor;
		}

		return lightAccumulation;
	}
};

class DirectionalLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vDirection;
	float m_fNearClip;
	float4x4 m_ViewProjTextureMatrix;
	float4x4 m_ViewMatrix;
	float m_fFarClip;
	float3 DirLightFillData;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity)
	{
		float3 lightAccumulation = (float3)0;

		// Diffuse Calculation
		float NdotL = saturate(dot(-m_vDirection, vNormal));
		lightAccumulation += NdotL;

		// Specular Calculation
		float3 directionToCam = normalize(gCameraPos - vPosition);
		float3 halfVec = normalize(directionToCam - m_vDirection);

		lightAccumulation += (pow(max(0, dot(halfVec, vNormal)), m_fSpecularPower) * fSpecIntensity);

		lightAccumulation *= m_vColor;

		return lightAccumulation;
	}

	float4 CalculatePosition(float3 position)
	{
		return mul(float4(position, 1.0f), gMVPMatrix);
	}

	float CalculateLightDepth(float3 position)
	{
		float depth = mul(float4(position, 1.0f), gModelViewMatrix).z;

		return (depth - m_fNearClip) / (m_fFarClip - m_fNearClip);
	}
};

float CalculateVSMShadowContribution(float2 fMoments, float fDepth)
{
	if(fDepth <= fMoments.x)
	{
		return 1.0f;
	}
	else
	{
		float variance = max(fMoments.y - (fMoments.x * fMoments.x), MIN_VARIANCE);

		float d = fDepth - fMoments.x;

				// p_max
		float p_max = variance / (variance + d*d);

		return pow(p_max, 4);//saturate((p_max - MIN_THRESHOLD) / (1.0f - MIN_THRESHOLD));
	}
}

float CalculateESMShadowContribution(float fSampledDepth, float fDepth)
{
	float depthExp = exp(-ESM_CONSTANT * fDepth);
	return saturate(depthExp * fSampledDepth);
}

cbuffer POINTLIGHT : register(b3)
{
	PointLight gPointLight;
};

cbuffer SPOTLIGHT : register(b4)
{
	SpotLight gSpotLight;
};

cbuffer DIRLIGHT : register(b5)
{
	DirectionalLight gDirLight;
};

cbuffer AMBIENTLIGHT : register(b6)
{
	AmbientLight gAmbientLight;
	float fAmbientFillData;
}

struct VSM_IN
{
	float3 position : POSITION;
};

struct DIRVSM_OUT
{
	float depth : TEXCOORD0;
	float4 position : SV_POSITION;
};

struct VIEWVSM_OUT
{
	float3 viewPos : TEXCOORD0;
	float4 position : SV_POSITION;
};

#endif