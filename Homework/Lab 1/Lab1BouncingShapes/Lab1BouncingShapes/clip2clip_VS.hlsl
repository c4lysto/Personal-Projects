#include "SharedDefines.h"

// declare any variables/structures and buffers specific to this vertex shader only

// output from the vertex shader
struct VS_OUT
{
	float4 color : COLOR;
	float4 clipPos : SV_POSITION; // leave this last
};

// vertex shader simply moves a clip space value down the pipeline.
VS_OUT vs_main( SCREEN_VERTEX input ) 
{
	VS_OUT output;
	// alter clip location via matrix (must be clip space matrix)
	output.clipPos = mul(float4(input.pos,1.0f), allShapes[input.index].shape_matrix);
	output.color = input.rgba * allShapes[input.index].shape_color; // send color to pixel shader (interpolated)
	return output;
}