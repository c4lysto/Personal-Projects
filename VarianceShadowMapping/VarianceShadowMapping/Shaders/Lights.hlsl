#ifndef LIGHTS_HLSL
#define LIGHTS_HLSL

#include "ShaderDeclarations.hlsl"

interface IBaseLight
{
	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity);

	float4 CalculateMVPPos(float3 position);
	float4 CalculateViewPos(float3 position);
};

class AmbientLight : IBaseLight
{
	float3 m_vColor;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity) {return m_vColor;}

	float4 CalculateMVPPos(float3 position) {return (float4)0;}
	float4 CalculateViewPos(float3 position) {return (float4)0;}
};

class PointLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vPosition;
	float m_fRadius;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity)
	{
		float3 lightAccumulation = (float3)0;

		float3 toLight = m_vPosition - vPosition;
		float toLightLength = length(toLight);
		toLight = normalize(toLight);

		if(toLightLength < m_fRadius)
		{
			float NdotL = saturate(dot(vNormal, toLight));

									// apply linear attenuation (pow(linearAttenuation, 2) makes it quadratic)
			float attenuation = pow(saturate(1.0f - toLightLength / m_fRadius), 2);

			// Diffuse Calculation
			lightAccumulation += m_vColor * (NdotL * attenuation);

			// Calculate Specular Color
			float3 reflectionVec = normalize(reflect(-toLight, vNormal));
			float3 directionToCam = normalize(gCameraPos - vPosition);

			lightAccumulation += m_vColor * (pow(saturate(dot(reflectionVec, directionToCam)), m_fSpecularPower) * attenuation);
		}

		return lightAccumulation;
	}
};

class SpotLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vPosition;
	float m_fRadius;
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
			lightAccumulation += m_vColor * (NdotL * radiusAttenuation * coneAttenuation);

			float3 reflectionVec = normalize(reflect(-toLight, vNormal));
			float3 directionToCam = normalize(gCameraPos - vPosition);

			lightAccumulation += m_vColor * ((pow(saturate(dot(reflectionVec, directionToCam)), m_fSpecularPower) * radiusAttenuation * coneAttenuation) * fSpecIntensity);
		}

		return lightAccumulation;
	}
};

class DirectionalLight : AmbientLight
{
	float m_fSpecularPower;
	float3 m_vDirection;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal, float fSpecIntensity)
	{
		float3 lightAccumulation = (float3)0;

		// Diffuse Calculation
		float NdotL = saturate(dot(-m_vDirection, vNormal));
		lightAccumulation += m_vColor * NdotL;

		// Specular Calculation
		float3 reflectionVec = normalize(reflect(m_vDirection, vNormal));
		float3 directionToCam = normalize(gCameraPos - vPosition);
		lightAccumulation += m_vColor * pow(max(0.0f, dot(reflectionVec, directionToCam)), m_fSpecularPower);

		return lightAccumulation;
	}
};

cbuffer POINTLIGHT : register(b3)
{
	PointLight gPointLight;
	float3 ptLightFillData;
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

struct VSM_OUT
{
	float4 viewPos : TEXCOORD0;
	float4 position : SV_POSITION;
};

#endif