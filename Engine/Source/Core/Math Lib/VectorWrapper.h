#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H

//#if SSE_AVAILABLE
#include "../Utilities/GlobalDefines.h"

#include <xmmintrin.h>
#include <intrin.h>

#define VEC_INT_TO_FLOAT _mm_castsi128_ps
#define VEC_FLOAT_TO_INT _mm_castps_si128

typedef __m128 Vector;
typedef Vector& Vector_Ref;
typedef const Vector& Vector_ConstRef;

#if _WIN64
	typedef Vector Vector_In;
#else
	typedef Vector_ConstRef Vector_In;
#endif

typedef Vector Vector_Out;

namespace VecElem
{
	// Used For Permutations And Indexing Into a Vector
	SELECTANY const int X = 0x0;
	SELECTANY const int Y = 0x1;
	SELECTANY const int Z = 0x2;
	SELECTANY const int W = 0x3;

	SELECTANY const int X1 = X;
	SELECTANY const int Y1 = Y;
	SELECTANY const int Z1 = Z;
	SELECTANY const int W1 = W;
	SELECTANY const int X2 = (0x10 | X1);
	SELECTANY const int Y2 = (0x10 | Y1);
	SELECTANY const int Z2 = (0x10 | Z1);
	SELECTANY const int W2 = (0x10 | W1);
};

// Initialization Operations:
Vector_Out VectorSet(float fVal);

Vector_Out VectorSet(float fX, float fY, float fZ, float fW);

Vector_Out VectorSet(int iVal);

Vector_Out VectorSet(int iX, int iY, int iZ, int iW);

// Used to load Aligned Data
Vector_Out VectorLoad(const float* const alignedFloat4Ptr);

// Used to load Unaligned Data
Vector_Out VectorLoadU(const float* const unalignedFloat4Ptr);

// Used to store in Aligned Data
void VectorStore(Vector_In vec, float* alignedFloat4Ptr);

// Used to store in Unaligned Data
void VectorStoreU(Vector_In vec, float* unalignedFloat4Ptr);

template<int index>
float VectorExtract(Vector_In vec);


// Arithmetic Operations: 

Vector_Out VectorAdd(Vector_In lhs, Vector_In rhs);
Vector_Out operator+(Vector_In lhs, Vector_In rhs);

Vector_Out VectorAddInt(Vector_In lhs, Vector_In rhs);

// Operates in the following manner:
// {lhs.x+lhs.y, lhs.z+lhs.w, rhs.x+rhs.y, rhs.z+rhs.w}
Vector_Out VectorHAdd(Vector_In lhs, Vector_In rhs);

Vector_Out VectorSubtract(Vector_In lhs, Vector_In rhs);
Vector_Out operator-(Vector_In lhs, Vector_In rhs);

Vector_Out VectorSubtractInt(Vector_In lhs, Vector_In rhs);

// Operates in the following manner:
// {lhs.x-lhs.y, lhs.z-lhs.w, rhs.x-rhs.y, rhs.z-rhs.w}
Vector_Out VectorHSub(Vector_In lhs, Vector_In rhs);

Vector_Out VectorMultiply(Vector_In lhs, Vector_In rhs);
Vector_Out operator*(Vector_In lhs, Vector_In rhs);

Vector_Out VectorMultiplyInt(Vector_In lhs, Vector_In rhs);

Vector_Out VectorDivide(Vector_In lhs, Vector_In rhs);
Vector_Out operator/(Vector_In lhs, Vector_In rhs);

Vector_Out VectorNegate(Vector_In vec);
Vector_Out operator-(Vector_In vec);

Vector_Out VectorAbs(Vector_In vec);

Vector_Out Sqrt(Vector_In vec);

Vector_Out RSqrt(Vector_In vec);

Vector_Out Recip(Vector_In vec);


// Logical Operations:

Vector_Out And(Vector_In lhs, Vector_In rhs);
Vector_Out operator&(Vector_In lhs, Vector_In rhs);

// rhs is Negated Before the And so: result = lhs & ~rhs;
Vector_Out AndNot(Vector_In lhs, Vector_In rhs);

Vector_Out Not(Vector_In vec);
Vector_Out operator~(Vector_In vec);

Vector_Out Or(Vector_In lhs, Vector_In rhs);
Vector_Out operator|(Vector_In lhs, Vector_In rhs);

Vector_Out XOr(Vector_In lhs, Vector_In rhs);
Vector_Out operator^(Vector_In lhs, Vector_In rhs);

Vector_Out LeftShift(Vector_In vec, int nCount);
Vector_Out operator<<(Vector_In vec, int nCount);

Vector_Out RightShift(Vector_In vec, int nCount);
Vector_Out operator>>(Vector_In vec, int nCount);


// Comparison Functions:

// Floating Point Comparisons:
#define VEC_CMP_DECL(name) bool name(Vector_In lhs, Vector_In rhs);

#define VEC_CMP_DECLBASE(name)	int name(Vector_In lhs, Vector_In rhs);
#define VEC_CMP_DECL1(name) VEC_CMP_DECL( name##X )
#define VEC_CMP_DECL2(name) VEC_CMP_DECL( name##XY )
#define VEC_CMP_DECL3(name) VEC_CMP_DECL( name##XYZ )
#define VEC_CMP_DECL4(name) VEC_CMP_DECL( name##XYZW )

#define VEC_CMP_DECL_ALL(name)  \
			VEC_CMP_DECLBASE(name) \
			VEC_CMP_DECL1(name) \
			VEC_CMP_DECL2(name) \
			VEC_CMP_DECL3(name) \
			VEC_CMP_DECL4(name)

VEC_CMP_DECL_ALL(IsEqual);
VEC_CMP_DECL_ALL(IsNotEqual);
VEC_CMP_DECL_ALL(IsGreaterThan);
VEC_CMP_DECL_ALL(IsGreaterThanOrEqual);
VEC_CMP_DECL_ALL(IsLessThan);
VEC_CMP_DECL_ALL(IsLessThanOrEqual);


// Integer Comparisons:
VEC_CMP_DECL_ALL(IsEqualInt);
VEC_CMP_DECL_ALL(IsGreaterThanInt);
VEC_CMP_DECL_ALL(IsLessThanInt);

VEC_CMP_DECL_ALL(IsNotEqualInt);

VEC_CMP_DECL_ALL(IsGreaterThanOrEqualInt);

VEC_CMP_DECL_ALL(IsLessThanOrEqualInt);

#undef VEC_CMP_DECL_ALL
#undef VEC_CMP_DECL4
#undef VEC_CMP_DECL3
#undef VEC_CMP_DECL2
#undef VEC_CMP_DECL1
#undef VEC_CMP_DECL
#undef VEC_CMP_DECL


// Misc Operations:
//template<int pX, int pY, int pZ, int pW>
//Vector_Out Permute(Vector_In lhs, Vector_In rhs);

template<int pX, int pY, int pZ, int pW>
Vector_Out Permute(Vector_In vec);

template<int pX, int pY, int pZ, int pW>
Vector_Out Permute(Vector_In lhs, Vector_In rhs);

Vector_Out Min(Vector_In lhs, Vector_In rhs);
Vector_Out MinInt(Vector_In lhs, Vector_In rhs);

Vector_Out Max(Vector_In lhs, Vector_In rhs);
Vector_Out MaxInt(Vector_In lhs, Vector_In rhs);

#include "VectorWrapper.inl"

#undef VEC_INT_TO_FLOAT
#undef VEC_FLOAT_TO_INT

//#endif // SSE_AVAILABLE

#endif // VECTORWRAPPER_H