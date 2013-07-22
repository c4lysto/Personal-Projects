#include "sharedDefines.h"

// simple pixel shader, inputs an interpolated vertex color and outputs it to the screen
float4 ps_main(float4 icolor : COLOR) : SV_TARGET
{
	return icolor;
}