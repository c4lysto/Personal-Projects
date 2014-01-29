#ifndef TESSELLATION_HLSL
#define TESSELLATION_HLSL

#define BACKFACE_CULL_THRESHHOLD -0.25f

cbuffer TESSELLATION : register(b9)
{
	float gMinTessFactor;
	float gMaxTessFactor;
	float gTessFactorRange; // should be gMaxTessFactor - gMinTessFactor

	// Screen Space Tessellation Variables
	float gDesiredPixelsPerEdge;
	float gPixelsPerScreenHeight;

	// Distance Tessellation Variables
	float gTessMinDist;
	float gTessMaxDist;
	float gTessDistRange; // should be gTessMaxDist - gTessMinDist
}

// Return true if the triangle is facing away from the camera, false otherwise
bool IsForwardFacingTriangle(float3 vVert1Normal, float3 vVert2Normal, float3 vVert3Normal,/*float3 vTriangleNormal, */float3 vCamToTriangle, float fNormalThreshhold)
{
	float3 vViewDotNormal;

	vViewDotNormal.x = (dot(vCamToTriangle, vVert1Normal) > fNormalThreshhold);
	vViewDotNormal.y = (dot(vCamToTriangle, vVert2Normal) > fNormalThreshhold);
	vViewDotNormal.z = (dot(vCamToTriangle, vVert3Normal) > fNormalThreshhold);

	return all(vViewDotNormal);
}

bool FrustumCull()
{

}

/*
fMinTessFactor - Minimum allowed Tessellation Factor
fMaxTessFactor - Maximum allowed Tessellation Factor
fDistToCamera - Distance from camera to a certain point (generally the edge midpoint)
fMinDist - Minimum distance allowed to reach the Maximum allowed Tessellation Factor
fDistRange - Range that allows for tessellation should be equal to MaxRange - MinRange
*/
precise float CalculateDistTessFactor(float fMinTessFactor, float fMaxTessFactor, float fDistToCamera, float fMinDist, float fDistRange)
{
	return lerp(fMinTessFactor, fMaxTessFactor, 1 - saturate((fDistToCamera - fMinDist) / fDistRange));
}

/*
vControlPoint1 - Control point 1 along the edge
vControlPoint2 - Control point 2 along the edge
mViewProjMatrix - View * Projection matrix
fEdgesPerScreenHeight - Should be equal to Screen Height / Pixels Per Edge
*/
precise float CalculateScreenSpaceTessFactor(float3 vControlPoint1, float3 vControlPoint2, float4x4 mViewProjMatrix, float fEdgesPerScreenHeight)
{
	float fRadius = distance(vControlPoint1, vControlPoint2);
	float3 vMidpoint = (vControlPoint1 + vControlPoint2) * 0.5f;

	float4 vProjPos = mul(float4(vMidpoint, 1), mViewProjMatrix);

	float D = abs((fRadius * mViewProjMatrix[1][1]) / vProjPos.w);

	return D * fEdgesPerScreenHeight;
}

float CalculateDistMipLevel(float fEdgeTessFactor)
{
	// 128 should be replaced by the length of the edge
	return log2(128 / fEdgeTessFactor) + 1;
}

#endif