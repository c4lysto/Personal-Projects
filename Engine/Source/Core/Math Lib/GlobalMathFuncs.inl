#ifndef GLOBALMATHFUNCS_INL
#define GLOBALMATHFUNCS_INL

template<typename Type>
__forceinline Type Max(Type lhs, Type rhs)
{
	return (lhs > rhs) ? lhs : rhs;
}

template<typename Type>
__forceinline Type Min(Type lhs, Type rhs)
{
	return (lhs < rhs) ? lhs : rhs;
}

template<typename Type>
__forceinline Type Clamp(Type val, Type minVal, Type maxVal)
{
	return (val > minVal) ? (val < maxVal) ? val : maxVal : minVal;
}

template<typename Type>
__forceinline Type Saturate(Type val)
{
	return Clamp(val, (Type)0, (Type)1);
}

template<typename Type>
__forceinline Vec2<Type> Normalize(const Vec2<Type>& vVector)
{
	Vec2<Type> tmp(vVector);
	return tmp.Normalize();
}

__forceinline Vec3f Normalize(Vec3f_In vVector)
{
	Vec3f tmp(vVector);
	return tmp.Normalize();
}

__forceinline Vec4f Normalize(Vec4f_In vVector)
{
	Vec4f tmp(vVector);
	return tmp.Normalize();
}

template<typename Type>
inline float AngleBetween(const Vec2<Type>& lhs, const Vec2<Type>& rhs)
{
	float dotProduct = (lhs.x * rhs.x) + (lhs.y * rhs.y);

	float length = lhs.magnitude() * rhs.magnitude();

	if(!length)
		return 0.0f;

	float angle = acos(dotProduct / length);

	if(_isnan(angle))
		return 0.0f;

	return angle;
}

template<typename Type>
__forceinline Type Lerp(const Type& lhs, const Type& rhs, const float fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

//template<typename Type>
//__forceinline Vec2<Type> Lerp(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}

//__forceinline Vec3f Lerp(Vec3f_In vVectorA, Vec3f_In vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}
//
//__forceinline Vec4f Lerp(Vec4f_In vVectorA, Vec4f_In vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}

__forceinline Mat44 Lerp(Mat44_In MatrixA, Mat44_In MatrixB, const float fLambda)
{
	Mat44 result;
	result.GetXAxisRef().SetXYZ(Lerp(MatrixA.GetXAxis().GetXYZ(), MatrixB.GetXAxis().GetXYZ(), fLambda));
	result.GetYAxisRef().SetXYZ(Lerp(MatrixA.GetYAxis().GetXYZ(), MatrixB.GetYAxis().GetXYZ(), fLambda));
	result.GetZAxisRef().SetXYZ(Lerp(MatrixA.GetZAxis().GetXYZ(), MatrixB.GetZAxis().GetXYZ(), fLambda));
	result.GetWAxisRef().SetXYZ(Lerp(MatrixA.GetWAxis().GetXYZ(), MatrixB.GetWAxis().GetXYZ(), fLambda));
	return result;
}

__forceinline Vec3f CrossProduct(Vec3f_In vVectorA, Vec3f_In vVectorB)
{
	return Vec3f(vVectorA.GetY() * vVectorB.GetZ() - vVectorA.GetZ() * vVectorB.GetY(),
				 vVectorA.GetZ() * vVectorB.GetX() - vVectorA.GetX() * vVectorB.GetZ(),
				 vVectorA.GetX() * vVectorB.GetY() - vVectorA.GetY() * vVectorB.GetX());
}

__forceinline Mat44 MatrixInverse(Mat44_In mMatrix)
{
	Mat44 retVal(mMatrix);
	retVal.Invert();
	return retVal;
}

__forceinline Mat44 MatrixTranspose(Mat44_In mMatrix)
{
	Mat44 retVal(mMatrix);
	retVal.Transpose();
	return retVal;
}

__forceinline Mat44 MatrixTranspos3x3(Mat44_In mMatrix)
{
	Mat44 retVal(mMatrix);
	retVal.Transpose3x3();
	return retVal;
}

__forceinline Mat44_Out RotateAround(Vec3f_In origin, Mat44_In rotatorsMatrix, Mat44_In rotationMatrix)
{
	Mat44 newMat(rotatorsMatrix);
	newMat.Translate(-origin);
	newMat *= rotationMatrix;
	newMat.Translate(origin);
	return newMat;
}

__forceinline Mat44V MakePerspectiveMatrix(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

	return Mat44V(	xScale,		0.0f,	0.0f,												0.0f,
					0.0f,		yScale,	0.0f,												0.0f,
					0.0f,		0.0f,	fFarClip / (fFarClip - fNearClip),					1.0f,
					0.0f,		0.0f,	(-fNearClip * fFarClip) / (fFarClip - fNearClip),	0.0f);
}

__forceinline Mat44V MakeOrthographicMatrix(float fWidth, float fHeight, float fNear, float fFar)
{
	return Mat44V(	2.0f / fWidth,		0.0f,					0.0f,						0.0f,
					0.0f,				2.0f / fHeight,			0.0f,						0.0f,
					0.0f,				0.0f,					1.0f / (fFar - fNear),		0.0f,
					0.0f,				0.0f,					-fNear / (fFar - fNear),	1.0f);
}

__forceinline Mat44V MakeTextureMatrixOffset(unsigned int unWidth, unsigned int unHeight)
{
	return Mat44V(0.5f,						0.0f,						0.0f,	0.0f,
				  0.0f,						-0.5f,						0.0f,	0.0f,
				  0.0f,						0.0f,						1.0f,	0.0f,
				  0.5f + (0.5f / unWidth),	0.5f + (0.5f / unHeight),	0.0f,	1.0f);
}
__forceinline float CalculateGaussianWeight(int nOffset, float fSigma = 1.0f)
{
	return (1 / sqrt(_2PI * (fSigma * fSigma))) * exp(-(nOffset * nOffset) / (2 * (fSigma * fSigma)));
}

// unRadius - Number of Pixels to Blur In a Single Direction Including the Center Pixel
inline void CalculateGaussianWeights(_Out_writes_all_(unRadius) float* pGaussianWeights, unsigned int unRadius, float fLimit = 1.0f)
{
	unsigned int i;
	float fCurrWeight;
	float fTotalWeight = 0;
	for(i = 0; i < unRadius; ++i)
	{
		fCurrWeight = CalculateGaussianWeight(i, fLimit);
		fTotalWeight += fCurrWeight;
		pGaussianWeights[i] = fCurrWeight;
	}

	fTotalWeight = fLimit / fTotalWeight;

	// Do the Center one, this also avoids if-checking in the following for loop
	pGaussianWeights[0] *= fTotalWeight;
	for(i = 1; i < unRadius; ++i)
	{
		pGaussianWeights[i] *= fTotalWeight * 0.5f;
	}
}

#if SSE_AVAILABLE
__forceinline Vec2V_Out Normalize(Vec2V_In vVector)
{
	Vec2V tmp(vVector);
	return tmp.Normalize();
}

__forceinline Vec3V_Out Normalize(Vec3V_In vVector)
{
	Vec3V tmp(vVector);
	return tmp.Normalize();
}

__forceinline Vec4V_Out Normalize(Vec4V_In vVector)
{
	Vec4V tmp(vVector);
	return tmp.Normalize();
}

inline float AngleBetween(Vec2V_In lhs, Vec2V_In rhs)
{
	float dotProduct = (lhs.GetX() * rhs.GetX()) + (lhs.GetY() * rhs.GetY());

	float length = Mag(lhs) * Mag(rhs);

	if(!length)
		return 0.0f;

	float angle = acos(dotProduct / length);

	if(_isnan(angle))
		return 0.0f;

	return angle;
}

//__forceinline Vec2V Lerp(Vec2V_In vVectorA, Vec2V_In vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}
//
//__forceinline Vec3f Lerp(Vec3V_In vVectorA, Vec3V_In vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}
//
//__forceinline Vec4f Lerp(Vec4V_In vVectorA, Vec4V_In vVectorB, const float fLambda)
//{
//	return vVectorA + (vVectorB - vVectorA) * fLambda;
//}

inline Mat44V Lerp(Mat44V_In MatrixA, Mat44V_In MatrixB, const float fLambda)
{
	Mat44V result;
	result.GetXAxisRef().SetXYZ(Lerp(MatrixA.GetXAxis().GetXYZ(), MatrixB.GetXAxis().GetXYZ(), fLambda));
	result.GetYAxisRef().SetXYZ(Lerp(MatrixA.GetYAxis().GetXYZ(), MatrixB.GetYAxis().GetXYZ(), fLambda));
	result.GetZAxisRef().SetXYZ(Lerp(MatrixA.GetZAxis().GetXYZ(), MatrixB.GetZAxis().GetXYZ(), fLambda));
	result.GetWAxisRef().SetXYZ(Lerp(MatrixA.GetWAxis().GetXYZ(), MatrixB.GetWAxis().GetXYZ(), fLambda));
	return result;
}

__forceinline Vec3V_Out CrossProduct(Vec3V_In vVectorA, Vec3V_In vVectorB)
{
	return Vec3V(vVectorA.GetY() * vVectorB.GetZ() - vVectorA.GetZ() * vVectorB.GetY(),
				 vVectorA.GetZ() * vVectorB.GetX() - vVectorA.GetX() * vVectorB.GetZ(),
				 vVectorA.GetX() * vVectorB.GetY() - vVectorA.GetY() * vVectorB.GetX());
}

__forceinline Mat44V_Out MatrixInverse(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	retVal.Invert();
	return retVal;
}

__forceinline Mat44V_Out MatrixTranspose(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	retVal.Transpose();
	return retVal;
}

__forceinline Mat44V_Out MatrixTranspose3x3(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	retVal.Transpose3x3();
	return retVal;
}

__forceinline Mat44V_Out RotateAround(Vec3V_In origin, Mat44V_In rotatorsMatrix, Mat44V_In rotationMatrix)
{
	Mat44V newMat(rotatorsMatrix);
	newMat.Translate(-origin);
	newMat *= rotationMatrix;
	newMat.Translate(origin);
	return newMat;
}
#endif //SSE_AVAILABLE
#endif //GLOBALMATHFUNCS_INL