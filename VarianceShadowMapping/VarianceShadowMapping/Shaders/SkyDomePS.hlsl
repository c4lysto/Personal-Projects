#ifndef SKYDOMEPS_HLSL
#define SKYDOMEPS_HLSL

#include "ShaderDeclarations.hlsl"

//float3 CalculateFogColor(float3 pixel_to_light_vector, float3 pixel_to_eye_vector)
//{
//	return lerp(g_AtmosphereDarkColor,g_AtmosphereBrightColor,0.5*dot(pixel_to_light_vector,-pixel_to_eye_vector)+0.5);
//}

float4 main(VERTUV input) : SV_TARGET
{
	float2 sampleTexCoord = float2(input.texCoord.x, min(1.0f, input.texCoord.y));

	float4 color = float4(DiffuseTexture.Sample(WrapSampler, sampleTexCoord).xyz, 1.0f);

	/*sampleTexCoord.x += 0.5f;

	float4 oppColor = DiffuseTexture.Sample(WrapSampler, sampleTexCoord);*/

	//color = lerp(color, float4(0.0f, 0.0f, 0.0f, 1.0f), (input.texCoord.y - 1) * 2);

	return color;



	//float3 acolor = CalculateFogColor(pixel_to_light_vector,pixel_to_eye_vector);
	//color.rgb = lerp(color.rgb,acolor,pow(saturate(input.texcoord.y),10));
	//color.a = 1.0f;

	//return color;
}

#endif