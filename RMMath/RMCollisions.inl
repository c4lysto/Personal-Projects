#pragma region Sphere Functions
inline void Sphere::Build(const Vec3f* pVerts, unsigned int unNumVerts)
{
	vCenter.zero_out();

	for(unsigned int i = 0; i < unNumVerts; ++i)
		vCenter += pVerts[i];

	vCenter /= (float)unNumVerts;

	fRadius = 0.0f;
	float fTmpRadius;
	for(unsigned int i = 0; i < unNumVerts; ++i)
	{
		fTmpRadius = (pVerts[i] - vCenter).sq_magnitude();

		if(fTmpRadius > fRadius)
			fRadius = fTmpRadius;
	}

	fRadius = sqrt(fRadius);
}
#pragma endregion

#pragma region Plane Functions
inline void Plane::Build(const Vec3f& vTopLeft, const Vec3f& vBottomLeft, const Vec3f& vBottomRight)
{
	vNormal = CrossProduct(vTopLeft - vBottomLeft, vBottomRight - vBottomLeft);
	vNormal.normalize();

	fOffset = DotProduct(vNormal, vBottomLeft);
}
#pragma endregion

#pragma region Frustum Functions
inline void Frustum::Build(float fFov, float fNear, float fFar, float fAspectRatio, const Matrix4f& mCamera)
{
	Vec3f nearCenter = mCamera.position + mCamera.zAxis * fNear;
		Vec3f farCenter = mCamera.position + mCamera.zAxis * fFar;

		float Hnear = 2 * tanf(fFov * 0.5f) * fNear;
		float Hfar = 2 * tanf(fFov * 0.5f) * fFar;
		float Wnear = Hnear * fAspectRatio;
		float Wfar = Hfar * fAspectRatio;

		vCorners[FTL] = farCenter + mCamera.yAxis * (Hfar * 0.5f) - mCamera.xAxis * (Wfar * 0.5f);
		vCorners[FBL] = farCenter - mCamera.yAxis * (Hfar * 0.5f) - mCamera.xAxis * (Wfar * 0.5f);
		vCorners[FBR] = farCenter - mCamera.yAxis * (Hfar * 0.5f) + mCamera.xAxis * (Wfar * 0.5f);
		vCorners[FTR] = farCenter + mCamera.yAxis * (Hfar * 0.5f) + mCamera.xAxis * (Wfar * 0.5f);

		vCorners[NTL] = nearCenter + mCamera.yAxis * (Hnear * 0.5f) - mCamera.xAxis * (Wnear * 0.5f);
		vCorners[NBL] = nearCenter - mCamera.yAxis * (Hnear * 0.5f) - mCamera.xAxis * (Wnear * 0.5f);
		vCorners[NBR] = nearCenter - mCamera.yAxis * (Hnear * 0.5f) + mCamera.xAxis * (Wnear * 0.5f);
		vCorners[NTR] = nearCenter + mCamera.yAxis * (Hnear * 0.5f) + mCamera.xAxis * (Wnear * 0.5f);

		Planes[LEFT_PLANE].Build(vCorners[NTL], vCorners[NBL], vCorners[FBL]);
		Planes[RIGHT_PLANE].Build(vCorners[FTR], vCorners[FBR], vCorners[NBR]);
		Planes[NEAR_PLANE].Build(vCorners[NTR], vCorners[NBR], vCorners[NBL]);
		Planes[FAR_PLANE].Build(vCorners[FTL], vCorners[FBL], vCorners[FBR]);
		Planes[TOP_PLANE].Build(vCorners[NTL], vCorners[FTL], vCorners[FTR]);
		Planes[BOTTOM_PLANE].Build(vCorners[FBL], vCorners[NBL], vCorners[NBR]);
}
#pragma endregion