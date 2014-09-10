
//#if SSE_AVAILABLE

template<int XMask, int YMask, int ZMask, int WMask>
__forceinline __m128 VectorAdd(const __m128 lhs, const __m128 rhs)
{
	Assert(XMask == VS_INVALID || XMask == VS_VALID, "VectorAdd() - XMask is INVALID!!!");
	Assert(YMask == VS_INVALID || YMask == VS_VALID, "VectorAdd() - YMask is INVALID!!!");
	Assert(ZMask == VS_INVALID || ZMask == VS_VALID, "VectorAdd() - ZMask is INVALID!!!");
	Assert(WMask == VS_INVALID || WMask == VS_VALID, "VectorAdd() - WMask is INVALID!!!");

	__m128 addVal = _mm_and_ps(rhs, __M128_INT_TO_FLOAT(_mm_setr_epi32(XMask, YMask, ZMask, WMask)));
	return _mm_add_ps(lhs, addVal);
}

template<int XMask, int YMask, int ZMask, int WMask>
__forceinline __m128 VectorSubtract(const __m128 lhs, const __m128 rhs)
{
	Assert(XMask == VS_INVALID || XMask == VS_VALID, "VectorSubtract() - XMask is INVALID!!!");
	Assert(YMask == VS_INVALID || YMask == VS_VALID, "VectorSubtract() - YMask is INVALID!!!");
	Assert(ZMask == VS_INVALID || ZMask == VS_VALID, "VectorSubtract() - ZMask is INVALID!!!");
	Assert(WMask == VS_INVALID || WMask == VS_VALID, "VectorSubtract() - WMask is INVALID!!!");

	__m128 subVal = _mm_and_ps(rhs, __M128_INT_TO_FLOAT(_mm_setr_epi32(XMask, YMask, ZMask, WMask)));
	return _mm_sub_ps(lhs, subVal);
}

template<int XMask, int YMask, int ZMask, int WMask>
__forceinline __m128 VectorMultiply(const __m128 lhs, const __m128 rhs)
{
	Assert(XMask == VS_INVALID || XMask == VS_VALID, "VectorMultiply() - XMask is INVALID!!!");
	Assert(YMask == VS_INVALID || YMask == VS_VALID, "VectorMultiply() - YMask is INVALID!!!");
	Assert(ZMask == VS_INVALID || ZMask == VS_VALID, "VectorMultiply() - ZMask is INVALID!!!");
	Assert(WMask == VS_INVALID || WMask == VS_VALID, "VectorMultiply() - WMask is INVALID!!!");

	__m128 maskVal = __M128_INT_TO_FLOAT(_mm_setr_epi32(XMask, YMask, ZMask, WMask));
	__m128 vecOne = _mm_set1_ps(1.0f);

	// Mask out valid and Invalid
	__m128 mulVal = _mm_and_ps(rhs, maskVal);

	// Setup Valid Components
	mulVal = _mm_or_ps(_mm_andnot_ps(maskVal, vecOne), mulVal);

	return _mm_mul_ps(lhs, mulVal);
}

template<int XMask, int YMask, int ZMask, int WMask>
__forceinline __m128 VectorDivide(const __m128 lhs, const __m128 rhs)
{
	Assert(XMask == VS_INVALID || XMask == VS_VALID, "VectorDivide() - XMask is INVALID!!!");
	Assert(YMask == VS_INVALID || YMask == VS_VALID, "VectorDivide() - YMask is INVALID!!!");
	Assert(ZMask == VS_INVALID || ZMask == VS_VALID, "VectorDivide() - ZMask is INVALID!!!");
	Assert(WMask == VS_INVALID || WMask == VS_VALID, "VectorDivide() - WMask is INVALID!!!");

	__m128 maskVal = __M128_INT_TO_FLOAT(_mm_setr_epi32(XMask, YMask, ZMask, WMask));
	__m128 vecOne = _mm_set1_ps(1.0f);

	// Mask out valid and Invalid
	__m128 divVal = _mm_and_ps(rhs, maskVal);

	// Setup Valid Components
	divVal = _mm_or_ps(_mm_andnot_ps(maskVal, vecOne), divVal);

	return _mm_div_ps(lhs, divVal);
}

//#endif // SSE_AVAILABLE