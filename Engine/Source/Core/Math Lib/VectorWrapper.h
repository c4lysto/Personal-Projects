#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H

//#if SSE_AVAILABLE

#include <xmmintrin.h>
#include <emmintrin.h>

#define __M128_INT_TO_FLOAT _mm_castsi128_ps


enum VectSlotValidEnum
{
	VS_INVALID = 0,
	VS_VALID = 0xFFFFFFFF,
};

template<int XMask, int YMask, int ZMask, int WMask>
__m128 VectorAdd(const __m128 lhs, const __m128 rhs);

template<int XMask, int YMask, int ZMask, int WMask>
__m128 VectorSubtract(const __m128 lhs, const __m128 rhs);

template<int XMask, int YMask, int ZMask, int WMask>
__m128 VectorMultiply(const __m128 lhs, const __m128 rhs);

template<int XMask, int YMask, int ZMask, int WMask>
__m128 VectorDivide(const __m128 lhs, const __m128 rhs);

#include "VectorWrapper.inl"

#undef __M128_INT_TO_FLOAT

//#endif // SSE_AVAILABLE

#endif // VECTORWRAPPER_H