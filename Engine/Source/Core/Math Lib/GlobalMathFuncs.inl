//#ifndef GLOBALMATHFUNCS_INL
//#define GLOBALMATHFUNCS_INL

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
	return Clamp(val, Type(0.0f), Type(1.0f));
}

__forceinline float Lerp(const float& lhs, const float& rhs, const float& fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

__forceinline double Lerp(const double& lhs, const double& rhs, const double& fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

__forceinline Vec2f_Out Lerp(Vec2f_In lhs, Vec2f_In rhs, const float& fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

__forceinline Vec3f_Out Lerp(Vec3f_In lhs, Vec3f_In rhs, const float& fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

__forceinline Vec4f_Out Lerp(Vec4f_In lhs, Vec4f_In rhs, const float& fLambda)
{
	return lhs + (rhs - lhs) * fLambda;
}

__forceinline float Abs(const float& fScalar)
{
	return abs(fScalar);
}

__forceinline double Abs(const double& fScalar)
{
	return abs(fScalar);
}

template<VecElem pX, VecElem pY>
__forceinline Vec2f_Out Permute(Vec2f_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::Y) &&
						(pY >= VecElem::X && pY <= VecElem::Y), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec2f_Out(lhs[pX], lhs[pY]);
}

template<VecElem pX, VecElem pY, VecElem pZ>
__forceinline Vec3f_Out Permute(Vec3f_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::Z) &&
						(pY >= VecElem::X && pY <= VecElem::Z) &&
						(pZ >= VecElem::X && pY <= VecElem::Z), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Z!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y && pZ == VecElem::Z, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec3f_Out(lhs[pX], lhs[pY], lhs[pZ]);
}

template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW> 
__forceinline Vec4f_Out Permute(Vec4f_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::W) &&
						(pY >= VecElem::X && pY <= VecElem::W) &&
						(pZ >= VecElem::X && pY <= VecElem::W) &&
						(pW >= VecElem::X && pY <= VecElem::W), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::W!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y && pZ == VecElem::Z && pW == VecElem::W, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec4f_Out(lhs[pX], lhs[pY], lhs[pZ], lhs[pW]);
}

template<VecElem pX, VecElem pY>
__forceinline Vec2f_Out Permute(Vec2f_In lhs, Vec2f_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::Y1 && pX >= VecElem::X2 && pX <= VecElem::Y2) &&
						(pY >= VecElem::X1 && pY <= VecElem::Y1 && pY >= VecElem::X2 && pY <= VecElem::Y2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	const float** vectors[] = {lhs.GetVector(), rhs.GetVector()};
	return Vec2f(vectors[pX>>4][pX&0x3], vectors[pY>>4][pY&0x3]);
}

template<VecElem pX, VecElem pY, VecElem pZ>
__forceinline Vec3f_Out Permute(Vec3f_In lhs, Vec3f_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::Z1 && pX >= VecElem::X2 && pX <= VecElem::Z2) &&
						(pY >= VecElem::X1 && pY <= VecElem::Z1 && pY >= VecElem::X2 && pY <= VecElem::Z2) &&
						(pZ >= VecElem::X1 && pZ <= VecElem::Z1 && pZ >= VecElem::X2 && pZ <= VecElem::Z2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1 && pZ == VecElem::Z1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2 && pZ == VecElem::Z2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	const float** vectors[] = {lhs.GetVector(), rhs.GetVector()};
	return Vec3f(vectors[pX>>4][pX&0x3], vectors[pY>>4][pY&0x3], vectors[pZ>>4][pZ&0x3]);
}

template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW> 
__forceinline Vec4f_Out Permute(Vec4f_In lhs, Vec4f_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::W1 && pX >= VecElem::X2 && pX <= VecElem::W2) &&
						(pY >= VecElem::X1 && pY <= VecElem::W1 && pY >= VecElem::X2 && pY <= VecElem::W2) &&
						(pZ >= VecElem::X1 && pZ <= VecElem::W1 && pZ >= VecElem::X2 && pZ <= VecElem::W2) &&
						(pW >= VecElem::X1 && pW <= VecElem::W1 && pW >= VecElem::X2 && pW <= VecElem::W2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1 && pZ == VecElem::Z1 && pW == VecElem::W1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2 && pZ == VecElem::Z2 && pW == VecElem::W2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	const float** vectors[] = {lhs.GetVector(), rhs.GetVector()};
	return Vec4f(vectors[pX>>4][pX&0x3], vectors[pY>>4][pY&0x3], vectors[pZ>>4][pZ&0x3], vectors[pW>>4][pW&0x3]);
}


// Comparison Funcs

#define VEC_CMP_MAKE_MASK_1  ((vecResult.GetXRef() != 0.0f) << 0)

#define VEC_CMP_MAKE_MASK_2 VEC_CMP_MAKE_MASK_1 | ((vecResult.GetYRef() != 0) << 1)

#define VEC_CMP_MAKE_MASK_3 VEC_CMP_MAKE_MASK_2 | ((vecResult.GetZRef() != 0) << 2)

#define VEC_CMP_MAKE_MASK_4 VEC_CMP_MAKE_MASK_3 | ((vecResult.GetWRef() != 0) << 3)

#define VEC_CMP_2(cmp) \
		lhs.GetXRef() cmp rhs.GetXRef() ? 0xFFFFFFFF : 0, \
		lhs.GetYRef() cmp rhs.GetYRef() ? 0xFFFFFFFF : 0

#define VEC_CMP_3(cmp) \
		VEC_CMP_2(cmp) \
		, lhs.GetZRef() cmp rhs.GetZRef() ? 0xFFFFFFFF : 0

#define VEC_CMP_4(cmp) \
		VEC_CMP_3(cmp) \
		, lhs.GetWRef() cmp rhs.GetWRef() ? 0xFFFFFFFF : 0

#define VEC_CMP_DEFBASE(name, varType, cmpDef)	\
	__forceinline varType##_Out name ( varType##_In lhs, varType##_In rhs) \
	{ \
		return varType##Int ( cmpDef ); \
	}

#define VEC_CMP_DEF(name, amt, nameExt, varType) \
	__forceinline bool name##nameExt ( varType##_In lhs, varType##_In rhs) \
	{ \
		varType vecResult = name(lhs, rhs); \
		return ( VEC_CMP_MAKE_MASK_##amt ) == CMP_MASK_##nameExt ; \
	}

#define VEC_CMP_DEF1(name, varType) VEC_CMP_DEF( name, 1, X, varType)
#define VEC_CMP_DEF2(name, varType) VEC_CMP_DEF( name, 2, XY, varType)
#define VEC_CMP_DEF3(name, varType) VEC_CMP_DEF( name, 3, XYZ, varType)
#define VEC_CMP_DEF4(name, varType) VEC_CMP_DEF( name, 4, XYZW, varType)

#define VEC_CMP_DEF_VEC2(name, cmp, varType)  \
		VEC_CMP_DEFBASE(name, varType, VEC_CMP_2(cmp)) \
		VEC_CMP_DEF1(name, varType) \
		VEC_CMP_DEF2(name, varType) 

#define VEC_CMP_DEF_VEC3(name, cmp, varType)  \
		VEC_CMP_DEFBASE(name, varType, VEC_CMP_3(cmp)) \
		VEC_CMP_DEF1(name, varType) \
		VEC_CMP_DEF2(name, varType) \
		VEC_CMP_DEF3(name, varType)

#define VEC_CMP_DEF_VEC4(name, cmp, varType)  \
		VEC_CMP_DEFBASE(name, varType, VEC_CMP_4(cmp)) \
		VEC_CMP_DEF1(name, varType) \
		VEC_CMP_DEF2(name, varType) \
		VEC_CMP_DEF3(name, varType) \
		VEC_CMP_DEF4(name, varType)

VEC_CMP_DEF_VEC2(IsEqual, ==, Vec2f)
VEC_CMP_DEF_VEC3(IsEqual, ==, Vec3f)
VEC_CMP_DEF_VEC4(IsEqual, ==, Vec4f)

VEC_CMP_DEF_VEC2(IsNotEqual, !=, Vec2f)
VEC_CMP_DEF_VEC3(IsNotEqual, !=, Vec3f)
VEC_CMP_DEF_VEC4(IsNotEqual, !=, Vec4f)

VEC_CMP_DEF_VEC2(IsGreaterThan, >, Vec2f)
VEC_CMP_DEF_VEC3(IsGreaterThan, >, Vec3f)
VEC_CMP_DEF_VEC4(IsGreaterThan, >, Vec4f)

VEC_CMP_DEF_VEC2(IsGreaterThanOrEqual, >=, Vec2f)
VEC_CMP_DEF_VEC3(IsGreaterThanOrEqual, >=, Vec3f)
VEC_CMP_DEF_VEC4(IsGreaterThanOrEqual, >=, Vec4f)

VEC_CMP_DEF_VEC2(IsLessThan, <, Vec2f)
VEC_CMP_DEF_VEC3(IsLessThan, <, Vec3f)
VEC_CMP_DEF_VEC4(IsLessThan, <, Vec4f)

VEC_CMP_DEF_VEC2(IsLessThanOrEqual, <=, Vec2f)
VEC_CMP_DEF_VEC3(IsLessThanOrEqual, <=, Vec3f)
VEC_CMP_DEF_VEC4(IsLessThanOrEqual, <=, Vec4f)

#undef VEC_CMP_2
#undef VEC_CMP_3
#undef VEC_CMP_4

#define VEC_CMP_2(cmp) \
	lhs.GetXiRef() cmp rhs.GetXiRef() ? 0xFFFFFFFF : 0, \
	lhs.GetYiRef() cmp rhs.GetYiRef() ? 0xFFFFFFFF : 0

#define VEC_CMP_3(cmp) \
	VEC_CMP_2(cmp) \
	, lhs.GetZiRef() cmp rhs.GetZiRef() ? 0xFFFFFFFF : 0

#define VEC_CMP_4(cmp) \
	VEC_CMP_3(cmp) \
	, lhs.GetWiRef() cmp rhs.GetWiRef() ? 0xFFFFFFFF : 0

VEC_CMP_DEF_VEC2(IsEqualInt, ==, Vec2f)
VEC_CMP_DEF_VEC3(IsEqualInt, ==, Vec3f)
VEC_CMP_DEF_VEC4(IsEqualInt, ==, Vec4f)

VEC_CMP_DEF_VEC2(IsNotEqualInt, !=, Vec2f)
VEC_CMP_DEF_VEC3(IsNotEqualInt, !=, Vec3f)
VEC_CMP_DEF_VEC4(IsNotEqualInt, !=, Vec4f)

VEC_CMP_DEF_VEC2(IsGreaterThanInt, >, Vec2f)
VEC_CMP_DEF_VEC3(IsGreaterThanInt, >, Vec3f)
VEC_CMP_DEF_VEC4(IsGreaterThanInt, >, Vec4f)

VEC_CMP_DEF_VEC2(IsGreaterThanOrEqualInt, >=, Vec2f)
VEC_CMP_DEF_VEC3(IsGreaterThanOrEqualInt, >=, Vec3f)
VEC_CMP_DEF_VEC4(IsGreaterThanOrEqualInt, >=, Vec4f)

VEC_CMP_DEF_VEC2(IsLessThanInt, <, Vec2f)
VEC_CMP_DEF_VEC3(IsLessThanInt, <, Vec3f)
VEC_CMP_DEF_VEC4(IsLessThanInt, <, Vec4f)

VEC_CMP_DEF_VEC2(IsLessThanOrEqualInt, <=, Vec2f)
VEC_CMP_DEF_VEC3(IsLessThanOrEqualInt, <=, Vec3f)
VEC_CMP_DEF_VEC4(IsLessThanOrEqualInt, <=, Vec4f)

#undef VEC_CMP_DEF_VEC4
#undef VEC_CMP_DEF_VEC3
#undef VEC_CMP_DEF_VEC2

#undef VEC_CMP_DEF4
#undef VEC_CMP_DEF3
#undef VEC_CMP_DEF2
#undef VEC_CMP_DEF1

#undef VEC_CMP_DEF
#undef VEC_CMP_DEFBASE

#undef VEC_CMP_4
#undef VEC_CMP_3
#undef VEC_CMP_2

#undef VEC_CMP_MAKE_MASK_4
#undef VEC_CMP_MAKE_MASK_3
#undef VEC_CMP_MAKE_MASK_2
#undef VEC_CMP_MAKE_MASK_1

__forceinline float Dot(Vec2f_In vVectorA, Vec2f_In vVectorB)
{
	return (vVectorA.x * vVectorB.x) + (vVectorA.y * vVectorB.y);
}

__forceinline float Dot(Vec3f_In vVectorA, Vec3f_In vVectorB)
{
	return (vVectorA.x * vVectorB.x) + (vVectorA.y * vVectorB.y) + (vVectorA.z * vVectorB.z);
}

__forceinline float Dot(Vec4f_In vVectorA, Vec4f_In vVectorB)
{
#if SSE_AVAILABLE
	__m128 vec1 = VectorLoadU(vVectorA.GetVector());
	__m128 vec2 = VectorLoadU(vVectorB.GetVector());
	vec1 = VectorMultiply(vec1, vec2);
	vec1 = VectorHAdd(vec1, vec1);
	return VectorExtractFloat<VecElem::X>(VectorHAdd(vec1, vec1));
#else
	return (vVectorA.x * vVectorB.x) + (vVectorA.y * vVectorB.y) + (vVectorA.z * vVectorB.z) + (vVectorA.w * vVectorB.w);
#endif
}

__forceinline float Mag(Vec2f_In vVector)
{
	return sqrtf(MagSq(vVector));
}

__forceinline float Mag(Vec3f_In vVector)
{
	return sqrtf(MagSq(vVector));
}

__forceinline float Mag(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	Vector vec = VectorLoadU(vVector.GetVector());
	Vector magSq = vec * vec;
	magSq = VectorHAdd(magSq, magSq);
	magSq = VectorHAdd(magSq, magSq);
	return VectorExtractFloat<VecElem::X>(VectorSqrt(magSq));
#else
	return sqrtf(MagSq());
#endif
}

__forceinline float Length(Vec2f_In vVector)
{
	return Mag(vVector);
}

__forceinline float Length(Vec3f_In vVector)
{
	return Mag(vVector);
}

__forceinline float Length(Vec4f_In vVector)
{
	return Mag(vVector);
}

__forceinline float MagSq(Vec2f_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline float MagSq(Vec3f_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline float MagSq(Vec4f_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline float LengthSq(Vec2f_In vVector)
{
	return MagSq(vVector);
}

__forceinline float LengthSq(Vec3f_In vVector)
{
	return MagSq(vVector);
}

__forceinline float LengthSq(Vec4f_In vVector)
{
	return MagSq(vVector);
}

__forceinline Vec2f_Out Normalize(Vec2f_In vVector)
{
	float mag = Mag(vVector);
	if(mag)
	{
		mag = 1 / mag;

		return vVector * mag;
	}
	return vVector;
}

__forceinline Vec3f_Out Normalize(Vec3f_In vVector)
{
	float mag = Mag(vVector);

	// protection against divide by zero
	if(mag)
	{
#if SSE_AVAILABLE
		return Vec3f(VectorDivide(VectorLoadU(vVector.GetVector()), VectorSet(mag, mag, mag, 0.0f)));
#else 
		mag = 1 / mag;

		return vVector * mag;
#endif
	}

	return vVector;
}

__forceinline Vec4f_Out Normalize(Vec4f_In vVector)
{
	float mag = Mag(vVector);
	if(mag)
	{
#if SSE_AVAILABLE
		return Vec4f(VectorDivide(VectorLoadU(vVector.GetVector()), VectorSet(mag)));
#else
		mag = 1 / mag;

		return vVector * mag;
#endif
	}
	return vVector;
}

__forceinline Mat33f_Out Normalize(Mat33f_In mMatrix)
{
	Mat33f tmp(mMatrix);
	tmp.Normalize();
	return tmp;
}

__forceinline Mat44f_Out Normalize(Mat44f_In mMatrix)
{
	Mat44f tmp(mMatrix);
	tmp.Normalize();
	return tmp;
}

__forceinline Vec2f_Out IntToFloat(Vec2f_In vVector)
{
	return Vec2f((float)*reinterpret_cast<const int*>(&vVector.GetXRef()), 
				 (float)*reinterpret_cast<const int*>(&vVector.GetYRef()));
}

__forceinline Vec3f_Out IntToFloat(Vec3f_In vVector)
{
	return Vec3f((float)*reinterpret_cast<const int*>(&vVector.GetXRef()), 
				 (float)*reinterpret_cast<const int*>(&vVector.GetYRef()), 
				 (float)*reinterpret_cast<const int*>(&vVector.GetZRef()));
}

__forceinline Vec4f_Out IntToFloat(Vec4f_In vVector)
{
	return Vec4f((float)*reinterpret_cast<const int*>(&vVector.GetXRef()), 
				 (float)*reinterpret_cast<const int*>(&vVector.GetYRef()), 
				 (float)*reinterpret_cast<const int*>(&vVector.GetZRef()),
				 (float)*reinterpret_cast<const int*>(&vVector.GetWRef()));
}

__forceinline Vec2f_Out FloatToInt(Vec2f_In vVector)
{
	return Vec2fInt((s32)vVector.GetXRef(), (s32)vVector.GetYRef());
}

__forceinline Vec3f_Out FloatToInt(Vec3f_In vVector)
{
	return Vec3fInt((s32)vVector.GetXRef(), (s32)vVector.GetYRef(), (s32)vVector.GetZRef());
}

__forceinline Vec4f_Out FloatToInt(Vec4f_In vVector)
{
	return Vec4fInt((s32)vVector.GetXRef(), (s32)vVector.GetYRef(), (s32)vVector.GetZRef(), (s32)vVector.GetWRef());
}

__forceinline Vec2f_Out Floor(Vec2f_In vVector)
{
	return Vec2f(floorf(vVector.GetXRef()), floorf(vVector.GetYRef()));
}

__forceinline Vec3f_Out Floor(Vec3f_In vVector)
{
	return Vec3f(floorf(vVector.GetXRef()), floorf(vVector.GetYRef()), floorf(vVector.GetZRef()));
}

__forceinline Vec4f_Out Floor(Vec4f_In vVector)
{
	return Vec4f(floorf(vVector.GetXRef()), floorf(vVector.GetYRef()), floorf(vVector.GetZRef()), floorf(vVector.GetWRef()));
}

__forceinline Vec2f_Out Ceil(Vec2f_In vVector)
{
	return Vec2f(ceilf(vVector.GetXRef()), ceilf(vVector.GetYRef()));
}

__forceinline Vec3f_Out Ceil(Vec3f_In vVector)
{
	return Vec3f(ceilf(vVector.GetXRef()), ceilf(vVector.GetYRef()), ceilf(vVector.GetZRef()));
}

__forceinline Vec4f_Out Ceil(Vec4f_In vVector)
{
	return Vec4f(ceilf(vVector.GetXRef()), ceilf(vVector.GetYRef()), ceilf(vVector.GetZRef()), ceilf(vVector.GetWRef()));
}

__forceinline Vec2f_Out Trunc(Vec2f_In vVector)
{
	return FloatToInt(vVector);
}

__forceinline Vec3f_Out Trunc(Vec3f_In vVector)
{
	return FloatToInt(vVector);
}

__forceinline Vec4f_Out Trunc(Vec4f_In vVector)
{
	return FloatToInt(vVector);
}

__forceinline Vec2f_Out Round(Vec2f& vVector)
{
	return Vec2f(floorf(vVector.GetXRef()+0.5f), floorf(vVector.GetYRef()+0.5f));
}

__forceinline Vec3f_Out Round(Vec3f_In vVector)
{
	return Vec3f(floorf(vVector.GetXRef()+0.5f), floorf(vVector.GetYRef()+0.5f), floorf(vVector.GetZRef()+0.5f));
}

__forceinline Vec4f_Out Round(Vec4f_In vVector)
{
	return Vec4f(floorf(vVector.GetXRef()+0.5f), floorf(vVector.GetYRef()+0.5f), floorf(vVector.GetZRef()+0.5f), floorf(vVector.GetWRef()+0.5f));
}

__forceinline Mat44f_Out Lerp(Mat44f_In MatrixA, Mat44f_In MatrixB, const float fLambda)
{
	Mat44f result;
	result.GetXAxisRef().SetXYZ(Lerp(MatrixA.GetXAxis().GetXYZ(), MatrixB.GetXAxis().GetXYZ(), fLambda));
	result.GetYAxisRef().SetXYZ(Lerp(MatrixA.GetYAxis().GetXYZ(), MatrixB.GetYAxis().GetXYZ(), fLambda));
	result.GetZAxisRef().SetXYZ(Lerp(MatrixA.GetZAxis().GetXYZ(), MatrixB.GetZAxis().GetXYZ(), fLambda));
	result.GetWAxisRef().SetXYZ(Lerp(MatrixA.GetWAxis().GetXYZ(), MatrixB.GetWAxis().GetXYZ(), fLambda));
	return result;
}

__forceinline Vec3f_Out Cross(Vec3f_In lhs, Vec3f_In rhs)
{
	return Vec3f(lhs.GetY() * rhs.GetZ() - lhs.GetZ() * rhs.GetY(),
				 lhs.GetZ() * rhs.GetX() - lhs.GetX() * rhs.GetZ(),
				 lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX());
}

__forceinline Mat44f_Out MatrixInverse(Mat44f_In mMatrix)
{
	Mat44f retVal(mMatrix);
	retVal.Invert();
	return retVal;
}

__forceinline Mat33f_Out Transpose(Mat33f_In mMatrix)
{
	Mat33f retVal(mMatrix);
	retVal.Transpose();
	return retVal;
}

__forceinline Mat44f_Out Transpose(Mat44f_In mMatrix)
{
	Mat44f retVal(mMatrix);
	retVal.Transpose();
	return retVal;
}

__forceinline Mat44f_Out Transpose3x3(Mat44f_In mMatrix)
{
	Mat44f retVal(mMatrix);
	retVal.Transpose3x3();
	return retVal;
}

__forceinline Mat44f_Out RotateAround(Vec3f_In origin, Mat44f_In rotatorsMatrix, Mat44f_In rotationMatrix)
{
	Mat44f newMat(rotatorsMatrix);
	newMat.Translate(-origin);
	newMat *= rotationMatrix;
	newMat.Translate(origin);
	return newMat;
}

__forceinline Mat44f_Out MakePerspectiveMatrix(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

	return Mat44f(	xScale,		0.0f,	0.0f,												0.0f,
					0.0f,		yScale,	0.0f,												0.0f,
					0.0f,		0.0f,	fFarClip / (fFarClip - fNearClip),					1.0f,
					0.0f,		0.0f,	(-fNearClip * fFarClip) / (fFarClip - fNearClip),	0.0f);
}

__forceinline Mat44f_Out MakeOrthographicMatrix(float fWidth, float fHeight, float fNear, float fFar)
{
	return Mat44f(	2.0f / fWidth,		0.0f,					0.0f,						0.0f,
					0.0f,				2.0f / fHeight,			0.0f,						0.0f,
					0.0f,				0.0f,					1.0f / (fFar - fNear),		0.0f,
					0.0f,				0.0f,					-fNear / (fFar - fNear),	1.0f);
}

__forceinline Mat44f_Out MakeTextureMatrixOffset(u32 unWidth, u32 unHeight)
{
	return Mat44f(0.5f,						0.0f,						0.0f,	0.0f,
				  0.0f,						-0.5f,						0.0f,	0.0f,
				  0.0f,						0.0f,						1.0f,	0.0f,
				  0.5f + (0.5f / unWidth),	0.5f + (0.5f / unHeight),	0.0f,	1.0f);
}

float CalculateGaussianWeight(s32 nOffset, float fSigma)
{
	return (1 / sqrt(TWO_PI * (fSigma * fSigma))) * exp(-(nOffset * nOffset) / (2 * (fSigma * fSigma)));
}

// unRadius - Number of Pixels to Blur In a Single Direction Including the Center Pixel
inline void CalculateGaussianWeights(_Out_writes_all_(unRadius) float* pGaussianWeights, u32 unRadius, float fLimit)
{
	u32 i;
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
__forceinline ScalarV_Out Lerp(ScalarV_In lhs, ScalarV_In rhs, ScalarV_In vLambda)
{
	return lhs + (rhs - lhs) * vLambda;
}

__forceinline Vec2V_Out Lerp(Vec2V_In lhs, Vec2V_In rhs, ScalarV_In vLambda)
{
	return lhs + (rhs - lhs) * vLambda;
}

__forceinline Vec3V_Out Lerp(Vec3V_In lhs, Vec3V_In rhs, ScalarV_In vLambda)
{
	return lhs + (rhs - lhs) * vLambda;
}

__forceinline Vec4V_Out Lerp(Vec4V_In lhs, Vec4V_In rhs, ScalarV_In vLambda)
{
	return lhs + (rhs - lhs) * vLambda;
}

__forceinline ScalarV_Out Abs(ScalarV_In vScalar)
{
	return ScalarV(VectorAbs(vScalar.GetVector()));
}

template<VecElem pX, VecElem pY>
__forceinline Vec2V_Out Permute(Vec2V_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::Y) &&
						(pY >= VecElem::X && pY <= VecElem::Y), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec2V(VectorPermute<pX, pY, VecElem::Z, VecElem::W>(lhs.GetVector()));
}

template<VecElem pX, VecElem pY, VecElem pZ>
__forceinline Vec3V_Out Permute(Vec3V_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::Z) &&
						(pY >= VecElem::X && pY <= VecElem::Z) &&
						(pZ >= VecElem::X && pY <= VecElem::Z), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Z!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y && pZ == VecElem::Z, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec3V(VectorPermute<pX, pY, pZ, VecElem::W>(lhs.GetVector()));
}
template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW>
__forceinline Vec4V_Out Permute(Vec4V_In lhs)
{
	CompileTimeAssert(	(pX >= VecElem::X && pX <= VecElem::W) &&
						(pY >= VecElem::X && pY <= VecElem::W) &&
						(pZ >= VecElem::X && pY <= VecElem::W) &&
						(pW >= VecElem::X && pY <= VecElem::W), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::W!");
	CompileTimeAssert(	pX == VecElem::X && pY == VecElem::Y && pZ == VecElem::Z && pW == VecElem::W, "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec4V(VectorPermute<pX, pY, pZ, pW>(lhs.GetVector()));
}

template<VecElem pX, VecElem pY>
__forceinline Vec2V_Out Permute(Vec2V_In lhs, Vec2V_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::Y1 && pX >= VecElem::X2 && pX <= VecElem::Y2) &&
						(pY >= VecElem::X1 && pY <= VecElem::Y1 && pY >= VecElem::X2 && pY <= VecElem::Y2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec2V(VectorPermute<pX, pY, VecElem::Z1, VecElem::W1>(lhs, rhs));
}

template<VecElem pX, VecElem pY, VecElem pZ>
__forceinline Vec3V_Out Permute(Vec3V_In lhs, Vec3V_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::Z1 && pX >= VecElem::X2 && pX <= VecElem::Z2) &&
						(pY >= VecElem::X1 && pY <= VecElem::Z1 && pY >= VecElem::X2 && pY <= VecElem::Z2) &&
						(pZ >= VecElem::X1 && pZ <= VecElem::Z1 && pZ >= VecElem::X2 && pZ <= VecElem::Z2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1 && pZ == VecElem::Z1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2 && pZ == VecElem::Z2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec2V(VectorPermute<pX, pY, pZ, VecElem::W1>(lhs, rhs));
}

template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW>
__forceinline Vec4V_Out Permute(Vec4V_In lhs, Vec4V_In rhs)
{
	CompileTimeAssert(	(pX >= VecElem::X1 && pX <= VecElem::W1 && pX >= VecElem::X2 && pX <= VecElem::W2) &&
						(pY >= VecElem::X1 && pY <= VecElem::W1 && pY >= VecElem::X2 && pY <= VecElem::W2) &&
						(pZ >= VecElem::X1 && pZ <= VecElem::W1 && pZ >= VecElem::X2 && pZ <= VecElem::W2) &&
						(pW >= VecElem::X1 && pW <= VecElem::W1 && pW >= VecElem::X2 && pW <= VecElem::W2), "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::Y!");
	CompileTimeAssert(	(pX == VecElem::X1 && pY == VecElem::Y1 && pZ == VecElem::Z1 && pW == VecElem::W1) || 
						(pX == VecElem::X2 && pY == VecElem::Y2 && pZ == VecElem::Z2 && pW == VecElem::W2), "Invalid Permute Indices! Vector Will Not Change, So Don't Bother Calling Permute!");
	return Vec2V(VectorPermute<pX, pY, pZ, pW>(lhs, rhs));
}

template<VecElem elem> 
__forceinline ScalarV_Out ScalarVFromElement(Vector_In vVector)
{
	CompileTimeAssert(elem >= VecElem::X && elem <= VecElem::W, "Invalid Permute Indices! Indices must be between VecElem::X <-> VecElem::W!");
	return ScalarV(VectorPermute<elem, elem, elem, elem>(vVector));
}

template<VecElem elem> 
__forceinline ScalarV_Out ScalarVFromElement(Vec2V_In vVector)
{
	return ScalarVFromElement<elem>(vVector.GetVector());
}

template<VecElem elem> 
__forceinline ScalarV_Out ScalarVFromElement(Vec3V_In vVector)
{
	return ScalarVFromElement<elem>(vVector.GetVector());
}

template<VecElem elem> 
__forceinline ScalarV_Out ScalarVFromElement(Vec4V_In vVector)
{
	return ScalarVFromElement<elem>(vVector.GetVector());
}


// Comparison Functions

#define VEC_CMP_DEFBASE(name, varType)	\
	__forceinline varType##_Out name ( varType##_In lhs, varType##_In rhs) \
	{ \
		return varType( Vector##name (lhs.GetVector(), rhs.GetVector()) ); \
	}

#define VEC_CMP_DEF(name, nameExt, varType) \
	__forceinline bool name##nameExt ( varType##_In lhs, varType##_In rhs ) \
	{ \
		return Vector##name##nameExt (lhs.GetVector(), rhs.GetVector()); \
	}

#define VEC_CMP_DEF1(name, varType) VEC_CMP_DEF(name, X, varType)
#define VEC_CMP_DEF2(name, varType) VEC_CMP_DEF(name, XY, varType)
#define VEC_CMP_DEF3(name, varType) VEC_CMP_DEF(name, XYZ, varType)
#define VEC_CMP_DEF4(name, varType) VEC_CMP_DEF(name, XYZW, varType)

#define VEC_CMP_DEF_VEC2(name, varType) \
		VEC_CMP_DEFBASE(name, varType) \
		VEC_CMP_DEF1(name, varType) \
		VEC_CMP_DEF2(name, varType)

#define VEC_CMP_DEF_VEC3(name, varType) \
		VEC_CMP_DEF_VEC2(name, varType) \
		VEC_CMP_DEF3(name, varType)

#define VEC_CMP_DEF_VEC4(name, varType) \
		VEC_CMP_DEF_VEC3(name, varType) \
		VEC_CMP_DEF4(name, varType)
		
VEC_CMP_DEF_VEC2(IsEqual, Vec2V)
VEC_CMP_DEF_VEC3(IsEqual, Vec3V)
VEC_CMP_DEF_VEC4(IsEqual, Vec4V)

VEC_CMP_DEF_VEC2(IsNotEqual, Vec2V)
VEC_CMP_DEF_VEC3(IsNotEqual, Vec3V)
VEC_CMP_DEF_VEC4(IsNotEqual, Vec4V)

VEC_CMP_DEF_VEC2(IsGreaterThan, Vec2V)
VEC_CMP_DEF_VEC3(IsGreaterThan, Vec3V)
VEC_CMP_DEF_VEC4(IsGreaterThan, Vec4V)

VEC_CMP_DEF_VEC2(IsGreaterThanOrEqual, Vec2V)
VEC_CMP_DEF_VEC3(IsGreaterThanOrEqual, Vec3V)
VEC_CMP_DEF_VEC4(IsGreaterThanOrEqual, Vec4V)

VEC_CMP_DEF_VEC2(IsLessThan, Vec2V)
VEC_CMP_DEF_VEC3(IsLessThan, Vec3V)
VEC_CMP_DEF_VEC4(IsLessThan, Vec4V)

VEC_CMP_DEF_VEC2(IsLessThanOrEqual, Vec2V)
VEC_CMP_DEF_VEC3(IsLessThanOrEqual, Vec3V)
VEC_CMP_DEF_VEC4(IsLessThanOrEqual, Vec4V)

VEC_CMP_DEF_VEC2(IsEqualInt, Vec2V)
VEC_CMP_DEF_VEC3(IsEqualInt, Vec3V)
VEC_CMP_DEF_VEC4(IsEqualInt, Vec4V)

VEC_CMP_DEF_VEC2(IsNotEqualInt, Vec2V)
VEC_CMP_DEF_VEC3(IsNotEqualInt, Vec3V)
VEC_CMP_DEF_VEC4(IsNotEqualInt, Vec4V)

VEC_CMP_DEF_VEC2(IsGreaterThanInt, Vec2V)
VEC_CMP_DEF_VEC3(IsGreaterThanInt, Vec3V)
VEC_CMP_DEF_VEC4(IsGreaterThanInt, Vec4V)

VEC_CMP_DEF_VEC2(IsGreaterThanOrEqualInt, Vec2V)
VEC_CMP_DEF_VEC3(IsGreaterThanOrEqualInt, Vec3V)
VEC_CMP_DEF_VEC4(IsGreaterThanOrEqualInt, Vec4V)

VEC_CMP_DEF_VEC2(IsLessThanInt, Vec2V)
VEC_CMP_DEF_VEC3(IsLessThanInt, Vec3V)
VEC_CMP_DEF_VEC4(IsLessThanInt, Vec4V)

VEC_CMP_DEF_VEC2(IsLessThanOrEqualInt, Vec2V)
VEC_CMP_DEF_VEC3(IsLessThanOrEqualInt, Vec3V)
VEC_CMP_DEF_VEC4(IsLessThanOrEqualInt, Vec4V)

#undef VEC_CMP_DEF_VEC4
#undef VEC_CMP_DEF_VEC3
#undef VEC_CMP_DEF_VEC2

#undef VEC_CMP_DEF4
#undef VEC_CMP_DEF3
#undef VEC_CMP_DEF2
#undef VEC_CMP_DEF1

#undef VEC_CMP_DEF
#undef VEC_CMP_DEFBASE


// Arithmetic Operation:

#define VEC_ARITH_OP2(componentType, cmp) \
		lhs.GetX##componentType##Ref() cmp rhs.GetX##componentType##Ref(), \
		lhs.GetY##componentType##Ref() cmp rhs.GetY##componentType##Ref()

#define VEC_ARITH_OP3(componentType, cmp) \
		VEC_ARITH_OP2(componentType, cmp) \
		, lhs.GetZ##componentType##Ref() cmp rhs.GetZ##componentType##Ref()

#define VEC_ARITH_OP4(componentType, cmp) \
		VEC_ARITH_OP3(componentType, cmp) \
		, lhs.GetW##componentType##Ref() cmp rhs.GetW##componentType##Ref()


#define VEC_ARITH_DEF(name, varType, operations) \
	__forceinline varType##_Out name (varType##_In lhs, varType##_In rhs) \
	{ \
		return varType( operations ); \
	}

#define VEC_ARITH_DEF_SIZE(name, cmp, num) \
		VEC_ARITH_DEF(name, Vec##num##f , VEC_ARITH_OP##num (, cmp))

// Floating Point Operations
VEC_ARITH_DEF_SIZE(Add, +, 2)
VEC_ARITH_DEF_SIZE(Add, +, 3)
VEC_ARITH_DEF_SIZE(Add, +, 4)

VEC_ARITH_DEF_SIZE(Subtract, -, 2)
VEC_ARITH_DEF_SIZE(Subtract, -, 3)
VEC_ARITH_DEF_SIZE(Subtract, -, 4)

VEC_ARITH_DEF_SIZE(Multiply, *, 2)
VEC_ARITH_DEF_SIZE(Multiply, *, 3)
VEC_ARITH_DEF_SIZE(Multiply, *, 4)

VEC_ARITH_DEF_SIZE(Divide, /, 2)
VEC_ARITH_DEF_SIZE(Divide, /, 3)
VEC_ARITH_DEF_SIZE(Divide, /, 4)


#undef	VEC_ARITH_DEF
#define VEC_ARITH_DEF(name, varType, operations) \
	__forceinline varType##_Out name (varType##_In lhs, varType##_In rhs) \
	{ \
		return varType##Int ( operations ); \
	}

#undef	VEC_ARITH_DEF_SIZE
#define	VEC_ARITH_DEF_SIZE(name, cmp, num) \
		VEC_ARITH_DEF(name, Vec##num##f , VEC_ARITH_OP##num (i, cmp))


// Integer Operations
VEC_ARITH_DEF_SIZE(AddInt, +, 2)
VEC_ARITH_DEF_SIZE(AddInt, +, 3)
VEC_ARITH_DEF_SIZE(AddInt, +, 4)

VEC_ARITH_DEF_SIZE(SubtractInt, -, 2)
VEC_ARITH_DEF_SIZE(SubtractInt, -, 3)
VEC_ARITH_DEF_SIZE(SubtractInt, -, 4)

VEC_ARITH_DEF_SIZE(MultiplyInt, *, 2)
VEC_ARITH_DEF_SIZE(MultiplyInt, *, 3)
VEC_ARITH_DEF_SIZE(MultiplyInt, *, 4)

VEC_ARITH_DEF_SIZE(DivideInt, /, 2)
VEC_ARITH_DEF_SIZE(DivideInt, /, 3)
VEC_ARITH_DEF_SIZE(DivideInt, /, 4)


// Vector Math Functions

__forceinline ScalarV_Out Sqrt(ScalarV_In vScalar)
{
	return ScalarV(VectorSqrt(vScalar.GetVector()));
}

__forceinline ScalarV Dot(Vec2V_In vVectorA, Vec2V_In vVectorB)
{
	Vector tmp = VectorMultiply(vVectorA.GetVector(), vVectorB.GetVector());
	return ScalarVFromElement<VecElem::X>(VectorHAdd(tmp, tmp));
}

__forceinline ScalarV Dot(Vec3V_In vVectorA, Vec3V_In vVectorB)
{
	Vector tmpRhs = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(vVectorB.GetVector(), VectorSet(0.0f));
	Vector tmp = VectorMultiply(vVectorA.GetVector(), tmpRhs);
	tmp = VectorHAdd(tmp, tmp);
	return ScalarVFromElement<VecElem::X>(VectorHAdd(tmp, tmp));
}

__forceinline ScalarV Dot(Vec4V_In vVectorA, Vec4V_In vVectorB)
{
	Vector tmp = VectorMultiply(vVectorA.GetVector(), vVectorB.GetVector());
	tmp = VectorHAdd(tmp, tmp);
	return ScalarVFromElement<VecElem::X>(VectorHAdd(tmp, tmp));
}

__forceinline ScalarV Mag(Vec2V_In vVector)
{
	return Sqrt(MagSq(vVector));
}

__forceinline ScalarV Mag(Vec3V_In vVector)
{
	return Sqrt(MagSq(vVector));
}

__forceinline ScalarV Mag(Vec4V_In vVector)
{
	return Sqrt(MagSq(vVector));
}

__forceinline ScalarV Length(Vec2V_In vVector)
{
	return Sqrt(LengthSq(vVector));
}

__forceinline ScalarV Length(Vec3V_In vVector)
{
	return Sqrt(LengthSq(vVector));
}

__forceinline ScalarV Length(Vec4V_In vVector)
{
	return Sqrt(LengthSq(vVector));
}

__forceinline ScalarV MagSq(Vec2V_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline ScalarV MagSq(Vec3V_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline ScalarV MagSq(Vec4V_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline ScalarV LengthSq(Vec2V_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline ScalarV LengthSq(Vec3V_In vVector)
{
	return Dot(vVector, vVector);
}

__forceinline ScalarV LengthSq(Vec4V_In vVector)
{
	return Dot(vVector, vVector);
}

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

//__forceinline Mat33V_Out Normalize(Mat33V_In mMatrix)
//{
//	Mat33V tmp(mMatrix);
//	tmp.Normalize();
//	return tmp;
//}

__forceinline Mat44V_Out Normalize(Mat44V_In mMatrix)
{
	Mat44V tmp(mMatrix);
	tmp.Normalize();
	return tmp;
}

__forceinline Vec2V_Out AddInt(Vec2V_In lhs, Vec2V_In rhs)
{
	return Vec2V(VectorAddInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec3V_Out AddInt(Vec3V_In lhs, Vec3V_In rhs)
{
	return Vec3V(VectorAddInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec4V_Out AddInt(Vec4V_In lhs, Vec4V_In rhs)
{
	return Vec4V(VectorAddInt(lhs.GetVector(), rhs.GetVector()));
}

__forceinline Vec2V_Out SubtractInt(Vec2V_In lhs, Vec2V_In rhs)
{
	return Vec2V(VectorSubtractInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec3V_Out SubtractInt(Vec3V_In lhs, Vec3V_In rhs)
{
	return Vec3V(VectorSubtractInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec4V_Out SubtractInt(Vec4V_In lhs, Vec4V_In rhs)
{
	return Vec4V(VectorSubtractInt(lhs.GetVector(), rhs.GetVector()));
}

__forceinline Vec2V_Out MultiplyInt(Vec2V_In lhs, Vec2V_In rhs)
{
	return Vec2V(VectorMultiplyInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec3V_Out MultiplyInt(Vec3V_In lhs, Vec3V_In rhs)
{
	return Vec3V(VectorMultiplyInt(lhs.GetVector(), rhs.GetVector()));
}
__forceinline Vec4V_Out MultiplyInt(Vec4V_In lhs, Vec4V_In rhs)
{
	return Vec4V(VectorMultiplyInt(lhs.GetVector(), rhs.GetVector()));
}

__forceinline Vec2V_Out IntToFloat(Vec2V_In vVector)
{
	return Vec2V(VectorIntToFloat(vVector.GetVector()));
}

__forceinline Vec3V_Out IntToFloat(Vec3V_In vVector)
{
	return Vec3V(VectorIntToFloat(vVector.GetVector()));
}

__forceinline Vec4V_Out IntToFloat(Vec4V_In vVector)
{
	return Vec4V(VectorIntToFloat(vVector.GetVector()));
}

__forceinline Vec2V_Out FloatToInt(Vec2V_In vVector)
{
	return Vec2V(VectorFloatToInt(vVector.GetVector()));
}

__forceinline Vec3V_Out FloatToInt(Vec3V_In vVector)
{
	return Vec3V(VectorFloatToInt(vVector.GetVector()));
}

__forceinline Vec4V_Out FloatToInt(Vec4V_In vVector)
{
	return Vec4V(VectorFloatToInt(vVector.GetVector()));
}

__forceinline Vec2V_Out Floor(Vec2V_In vVector)
{
	return Vec2V(VectorFloor(vVector.GetVector()));
}

__forceinline Vec3V_Out Floor(Vec3V_In vVector)
{
	return Vec3V(VectorFloor(vVector.GetVector()));
}

__forceinline Vec4V_Out Floor(Vec4V_In vVector)
{
	return Vec4V(VectorFloor(vVector.GetVector()));
}

__forceinline Vec2V_Out Ceil(Vec2V_In vVector)
{
	return Vec2V(VectorCeil(vVector.GetVector()));
}

__forceinline Vec3V_Out Ceil(Vec3V_In vVector)
{
	return Vec3V(VectorCeil(vVector.GetVector()));
}

__forceinline Vec4V_Out Ceil(Vec4V_In vVector)
{
	return Vec4V(VectorCeil(vVector.GetVector()));
}

__forceinline Vec2V_Out Trunc(Vec2V_In vVector)
{
	return Vec2V(VectorTrunc(vVector.GetVector()));
}

__forceinline Vec3V_Out Trunc(Vec3V_In vVector)
{
	return Vec3V(VectorTrunc(vVector.GetVector()));
}

__forceinline Vec4V_Out Trunc(Vec4V_In vVector)
{
	return Vec4V(VectorTrunc(vVector.GetVector()));
}

__forceinline Vec2V_Out Round(Vec2V_In vVector)
{
	return Vec2V(VectorRound(vVector.GetVector()));
}

__forceinline Vec3V_Out Round(Vec3V_In vVector)
{
	return Vec3V(VectorRound(vVector.GetVector()));
}

__forceinline Vec4V_Out Round(Vec4V_In vVector)
{
	return Vec4V(VectorRound(vVector.GetVector()));
}

inline Mat44V_Out Lerp(Mat44V_In MatrixA, Mat44V_In MatrixB, ScalarV_In vLambda)
{
	Mat44V result;
	result.GetXAxisRef().SetXYZ(Lerp(MatrixA.GetXAxis().GetXYZ(), MatrixB.GetXAxis().GetXYZ(), vLambda));
	result.GetYAxisRef().SetXYZ(Lerp(MatrixA.GetYAxis().GetXYZ(), MatrixB.GetYAxis().GetXYZ(), vLambda));
	result.GetZAxisRef().SetXYZ(Lerp(MatrixA.GetZAxis().GetXYZ(), MatrixB.GetZAxis().GetXYZ(), vLambda));
	result.GetWAxisRef().SetXYZ(Lerp(MatrixA.GetWAxis().GetXYZ(), MatrixB.GetWAxis().GetXYZ(), vLambda));
	return result;
}

__forceinline Vec3V_Out Cross(Vec3V_In lhs, Vec3V_In rhs)
{
	return Vec3V(VectorCrossProduct(lhs.GetVector(), rhs.GetVector()));
}

__forceinline Mat44V_Out MatrixInverse(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	retVal.Invert();
	return retVal;
}

__forceinline Mat44V_Out Transpose(Mat44V_In mMatrix)
{
	Mat44V retVal(mMatrix);
	retVal.Transpose();
	return retVal;
}

__forceinline Mat44V_Out Transpose3x3(Mat44V_In mMatrix)
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

__forceinline Mat44V_Out MakePerspectiveMatrixV(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

	return Mat44V(	xScale,		0.0f,	0.0f,												0.0f,
					0.0f,		yScale,	0.0f,												0.0f,
					0.0f,		0.0f,	fFarClip / (fFarClip - fNearClip),					1.0f,
					0.0f,		0.0f,	(-fNearClip * fFarClip) / (fFarClip - fNearClip),	0.0f);
}

__forceinline Mat44V_Out MakeOrthographicMatrixV(float fWidth, float fHeight, float fNear, float fFar)
{
	return Mat44V(	2.0f / fWidth,		0.0f,					0.0f,						0.0f,
					0.0f,				2.0f / fHeight,			0.0f,						0.0f,
					0.0f,				0.0f,					1.0f / (fFar - fNear),		0.0f,
					0.0f,				0.0f,					-fNear / (fFar - fNear),	1.0f);
}

__forceinline Mat44V_Out MakeTextureMatrixOffsetV(unsigned int unWidth, unsigned int unHeight)
{
	return Mat44V(0.5f,						0.0f,						0.0f,	0.0f,
				  0.0f,						-0.5f,						0.0f,	0.0f,
				  0.0f,						0.0f,						1.0f,	0.0f,
				  0.5f + (0.5f / unWidth),	0.5f + (0.5f / unHeight),	0.0f,	1.0f);
}
#endif //SSE_AVAILABLE
//#endif //GLOBALMATHFUNCS_INL