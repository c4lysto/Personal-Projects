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
__forceinline float DotProduct(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB)
{
	return (vVectorA.GetX() * vVectorB.GetX()) + (vVectorA.GetY() * vVectorB.GetY());
}

__forceinline float DotProduct(Vec3f_In vVectorA, Vec3f_In vVectorB)
{
	return (vVectorA.GetX() * vVectorB.GetX()) + (vVectorA.GetY() * vVectorB.GetY()) + (vVectorA.GetZ() * vVectorB.GetZ());
}

__forceinline float DotProduct(Vec4f_In vVectorA, Vec4f_In vVectorB)
{
#if SSE_AVAILABLE && IS_SLOWER
	__m128 vec1 = _mm_loadu_ps(vVectorA.vector);
	__m128 vec2 = _mm_loadu_ps(vVectorB.vector);

	vec1 = _mm_mul_ps(vec1, vec2);

	__m128 tmp = _mm_shuffle_ps(vec1, vec1, _MM_FSHUFFLE(2, 3, 0, 0));

	vec1 = _mm_add_ps(vec1, tmp);

	return _mm_add_ss(vec1, _mm_shuffle_ps(vec1, vec1, _MM_FSHUFFLE(1, 0, 0, 0))).m128_f32[0];
#else
	return vVectorA.GetX() * vVectorB.GetX() + vVectorA.GetY() * vVectorB.GetY() + vVectorA.GetZ() * vVectorB.GetZ() + vVectorA.GetW() * vVectorB.GetW();
#endif
}

template<typename Type>
__forceinline float AngleBetween(const Vec2<Type>& lhs, const Vec2<Type>& rhs)
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
	return retVal.Invert();
}

__forceinline Mat44 MatrixTranspose(Mat44_In mMatrix)
{
	Mat44 retVal(mMatrix);
	return retVal.Transpose();
}

__forceinline Mat44 MatrixTranspos3x3(Mat44_In mMatrix)
{
	Mat44 retVal(mMatrix);
	return retVal.Transpose3x3();
}

__forceinline Mat44V MakePerspectiveMatrix(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

	return Mat44V(	xScale,		0.0f,	0.0f,											0.0f,
					0.0f,		yScale,	0.0f,											0.0f,
					0.0f,		0.0f,	fFarClip / (fFarClip - fNearClip),				1.0f,
					0.0f,		0.0f,	-fNearClip * fFarClip / (fFarClip - fNearClip),	0.0f);
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

#if SSE_AVAILABLE
__forceinline Vec3V Normalize(Vec3V_In vVector)
{
	Vec3V tmp(vVector);
	return tmp.Normalize();
}

__forceinline Vec4V Normalize(Vec4V_In vVector)
{
	Vec4V tmp(vVector);
	return tmp.Normalize();
}

__forceinline float DotProduct(Vec2V_In vVectorA, Vec2V_In vVectorB)
{
	return (vVectorA.GetX() * vVectorB.GetX()) + (vVectorA.GetY() * vVectorB.GetY());
}

__forceinline float DotProduct(Vec3V_In vVectorA, Vec3V_In vVectorB)
{
	Vec3V tmp = vVectorA * vVectorB;
	return tmp.GetX() + tmp.GetY() + tmp.GetZ();
}

__forceinline float DotProduct(Vec4V_In vVectorA, Vec4V_In vVectorB)
{
	Vec4V tmp = vVectorA * vVectorB;
	return tmp.GetX() + tmp.GetY() + tmp.GetZ() + tmp.GetW();
}

__forceinline float AngleBetween(Vec2V_In lhs, Vec2V_In rhs)
{
	float dotProduct = (lhs.GetX() * rhs.GetX()) + (lhs.GetY() * rhs.GetY());

	float length = lhs.Mag() * rhs.Mag();

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

//__forceinline Mat44V Lerp(Mat44V_In MatrixA, Mat44V_In MatrixB, const float fLambda)
//{
//	Mat44V result;
//
//	result.GetXAxisRef().SetXYZ(Lerp(MatrixA.GetXAxis().GetXYZ(), MatrixB.GetXAxis().GetXYZ(), fLambda));
//	result.GetYAxisRef().SetXYZ(Lerp(MatrixA.GetYAxis().GetXYZ(), MatrixB.GetYAxis().GetXYZ(), fLambda));
//	result.GetZAxisRef().SetXYZ(Lerp(MatrixA.GetZAxis().GetXYZ(), MatrixB.GetZAxis().GetXYZ(), fLambda));
//	result.GetWAxisRef().SetXYZ(Lerp(MatrixA.GetWAxis().GetXYZ(), MatrixB.GetWAxis().GetXYZ(), fLambda));
//
//	return result;
//}

__forceinline Vec3V CrossProduct(Vec3V_In vVectorA, Vec3V_In vVectorB)
{
	return Vec3V(vVectorA.GetY() * vVectorB.GetZ() - vVectorA.GetZ() * vVectorB.GetY(),
				 vVectorA.GetZ() * vVectorB.GetX() - vVectorA.GetX() * vVectorB.GetZ(),
				 vVectorA.GetX() * vVectorB.GetY() - vVectorA.GetY() * vVectorB.GetX());
}

__forceinline Mat44V MatrixInverse(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	return retVal.Invert();
}

__forceinline Mat44V MatrixTranspose(Mat44V mMatrix)
{
	Mat44V retVal(mMatrix);
	return retVal.Transpose();
}

__forceinline Mat44V MatrixTranspos3x3(Mat44V mMatrix)
{
	Mat44V retVal(mMatrix);
	return retVal.Transpose3x3();
}
#endif //SSE_AVAILABLE
#endif //GLOBALMATHFUNCS_INL