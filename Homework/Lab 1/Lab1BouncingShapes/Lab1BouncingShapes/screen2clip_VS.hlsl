#include "SharedDefines.h"

// declare any variables/structures and buffers specific to this vertex shader only

// output from the vertex shader
struct VS_OUT
{
	float4 color : COLOR;
	float4 clipPos : SV_POSITION; // leave this last
};

// vertex shader transforms a screen space shape into clip space
VS_OUT vs_main( SCREEN_VERTEX input ) 
{
	VS_OUT output;
	// alter screen location via matrix 
	float4 screen = mul(float4(input.pos,1.0f), allShapes[input.index].shape_matrix);
	// shift window space coordinates to clip space
	output.clipPos = float4((screen.x / theScene.backbuffer_dimensions.x) * 2.0f - 1.0f, 
							1.0f - (screen.y / theScene.backbuffer_dimensions.y) * 2.0f, screen.z, screen.w);
	output.color = input.rgba * allShapes[input.index].shape_color; // send color to pixel shader (interpolated)
	return output;
}