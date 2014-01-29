#ifndef SKYDOMEVS_HLSL
#define SKYDOMEVS_HLSL

#include "ShaderDeclarations.hlsl"

VERTUV main( VERTNORMTANUV_IN input )
{
	VERTUV output;

	output.position = mul(float4(input.position + gCameraPos, 1.0f), gViewProjectionMatrix);
	output.position.z = output.position.w;

	// Converts the Sphere V Coordinates:
	// TopV-1.0f			TopV-0.0f			
	// MidV-0.5f	to		MidV-1.0f
	// BotV-0.0f			BotV-2.0f
	// To allow for full coverage of the SkyDome texture over half of the SkyDome
	// Since I'm not experienced at all with my UV mapping this will have to do...
	output.texCoord = float2(input.texCoord.x, 2 - input.texCoord.y * 2);
	//output.texCoord = float2(input.texCoord.x, 1 - input.texCoord.y);

	return output;
}

#endif