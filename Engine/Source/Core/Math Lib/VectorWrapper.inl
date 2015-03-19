
//#if SSE_AVAILABLE

// Initialization Operations:

__forceinline Vector_Out VectorSet(const float& fVal)
{
	return _mm_set1_ps(fVal);
}

__forceinline Vector_Out VectorSet(const float& fX, const float& fY, const float& fZ, const float& fW)
{
	return _mm_setr_ps(fX, fY, fZ, fW);
}

__forceinline Vector_Out VectorSet(const int& iVal)
{
	return VEC_INT_TO_FLOAT(_mm_set1_epi32(iVal));
}

__forceinline Vector_Out VectorSet(const int& iX, const int& iY, const int& iZ, const int& iW)
{
	return VEC_INT_TO_FLOAT(_mm_setr_epi32(iX, iY, iZ, iW));
}

// Used to load Aligned Data
__forceinline Vector_Out VectorLoad(const float* const alignedFloat4Ptr)
{
	return _mm_load_ps(alignedFloat4Ptr);
}

// Used to load Unaligned Data
__forceinline Vector_Out VectorLoadU(const float* const unalignedFloat4Ptr)
{
	return _mm_loadu_ps(unalignedFloat4Ptr);
}

// Used to store in Aligned Data
__forceinline void VectorStore(Vector_In lhs, float* alignedFloat4Ptr)
{
	_mm_store_ps(alignedFloat4Ptr, lhs);
}

// Used to store in Unaligned Data
__forceinline void VectorStoreU(Vector_In lhs, float* unalignedFloat4Ptr)
{
	_mm_storeu_ps(unalignedFloat4Ptr, lhs);
}

template<VecElem index>
__forceinline float VectorExtractFloat(Vector_In vec)
{
	CompileTimeAssert(index >= VecElem::X && index <= VecElem::W, "Invalid Permute X-Index. Must be between X & W!");
	return _mm_cvtss_f32(VectorPermute<index, index, index, index>(vec));
}

template<> __forceinline float VectorExtractFloat<VecElem::X>(Vector_In vec) {return _mm_cvtss_f32(vec);}

template<VecElem index>
__forceinline int VectorExtractInt(Vector_In vec)
{
	CompileTimeAssert(index >= VecElem::X && index <= VecElem::W, "Invalid Permute X-Index. Must be between X & W!");
	return _mm_extract_epi32(VEC_FLOAT_TO_INT(vec), (int)index);
}


// Conversion Operations:

__forceinline Vector_Out VectorIntToFloat(Vector_In vec)
{
	return _mm_cvtepi32_ps(VEC_FLOAT_TO_INT(vec));
}

__forceinline Vector_Out VectorFloatToInt(Vector_In vec)
{
	return VEC_INT_TO_FLOAT(_mm_cvtps_epi32(vec));
}


// Arithmetic Operations: 

__forceinline Vector_Out VectorAdd(Vector_In lhs, Vector_In rhs)
{
	return _mm_add_ps(lhs, rhs);
}

__forceinline Vector_Out operator+(Vector_In lhs, Vector_In rhs)
{
	return VectorAdd(lhs, rhs);
}

__forceinline Vector_Out VectorAddInt(Vector_In lhs, Vector_In rhs)
{
	return VEC_INT_TO_FLOAT(_mm_add_epi32(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs)));
}

// Operates in the following manner:
// {lhs.x+lhs.y, lhs.z+lhs.w, rhs.x+rhs.y, rhs.z+rhs.w}
__forceinline Vector_Out VectorHAdd(Vector_In lhs, Vector_In rhs)
{
	return _mm_hadd_ps(lhs, rhs);
}

__forceinline Vector_Out VectorSubtract(Vector_In lhs, Vector_In rhs)
{
	return _mm_sub_ps(lhs, rhs);
}

__forceinline Vector_Out operator-(Vector_In lhs, Vector_In rhs)
{
	return VectorSubtract(lhs, rhs);
}

__forceinline Vector_Out VectorSubtractInt(Vector_In lhs, Vector_In rhs)
{
	return VEC_INT_TO_FLOAT(_mm_sub_epi32(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs)));
}

// Operates in the following manner:
// {lhs.x-lhs.y, lhs.z-lhs.w, rhs.x-rhs.y, rhs.z-rhs.w}
__forceinline Vector_Out VectorHSub(Vector_In lhs, Vector_In rhs)
{
	return _mm_hsub_ps(lhs, rhs);
}

__forceinline Vector_Out VectorMultiply(Vector_In lhs, Vector_In rhs)
{
	return _mm_mul_ps(lhs, rhs);
}

__forceinline Vector_Out operator*(Vector_In lhs, Vector_In rhs)
{
	return VectorMultiply(lhs, rhs);
}

__forceinline Vector_Out VectorMultiplyInt(Vector_In lhs, Vector_In rhs)
{
	return VEC_INT_TO_FLOAT(_mm_mul_epi32(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs)));
}

__forceinline Vector_Out VectorDivide(Vector_In lhs, Vector_In rhs)
{
	return _mm_div_ps(lhs, rhs);
}

__forceinline Vector_Out operator/(Vector_In lhs, Vector_In rhs)
{
	return VectorDivide(lhs, rhs);
}

__forceinline Vector_Out VectorNegate(Vector_In vec)
{
	// Toggle The Sign Bit
	return _mm_xor_ps(vec, VEC_INT_TO_FLOAT(_mm_set1_epi32(0x80000000)));
}

__forceinline Vector_Out operator-(Vector_In vec)
{
	return VectorNegate(vec);
}

__forceinline Vector_Out VectorAbs(Vector_In vec)
{
	return _mm_and_ps(vec, VEC_INT_TO_FLOAT(_mm_set1_epi32(0x7FFFFFFF)));
}

__forceinline Vector_Out VectorSqrt(Vector_In vec)
{
	return _mm_sqrt_ps(vec);
}

__forceinline Vector_Out VectorRSqrt(Vector_In vec)
{
	return _mm_rsqrt_ps(vec);
}

__forceinline Vector_Out VectorRecip(Vector_In vec)
{
	return _mm_rcp_ps(vec);
}

__forceinline Vector_Out VectorFloor(Vector_In vec)
{
	return _mm_round_ps(vec, _MM_FROUND_TO_NEG_INF);
}

__forceinline Vector_Out VectorCeil(Vector_In vec)
{
	return _mm_round_ps(vec, _MM_FROUND_TO_POS_INF);
}

__forceinline Vector_Out VectorTrunc(Vector_In vec)
{
	return _mm_round_ps(vec, _MM_FROUND_TO_ZERO);
}

__forceinline Vector_Out VectorRound(Vector_In vec)
{
	return _mm_round_ps(vec, _MM_FROUND_TO_NEAREST_INT);
}

__forceinline Vector_Out VectorSign(Vector_In vec)
{
	return VectorOr(VectorAnd(vec, VectorSet((int)0x80000000)), VectorSet(1.0f));
}


// Logical Operations:

__forceinline Vector_Out VectorAnd(Vector_In lhs, Vector_In rhs)
{
	return _mm_and_ps(lhs, rhs);
}

__forceinline Vector_Out operator&(Vector_In lhs, Vector_In rhs)
{
	return VectorAnd(lhs, rhs);
}

__forceinline Vector_Out VectorAndNot(Vector_In lhs, Vector_In rhs)
{
	return _mm_andnot_ps(rhs, lhs);
}

__forceinline Vector_Out VectorNot(Vector_In vec)
{
	return _mm_xor_ps(vec, VEC_INT_TO_FLOAT(_mm_set1_epi32(0xFFFFFFFF)));
}

__forceinline Vector_Out operator~(Vector_In vec)
{
	return VectorNot(vec);
}

__forceinline Vector_Out VectorOr(Vector_In lhs, Vector_In rhs)
{
	return _mm_or_ps(lhs, rhs);
}

__forceinline Vector_Out operator|(Vector_In lhs, Vector_In rhs)
{
	return VectorOr(lhs, rhs);
}

__forceinline Vector_Out VectorXOr(Vector_In lhs, Vector_In rhs)
{
	return _mm_xor_ps(lhs, rhs);
}

__forceinline Vector_Out operator^(Vector_In lhs, Vector_In rhs)
{
	return VectorXOr(lhs, rhs);
}

__forceinline Vector_Out LeftShift(Vector_In vec, int nCount)
{
	return VEC_INT_TO_FLOAT(_mm_slli_epi32(VEC_FLOAT_TO_INT(vec), nCount));
}

__forceinline Vector_Out operator<<(Vector_In vec, int nCount)
{
	return LeftShift(vec, nCount);
}

__forceinline Vector_Out RightShift(Vector_In vec, int nCount)
{
	return VEC_INT_TO_FLOAT(_mm_srli_epi32(VEC_FLOAT_TO_INT(vec), nCount));
}

__forceinline Vector_Out operator>>(Vector_In vec, int nCount)
{
	return RightShift(vec, nCount);
}


// Comparison Functions:

// Floating Point Comparisons:
#define CMP_MASK_X (0x1)
#define CMP_MASK_XY (0x3)
#define CMP_MASK_XYZ (0x7)
#define CMP_MASK_XYZW (0xF)

#define VEC_CMP_DEFBASE_IMPL(name, intrinsic) \
	__forceinline Vector_Out Vector##name (Vector_In lhs, Vector_In rhs) \
	{ \
		return intrinsic (lhs, rhs); \
	}

#define VEC_CMP_DEF(name, nameExt) \
	__forceinline bool Vector##name##nameExt (Vector_In lhs, Vector_In rhs) \
	{ \
		return ( _mm_movemask_ps( Vector##name (lhs, rhs) ) == CMP_MASK_##nameExt ); \
	}

#define VEC_CMP_DEFBASE(name, intrinsic)  VEC_CMP_DEFBASE_IMPL(name, intrinsic)
#define VEC_CMP1_DEF(name) VEC_CMP_DEF(name, X)
#define VEC_CMP2_DEF(name) VEC_CMP_DEF(name, XY)
#define VEC_CMP3_DEF(name) VEC_CMP_DEF(name, XYZ)
#define VEC_CMP4_DEF(name) VEC_CMP_DEF(name, XYZW)

#define VEC_CMP_DEF_ALL(name, intrinsic)  \
			VEC_CMP_DEFBASE(name, intrinsic) \
			VEC_CMP1_DEF(name) \
			VEC_CMP2_DEF(name) \
			VEC_CMP3_DEF(name) \
			VEC_CMP4_DEF(name)

VEC_CMP_DEF_ALL(IsEqual, _mm_cmpeq_ps);
VEC_CMP_DEF_ALL(IsNotEqual, _mm_cmpneq_ps);
VEC_CMP_DEF_ALL(IsGreaterThan, _mm_cmpgt_ps);
VEC_CMP_DEF_ALL(IsGreaterThanOrEqual, _mm_cmpge_ps);
VEC_CMP_DEF_ALL(IsLessThan, _mm_cmplt_ps);
VEC_CMP_DEF_ALL(IsLessThanOrEqual, _mm_cmple_ps);


// Integer Comparisons:
// Redefine VEC_CMP_DEF & VEC_CMP_DEFBASE for integer operations
#undef VEC_CMP_DEFBASE_IMPL
#define VEC_CMP_DEFBASE_IMPL(name, intrinsic) \
	__forceinline Vector_Out Vector##name (Vector_In lhs, Vector_In rhs) \
{ \
	return VEC_INT_TO_FLOAT(intrinsic(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs))); \
}

#undef VEC_CMP_DEF
#define VEC_CMP_DEF(name, nameExt) \
	__forceinline bool Vector##name##nameExt (Vector_In lhs, Vector_In rhs) \
	{ \
	return ( _mm_movemask_ps( Vector##name (lhs, rhs) ) == CMP_MASK_##nameExt ); \
	}

VEC_CMP_DEF_ALL(IsEqualInt, _mm_cmpeq_epi32);
VEC_CMP_DEF_ALL(IsGreaterThanInt, _mm_cmpgt_epi32);
VEC_CMP_DEF_ALL(IsLessThanInt, _mm_cmplt_epi32);

__forceinline Vector_Out VectorIsNotEqualInt(Vector_In lhs, Vector_In rhs)
{
	return VectorNegate(VectorIsEqualInt(lhs, rhs));
}

__forceinline bool VectorIsNotEqualIntX(Vector_In lhs, Vector_In rhs)
{
	return _mm_movemask_ps(VectorIsNotEqualInt(lhs, rhs)) == CMP_MASK_X;
}

__forceinline bool VectorIsNotEqualIntXY(Vector_In lhs, Vector_In rhs)
{
	return _mm_movemask_ps(VectorIsNotEqualInt(lhs, rhs)) == CMP_MASK_XY;
}

__forceinline bool VectorIsNotEqualIntXYZ(Vector_In lhs, Vector_In rhs)
{
	return _mm_movemask_ps(VectorIsNotEqualInt(lhs, rhs)) == CMP_MASK_XYZ;
}

__forceinline bool VectorIsNotEqualIntXYZW(Vector_In lhs, Vector_In rhs)
{
	return _mm_movemask_ps(VectorIsNotEqualInt(lhs, rhs)) == CMP_MASK_XYZW;
}

#define VEC_CMP_OREQUAL_INT_DEFBASE(name, cmp) \
	__forceinline Vector_Out Vector##name (Vector_In lhs, Vector_In rhs) \
	{ \
		return VectorOr( Vector##name (lhs, rhs), VectorIsEqualInt(lhs, rhs)); \
	}

#define VEC_CMP_OREQUAL_INT_DEF(name, nameExt, cmp) \
	__forceinline bool Vector##name##nameExt (Vector_In lhs, Vector_In rhs) \
	{ \
		return Vector##cmp##nameExt (lhs, rhs) || VectorIsEqualInt##nameExt (lhs, rhs); \
	}

#define VEC_CMP_OREQUAL_INT_DEF_ALL(name, cmp1) \
	VEC_CMP_OREQUAL_INT_DEFBASE(name, cmp1) \
	VEC_CMP_OREQUAL_INT_DEF(name, X, cmp1) \
	VEC_CMP_OREQUAL_INT_DEF(name, XY, cmp1) \
	VEC_CMP_OREQUAL_INT_DEF(name, XYZ, cmp1) \
	VEC_CMP_OREQUAL_INT_DEF(name, XYZW, cmp1)

VEC_CMP_OREQUAL_INT_DEF_ALL(IsGreaterThanOrEqualInt, IsGreaterThanInt);
VEC_CMP_OREQUAL_INT_DEF_ALL(IsLessThanOrEqualInt, IsLessThanInt);

#undef VEC_CMP_OREQUAL_INT_DEF_ALL
#undef VEC_CMP_OREQUAL_INT_DEF
#undef VEC_CMP_OREQUAL_INT_DEFBASE

//__forceinline bool IsLessThanEqualIntAll(Vector_In lhs, Vector_In rhs)
//{
//	return IsLessThanIntAll(lhs, rhs) || IsEqualIntAll(lhs, rhs);
//}

#undef VEC_CMP_DEF_ALL
#undef VEC_CMP4_DEF
#undef VEC_CMP3_DEF
#undef VEC_CMP2_DEF
#undef VEC_CMP1_DEF
#undef VEC_CMP_DEFBASE
#undef VEC_CMP_DEF
#undef VEC_CMP_DEFBASE_IMPL


// Misc Operations
template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW>
__forceinline Vector_Out VectorPermute(Vector_In vec)
{
	CompileTimeAssert(pX >= VecElem::X && pX <= VecElem::W, "Invalid Permute X-Index. Must be between X & W!");
	CompileTimeAssert(pY >= VecElem::X && pY <= VecElem::W, "Invalid Permute Y-Index. Must be between X & W!");
	CompileTimeAssert(pZ >= VecElem::X && pZ <= VecElem::W, "Invalid Permute Z-Index. Must be between X & W!");
	CompileTimeAssert(pW >= VecElem::X && pW <= VecElem::W, "Invalid Permute W-Index. Must be between X & W!");
	return _mm_shuffle_ps(vec, vec, _MM_FSHUFFLE(pX, pY, pZ, pW));
}	

#define SHUFFLE_MASKED(index) ((int)(index) & 0x3)
#define PERM_SHUFFLE(x, y, z, w) (_MM_FSHUFFLE(SHUFFLE_MASKED(x), SHUFFLE_MASKED(y), SHUFFLE_MASKED(z), SHUFFLE_MASKED(w)))
#define BLEND_MASK(x, y, z, w) ((x) | ((y)<<1) | ((z)<<2) | ((w)<<3))

template<VecElem pX, VecElem pY, VecElem pZ, VecElem pW>
__forceinline Vector_Out VectorPermute(Vector_In lhs, Vector_In rhs)
{
	CompileTimeAssert((pX >= VecElem::X1 && pX <= VecElem::W1) || (pX >= VecElem::X2 && pX <= VecElem::W2), "Invalid Permute X-Index. Must be between X1 & W2!");
	CompileTimeAssert((pY >= VecElem::X1 && pY <= VecElem::W1) || (pY >= VecElem::X2 && pY <= VecElem::W2), "Invalid Permute Y-Index. Must be between X1 & W2!");
	CompileTimeAssert((pZ >= VecElem::X1 && pZ <= VecElem::W1) || (pZ >= VecElem::X2 && pZ <= VecElem::W2), "Invalid Permute Z-Index. Must be between X1 & W2!");
	CompileTimeAssert((pW >= VecElem::X1 && pW <= VecElem::W1) || (pW >= VecElem::X2 && pW <= VecElem::W2), "Invalid Permute W-Index. Must be between X1 & W2!");

	CompileTimeAssert(!((pX < VecElem::X2 && pY < VecElem::X2 && pZ < VecElem::X2 && pW < VecElem::X2) ||
		(pX > VecElem::W1 && pY > VecElem::W1 && pZ > VecElem::W1 && pW > VecElem::W1)), "All Components Are From The Same Vector, Use 'Permute(Vector_In vec)' Instead");

	if((pX < VecElem::X2 && pY < VecElem::X2) &&			// Permute<Vec1, Vec1, Vec2, Vec2>
	   (pZ > VecElem::W1 && pW > VecElem::W1))
	{
		return _mm_shuffle_ps(lhs, rhs, PERM_SHUFFLE(pX, pY, pZ, pW));
	}
	else if((pX > VecElem::W1 && pY > VecElem::W1) &&		// Permute<Vec2, Vec2, Vec1, Vec1>
			(pZ < VecElem::X2 && pW < VecElem::X2))
	{
		return _mm_shuffle_ps(rhs, lhs, PERM_SHUFFLE(pX, pY, pZ, pW));
	}
	else if((pX < VecElem::X2 && pY < VecElem::X2) &&		// Permute<Vec1, Vec1, Vec1, Vec2>
			(pZ < VecElem::X2 && pW > VecElem::W1))
	{
		Vector tmpRhs = _mm_shuffle_ps(lhs, rhs, PERM_SHUFFLE(pZ, pZ, pW, pW));
		return _mm_shuffle_ps(lhs, tmpRhs, PERM_SHUFFLE(pX, pY, 0, 2));
	}
	else if((pX > VecElem::W1 && pY > VecElem::W1) &&		// Permute<Vec2, Vec2, Vec2, Vec1>
			(pZ > VecElem::W1 && pW < VecElem::X2))
	{
		Vector tmpRhs = _mm_shuffle_ps(rhs, lhs, PERM_SHUFFLE(pZ, pZ, pW, pW));
		return _mm_shuffle_ps(rhs, tmpRhs, PERM_SHUFFLE(pX, pY, 0, 2));
	}
	else if((pX < VecElem::X2 && pY > VecElem::W1) &&		// Permute<Vec1, Vec2, Vec2, Vec2>
		    (pZ > VecElem::W1 && pW > VecElem::W1))
	{
		Vector tmpLhs = _mm_shuffle_ps(lhs, rhs, PERM_SHUFFLE(pX, pX, pY, pY));
		return _mm_shuffle_ps(tmpLhs, rhs, PERM_SHUFFLE(0, 2, pZ, pW));
	}
	else if((pX > VecElem::W1 && pY < VecElem::X2) &&		// Permute<Vec2, Vec1, Vec1, Vec1>
		    (pZ < VecElem::X2 && pW < VecElem::X2))
	{
		Vector tmpLhs = _mm_shuffle_ps(rhs, lhs, PERM_SHUFFLE(pX, pX, pY, pY));
		return _mm_shuffle_ps(tmpLhs, lhs, PERM_SHUFFLE(0, 2, pZ, pW));
	}
	else if((pX < VecElem::X2 && pZ < VecElem::X2) &&		// Permute<Vec1, Vec2, Vec1, Vec2>
			(pY > VecElem::W1 && pW > VecElem::W1))
	{
		Vector tmpLhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pZ), (VecElem)SHUFFLE_MASKED(pZ)>(lhs);
		Vector tmpRhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pW), (VecElem)SHUFFLE_MASKED(pW)>(rhs);
		return _mm_blend_ps(tmpLhs, tmpRhs, BLEND_MASK(0, 1, 0, 1));
	}
	else if((pX > VecElem::W1 && pZ > VecElem::W1) &&		// Permute<Vec2, Vec1, Vec2, Vec1>
			(pY < VecElem::X2 && pW < VecElem::X2))
	{
		Vector tmpLhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pZ), (VecElem)SHUFFLE_MASKED(pZ)>(rhs);
		Vector tmpRhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pW), (VecElem)SHUFFLE_MASKED(pW)>(lhs);
		return _mm_blend_ps(tmpLhs, tmpRhs, BLEND_MASK(0, 1, 0, 1));
	}
	else if((pX < VecElem::X2 && pW < VecElem::X2) &&		// Permute<Vec1, Vec2, Vec2, Vec1>
			(pY > VecElem::W1 && pZ > VecElem::W1))
	{
		Vector tmpLhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pW), (VecElem)SHUFFLE_MASKED(pW)>(lhs);
		Vector tmpRhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pZ), (VecElem)SHUFFLE_MASKED(pZ)>(rhs);
		return _mm_blend_ps(tmpLhs, tmpRhs, BLEND_MASK(0, 1, 1, 0));
	}
	else if((pX > VecElem::W1 && pW > VecElem::W1) &&		// Permute<Vec2, Vec1, Vec1, Vec2>
			(pY < VecElem::X2 && pZ < VecElem::X2))
	{
		Vector tmpLhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pX), (VecElem)SHUFFLE_MASKED(pW), (VecElem)SHUFFLE_MASKED(pW)>(rhs);
		Vector tmpRhs = VectorPermute<(VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pY), (VecElem)SHUFFLE_MASKED(pZ), (VecElem)SHUFFLE_MASKED(pZ)>(lhs);
		return _mm_blend_ps(tmpLhs, tmpRhs, BLEND_MASK(0, 1, 1, 0));
	}
	else if((pX < VecElem::X2 && pY > VecElem::W1) &&		// Permute<Vec1, Vec2, Vec1, Vec1>
		    (pZ < VecElem::X2 && pW < VecElem::X2))
	{
		Vector tmpLhs = _mm_shuffle_ps(lhs, rhs, PERM_SHUFFLE(pX, pX, pY, pY));
		return _mm_shuffle_ps(tmpLhs, lhs, PERM_SHUFFLE(0, 2, pZ, pW));
	}
	else if((pX > VecElem::W1 && pY < VecElem::X2) &&		// Permute<Vec2, Vec1, Vec2, Vec2>
		    (pZ > VecElem::W1 && pW > VecElem::W1))
	{
		Vector tmpLhs = _mm_shuffle_ps(rhs, lhs, PERM_SHUFFLE(pX, pX, pY, pY));
		return _mm_shuffle_ps(tmpLhs, rhs, PERM_SHUFFLE(0, 2, pZ, pW));
	}
	else if((pX < VecElem::X2 && pY < VecElem::X2) &&		// Permute<Vec1, Vec1, Vec2, Vec1>
			(pZ > VecElem::W1 && pW < VecElem::X2))
	{
		Vector tmpRhs = _mm_shuffle_ps(rhs, lhs, PERM_SHUFFLE(pZ, pZ, pW, pW));
		return _mm_shuffle_ps(lhs, tmpRhs, PERM_SHUFFLE(pX, pY, 0, 2));
	}
	else if((pX > VecElem::W1 && pY > VecElem::W1) &&		// Permute<Vec2, Vec2, Vec1, Vec2>
			(pZ < VecElem::X2 && pW > VecElem::W1))
	{
		Vector tmpRhs = _mm_shuffle_ps(lhs, rhs, PERM_SHUFFLE(pZ, pZ, pW, pW));
		return _mm_shuffle_ps(rhs, tmpRhs, PERM_SHUFFLE(pX, pY, 0, 2));
	}
	else
	{
		//CompileTimeAssert(false, "Unhandled Permute Case!");
		return _mm_setzero_ps();
	}
}

#undef PERM_SHUFFLE
#undef SHUFFLE_MASKED

template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z1, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 0, 0, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y2, VecElem::Z1, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 1, 0, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y2, VecElem::Z2, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 1, 1, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y2, VecElem::Z1, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 1, 0, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z2, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 0, 1, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z2, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 0, 1, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z1, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(1, 0, 0, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 1, 0, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z2, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 1, 1, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z2, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 1, 1, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 1, 0, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 0, 1, 0));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 0, 1, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_blend_ps(lhs, rhs, BLEND_MASK(0, 0, 0, 1));}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::X2, VecElem::Y1, VecElem::Y2>(Vector_In lhs, Vector_In rhs) {return _mm_unpacklo_ps(lhs, rhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::X1, VecElem::Y2, VecElem::Y1>(Vector_In lhs, Vector_In rhs) {return _mm_unpacklo_ps(rhs, lhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::Z1, VecElem::Z2, VecElem::W1, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_unpackhi_ps(lhs, rhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::Z2, VecElem::Z1, VecElem::W2, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_unpackhi_ps(rhs, lhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::Z2, VecElem::W2, VecElem::Z1, VecElem::W1>(Vector_In lhs, Vector_In rhs) {return _mm_movehl_ps(lhs, rhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::Z1, VecElem::W1, VecElem::Z2, VecElem::W2>(Vector_In lhs, Vector_In rhs) {return _mm_movehl_ps(rhs, lhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::X1, VecElem::Y1, VecElem::X2, VecElem::Y2>(Vector_In lhs, Vector_In rhs) {return _mm_movelh_ps(lhs, rhs);}
template<> __forceinline Vector_Out VectorPermute<VecElem::X2, VecElem::Y2, VecElem::X1, VecElem::Y1>(Vector_In lhs, Vector_In rhs) {return _mm_movelh_ps(rhs, lhs);}

#undef BLEND_MASK

__forceinline Vector_Out VectorMin(Vector_In lhs, Vector_In rhs)
{
	return _mm_min_ps(lhs, rhs);
}

__forceinline Vector_Out VectorMinInt(Vector_In lhs, Vector_In rhs)
{
	return VEC_INT_TO_FLOAT(_mm_min_epi32(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs)));
}

__forceinline Vector_Out VectorMax(Vector_In lhs, Vector_In rhs)
{
	return _mm_max_ps(lhs, rhs);
}

__forceinline Vector_Out VectorMaxInt(Vector_In lhs, Vector_In rhs)
{
	return VEC_INT_TO_FLOAT(_mm_max_epi32(VEC_FLOAT_TO_INT(lhs), VEC_FLOAT_TO_INT(rhs)));
}


// Vector Math Operations:

__forceinline Vector_Out VectorCrossProduct(Vector_In lhs, Vector_In rhs)
{
	Vector vec1 = VectorPermute<VecElem::Y, VecElem::Z, VecElem::X, VecElem::X>(lhs);
	Vector vec2 = VectorPermute<VecElem::Z, VecElem::X, VecElem::Y, VecElem::X>(rhs);
	Vector vec3 = VectorMultiply(vec1, vec2);

	vec1 = VectorPermute<VecElem::Z, VecElem::X, VecElem::Y, VecElem::X>(lhs);
	vec2 = VectorPermute<VecElem::Y, VecElem::Z, VecElem::X, VecElem::X>(rhs);

	return VectorSubtract(vec3, VectorMultiply(vec1, vec2));
}

__forceinline float VectorDot2(Vector_In lhs, Vector_In rhs)
{
	Vector dp = VectorMultiply(lhs, rhs);
	return VectorExtractFloat<VecElem::X>(VectorHAdd(dp, dp));
}

__forceinline float VectorDot3(Vector_In lhs, Vector_In rhs)
{
	Vector rhsWZero = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::X2>(rhs, VectorSet(0.0f));
	Vector dp = VectorMultiply(lhs, rhsWZero);
	Vector result = VectorHAdd(dp, dp);
	return VectorExtractFloat<VecElem::X>(VectorHAdd(result, result));
}

__forceinline float VectorDot4(Vector_In lhs, Vector_In rhs)
{
	Vector dp = VectorMultiply(lhs, rhs);
	Vector result = VectorHAdd(dp, dp);
	return VectorExtractFloat<VecElem::X>(VectorHAdd(result, result));
}

//#endif // SSE_AVAILABLE