#ifndef MATHDECLARATIONS_H
#define MATHDECLARATIONS_H

#include <Windows.h>

#include <DirectXMath.h>
using namespace DirectX;

#ifndef SSE_AVAILABLE
#if defined(_M_IX86) || defined(_M_AMD64)
#define SSE_AVAILABLE 1
#else
#define SSE_AVAILABLE 0
#endif
#endif // SSE_AVAILABLE

// Initializer Enums
enum eZeroInitializer { INIT_ZERO };
enum eOneInitializer { INIT_ONE };
enum eTwoInitializer { INIT_TWO };
enum eThreeInitializer { INIT_THREE };
enum eFourInitializer { INIT_FOUR };
enum eFiveInitializer { INIT_FIVE };
enum eSixInitializer { INIT_SIX };
enum eSevenInitializer { INIT_SEVEN };
enum eEightInitializer { INIT_EIGHT };
enum eNineInitializer { INIT_NINE };
enum eTenInitializer { INIT_TEN };
enum ePIInitializer { INIT_PI };
enum eTwoPIInitializer { INIT_TWOPI };
enum eQuarterInitializer {INIT_QUARTER};
enum eHalfInitializer {INIT_HALF};
enum eIdentityInitializer { INIT_IDENTITY };

#define FLOAT_AS_INT_REP(valName, value) \
	extern const __declspec(selectany) int FLT_##valName##_AS_INT = value;

FLOAT_AS_INT_REP(ZERO, 0x0)
FLOAT_AS_INT_REP(ONE, 0x3F800000)
FLOAT_AS_INT_REP(NAN, 0xFFFFFFFF)


#undef FLOAT_AS_INT_REP

template<typename Type> class Vec2;
class Vec3f;
class Vec4f;
class Mat44;

#if SSE_AVAILABLE
class Vec2V;
class Vec3V;
class Vec4V;
class Mat44V;

// Conversion Macros - Start
// Normal to Vectorized
#define VEC2F_TO_VEC2V(x) (Vec2V(x.GetX(), x.GetY()))
#define VEC3F_TO_VEC3V(x) (Vec3V(x.GetX(), x.GetY(), x.GetZ()))
#define VEC4F_TO_VEC4V(x) (Vec4V(x.GetX(), x.GetY(), x.GetZ(), x.GetW()))
#define MAT44_TO_MAT44V(x) (Mat44V(	VEC4F_TO_VEC4V(x.GetXAxis()), \
									VEC4F_TO_VEC4V(x.GetYAxis()), \
									VEC4F_TO_VEC4V(x.GetZAxis()), \
									VEC4F_TO_VEC4V(x.GetWAxis())))

// Vectorized to Normal
#define VEC2V_TO_VEC2F(x) (Vec2f(*(Vec2f*)&(x)))
#define VEC3V_TO_VEC3F(x) (Vec3f(*(Vec3f*)&(x)))
#define VEC4V_TO_VEC4F(x) (Vec4f(*(Vec4f*)&(x)))
#define MAT44V_TO_MAT44(x) (Mat44(*(Mat44*)&(x)))

// Conversion Macros - End

#ifndef VEC_FILL_VAL
#define VEC_FILL_VAL (0.0f)
#endif //VEC_FILL_VAL
#else
#define Vec2V Vec2f
#define Vec3V Vec3f
#define Vec4V Vec4f
#define Mat44V Mat44

// Conversion Macros - Start
// Normal to Vectorized
#define VEC2F_TO_VEC2V(x) Vec2f(x)
#define VEC3F_TO_VEC3V(x) Vec3f(x)
#define VEC4F_TO_VEC4V(x) Vec4f(x)
#define MAT44_TO_MAT44V(x) Mat44(x)

// Vectorized to Normal
#define VEC2V_TO_VEC2F(x) VEC2F_TO_VEC2V(x)
#define VEC3V_TO_VEC3F(x) VEC3F_TO_VEC3V(x)
#define VEC4V_TO_VEC4F(x) VEC4F_TO_VEC4V(x)
#define MAT44V_TO_MAT44(x) MAT44_TO_MAT44V(x)
#endif//SSE_AVAILABLE

#if SSE_AVAILABLE
// Flipped the value around because __m128 stores the float in the opposite order that you think it does, TRUST ME!
// _MM_FSHUFFLE makes using the _mm_shuffle_ps() function more intuitive by flipping the values that it passes to _MM_SHUFFLE()
#define _MM_FSHUFFLE(fp0,fp1,fp2,fp3) _MM_SHUFFLE(fp3,fp2,fp1,fp0)
#endif

#ifndef PI
#define PI (3.1415926535897932384626433832795f)
#endif

#ifndef _2PI
#define _2PI (2.0f*PI)
#endif

#ifndef PI_OVER_2
#define PI_OVER_2 (PI*0.5f)
#endif

#ifndef PI_OVER_4
#define PI_OVER_4 (PI*0.25f)
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     (1.192092896e-07F)
#endif

#ifndef PI_OVER_180
#define PI_OVER_180 (PI/180.0f)
#endif

#ifndef _180_OVER_PI
#define _180_OVER_PI (180.0f/PI)
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS(deg) ((deg) * PI_OVER_180)
#endif

#ifndef RADIANS_TO_DEGREES
#define RADIANS_TO_DEGREES(rad) ((rad) * _180_OVER_PI)
#endif

#ifdef max
#undef max
#endif
#define max(a,b) Max(a, b)

#ifdef min
#undef min
#endif
#define min(a,b) Min(a, b)

#endif //MATHDECLARATIONS_H