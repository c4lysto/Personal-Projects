#ifndef LIGHTS_HLSL
#define LIGHTS_HLSL

#include "ShaderDeclarations.hlsl"

interface IBaseLight
{
	float3 CalculateDiffuseColor(float3 vPosition, float3 vNormal);

	float3 CalculateSpecularColor(float3 vPosition, float3 vNormal);

	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal);
};

class AmbientLight : IBaseLight
{
	float3 m_vColor;


	float3 CalculateDiffuseColor(float3 vPosition, float3 vNormal) {return (float3)0;}

	float3 CalculateSpecularColor(float3 vPosition, float3 vNormal) {return (float3)0;}

	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal) {return m_vColor;}
};

class PointLight : AmbientLight
{
	float m_fSpecularPower;
	float m_fSpecularIntensity;
	float3 m_vPosition;
	float m_fRadius;


	float3 CalculateDiffuseColor(float3 vPosition, float3 vNormal)
	{
		float3 toLight = m_vPosition - vPosition;
		float NdotL = saturate(dot(vNormal, normalize(toLight)));

		// apply linear attenuation
		float attenuation = saturate(1.0f - length(toLight) / m_fRadius);

									// make quadratic
		return m_vColor * (NdotL * (attenuation * attenuation));
	}

	float3 CalculateSpecularColor(float3 vPosition, float3 vNormal)
	{
									// Pixel to Cam							// Pixel to Light
		//float3 halfAngle = normalize(normalize(gCameraPos - vPosition) + normalize(m_vPosition - vPosition));
		//return pow(max(0, dot(halfAngle, vNormal)), m_fSpecularPower) * m_vColor;

		float3 toPixel = normalize(vPosition - m_vPosition);
									
		float3 reflectionVec = normalize(reflect(toPixel, vNormal));
		float3 directionToCam = normalize(gCameraPos - vPosition);

		float3 toLight = m_vPosition - vPosition;
		float attenuation = saturate(1.0f - length(toLight) / m_fRadius);
	
		return m_vColor * pow(saturate(dot(reflectionVec, directionToCam)), m_fSpecularPower) * (attenuation * attenuation);
	}

	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal)
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
	float m_fSpecularIntensity;
	float3 m_vPosition;
	float3 m_vDirection;
	float m_fInnerConeRatio;
	float m_fOuterConeRatio;
	float m_fRadius;
	float2 m_fSpotLightFillData;


	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal)
	{
		float3 lightAccumulation = (float3)0;

		float3 toLight = m_vPosition - vPosition;
		float toLightLength = length(toLight);
		toLight = normalize(toLight);
		float surfaceRatio = dot(-toLight, m_vDirection);

		// check to see if pixel is within the outer cone of the spotlight
		if(surfaceRatio > m_fOuterConeRatio)
		{		
			float radiusAttenuation = pow(saturate(1.0f - toLightLength / m_fRadius), 2);
											// apply linear attenuation (pow(linearAttenuation, 2) makes it quadratic)
			float coneAttenuation = pow(1.0f - saturate((m_fInnerConeRatio - surfaceRatio) / (m_fInnerConeRatio - m_fOuterConeRatio)), 2);

			float NdotL = saturate(dot(vNormal, toLight));

			// Diffuse
			lightAccumulation += m_vColor * (NdotL * radiusAttenuation * coneAttenuation);

			float3 reflectionVec = normalize(reflect(-toLight, vNormal));
			float3 directionToCam = normalize(gCameraPos - vPosition);

			lightAccumulation += m_vColor * (pow(saturate(dot(reflectionVec, directionToCam)), m_fSpecularPower) * radiusAttenuation * coneAttenuation);
		}

		return lightAccumulation;
	}
};

class DirectionalLight : AmbientLight
{
	float m_fSpecularPower;
	float m_fSpecularIntensity;
	float3 m_vDirection;


	float3 CalculateDiffuseColor(float3 vPosition, float3 vNormal)
	{
		float NdotL = saturate(dot(-m_vDirection, vNormal));
		return m_vColor * NdotL;
	}

	float3 CalculateSpecularColor(float3 vPosition, float3 vNormal)
	{
		float3 reflectionVec = normalize(reflect(m_vDirection, vNormal));
		float3 directionToCam = normalize(gCameraPos - vPosition);
		return m_vColor * pow(max(0.0f, dot(reflectionVec, directionToCam)), m_fSpecularPower);
		//return pow(max(0, dot(halfAngle, vNormal)), m_fSpecularPower) * m_vColor;
	}

	float3 CalculateLightAccumulation(float3 vPosition, float3 vNormal)
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

#endif