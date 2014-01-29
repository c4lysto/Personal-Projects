#ifndef VSMVERTEX_HLSL
#define VSMVERTEX_HLSL

#include "Lights.hlsl"

IBaseLight gLight;

VIEWVSM_OUT main(VSM_IN input)
{
	VIEWVSM_OUT output = (VIEWVSM_OUT)0;

	//output.position = gLight.CalculateMVPPos(input.position);
	//output.viewPos = mul(float4(input.position, 1.0f), gModelViewMatrix);

	return output;
}

#endif