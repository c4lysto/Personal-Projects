#ifndef MATHDECLARATIONS_H
#define MATHDECLARATIONS_H

#if WIN32 || _WIN64
#include <Windows.h>
#endif

#include <DirectXMath.h>
using DirectX::XMMATRIX;

#ifndef SSE_AVAILABLE
#if defined(_M_IX86) || defined(_M_AMD64)
#define SSE_AVAILABLE 1
#else
#define SSE_AVAILABLE 0
#endif
#endif // SSE_AVAILABLE

// Initializer Enums
enum eZeroInitializer		{ I_ZERO };
enum eOneInitializer		{ I_ONE };
enum eTwoInitializer		{ I_TWO };
enum eThreeInitializer		{ I_THREE };
enum eFourInitializer		{ I_FOUR };
enum eFiveInitializer		{ I_FIVE };
enum eSixInitializer		{ I_SIX };
enum eSevenInitializer		{ I_SEVEN };
enum eEightInitializer		{ I_EIGHT };
enum eNineInitializer		{ I_NINE };
enum eTenInitializer		{ I_TEN };
enum ePIInitializer			{ I_PI };
enum eTwoPIInitializer		{ I_TWOPI };
enum eHalfPIInitializer		{ I_HALFPI };
enum eQuarterInitializer	{ I_QUARTER };
enum eHalfInitializer		{ I_HALF };
enum eIdentityInitializer	{ I_IDENTITY };
enum eFLTMINInitializer		{ I_FLT_MIN };
enum eFLTMAXInitializer		{ I_FLT_MAX };

enum eXAxisInitializer		{ I_X_AXIS };
enum eYAxisInitializer		{ I_Y_AXIS };
enum eZAxisInitializer		{ I_Z_AXIS };
enum eWAxisInitializer		{ I_W_AXIS };

enum eUpAxisInitializer		{ I_UP_AXIS };

enum eXRotationInitializer	{ I_ROTATION_X };
enum eYRotationInitializer	{ I_ROTATION_Y };
enum eZRotationInitializer	{ I_ROTATION_Z };

enum class VecElem
{
	X = 0x0,
	Y = 0x1,
	Z = 0x2,
	W = 0x3,

	X1 = X,
	Y1 = Y,
	Z1 = Z,
	W1 = W,
	X2 = (0x10 | X1),
	Y2 = (0x10 | Y1),
	Z2 = (0x10 | Z1),
	W2 = (0x10 | W1)
};

class Vec2f;
class Vec3f;
class Vec4f;
class Mat33f;
class Mat34f;
class Mat44f;

#if SSE_AVAILABLE
class ScalarV;
class Vec2V;
class Vec3V;
class Vec4V;
class Mat33V;
class Mat34V;
class Mat44V;

// Conversion Macros - Start
// Normal to Vectorized
#define VEC2F_TO_VEC2V(x) (Vec2V(x.GetXRef(), x.GetYRef()))
#define VEC3F_TO_VEC3V(x) (Vec3V(x.GetXRef(), x.GetYRef(), x.GetZRef()))
#define VEC4F_TO_VEC4V(x) (Vec4V(x.GetXRef(), x.GetYRef(), x.GetZRef(), x.GetWRef()))

#define MAT33F_TO_MAT33V(x) (Mat33V(VEC3F_TO_VEC3V(x.GetXAxisRef()), \
									VEC3F_TO_VEC3V(x.GetYAxisRef()), \
									VEC3F_TO_VEC3V(x.GetZAxisRef()))))

#define MAT34F_TO_MAT34V(x) (Mat34V(VEC3F_TO_VEC3V(x.GetXAxisRef()), \
									VEC3F_TO_VEC3V(x.GetYAxisRef()), \
									VEC3F_TO_VEC3V(x.GetZAxisRef()), \
									VEC3F_TO_VEC3V(x.GetWAxisRef())))

#define MAT44F_TO_MAT44V(x) (Mat44V(VEC4F_TO_VEC4V(x.GetXAxisRef()), \
									VEC4F_TO_VEC4V(x.GetYAxisRef()), \
									VEC4F_TO_VEC4V(x.GetZAxisRef()), \
									VEC4F_TO_VEC4V(x.GetWAxisRef())))

// Vectorized to Normal
#define VEC2V_TO_VEC2F(x) (Vec2f(*(Vec2f*)&(x)))
#define VEC3V_TO_VEC3F(x) (Vec3f(*(Vec3f*)&(x)))
#define VEC4V_TO_VEC4F(x) (Vec4f(*(Vec4f*)&(x)))
#define MAT33V_TO_MAT33F(x) (Mat33f(*(Mat33f*)&(x)))
#define MAT34V_TO_MAT34F(x) (Mat34f(*(Mat34f*)&(x)))
#define MAT44V_TO_MAT44F(x) (Mat44f(*(Mat44f*)&(x)))

// Conversion Macros - End

#ifndef VEC_FILL_VAL
#define VEC_FILL_VAL (0.0f)
#endif //VEC_FILL_VAL
#else
#define ScalarV float
#define Vec2V Vec2f
#define Vec3V Vec3f
#define Vec4V Vec4f
#define Mat33V Mat33f
#define Mat34V Mat34f
#define Mat44V Mat44f

// Conversion Macros - Start
// Normal to Vectorized
#define VEC2F_TO_VEC2V(x) x
#define VEC3F_TO_VEC3V(x) x
#define VEC4F_TO_VEC4V(x) x
#define MAT33F_TO_MAT33V(x) x
#define MAT34F_TO_MAT34V(x) x
#define MAT44F_TO_MAT44V(x) x

// Vectorized to Normal
#define VEC2V_TO_VEC2F(x) x
#define VEC3V_TO_VEC3F(x) x
#define VEC4V_TO_VEC4F(x) x
#define MAT33V_TO_MAT33F(x) x
#define MAT34V_TO_MAT34F(x) x
#define MAT44V_TO_MAT44F(x) x
#endif//SSE_AVAILABLE

#if SSE_AVAILABLE
// Flipped the value around because __m128 stores the float in the opposite order that you think it does, TRUST ME!
// _MM_FSHUFFLE makes using the _mm_shuffle_ps() function more intuitive by flipping the values that it passes to _MM_SHUFFLE()
#define _MM_FSHUFFLE(fp0,fp1,fp2,fp3) _MM_SHUFFLE((int)(fp3),(int)(fp2),(int)(fp1),(int)(fp0))
#endif

#ifndef PI
#define PI (3.1415926535897932384626433832795f)
#endif

#ifndef TWO_PI
#define TWO_PI (2.0f*PI)
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

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif // FLT_MAX

#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F
#endif // FLT_MIN

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