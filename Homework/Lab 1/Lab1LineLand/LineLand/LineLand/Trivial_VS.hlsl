struct INPUT_VERTEX
{
	float2 coordinate : POSITION;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
};

// TODO: PART 3 STEP 2a
cbuffer THIS_IS_VRAM : register( b0 )
{
	float4 constantColor;
	float2 constantOffset;
	float2 padding;
};

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	sendToRasterizer.projectedCoordinate.w = 1;
	
	sendToRasterizer.projectedCoordinate.xy = fromVertexBuffer.coordinate.xy;
		
	// TODO : PART 4 STEP 4
	/*sendToRasterizer.projectedCoordinate.xy += constantOffset;*/
	
	// TODO : PART 3 STEP 7
	/*sendToRasterizer.colorOut = constantColor;*/
	// END PART 3

	return sendToRasterizer;
}