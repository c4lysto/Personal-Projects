#ifndef SUNVS_HLSL
#define	SUNVS_HLSL

#include "ShaderDeclarations.hlsl"
#include "Lights.hlsl"

float4 main(/*VERTNORMTANUV_IN input*/) : SV_POSITION
{
	//VERTUV output;

	//output.position = float4(input.position + gCameraPos - gDirLight.m_vDirection, 1.0f);

	//output.position = mul(output.position, gViewProjectionMatrix);
	////output.position.z = output.position.w;

	//output.texCoord = input.texCoord;

	//return output;

	float4 retVal = mul(float4(gCameraPos - gDirLight.m_vDirection, 1.0f), gViewProjectionMatrix);
	retVal.z = retVal.w;

	return retVal;
}

#endif