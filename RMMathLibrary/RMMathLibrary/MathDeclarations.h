#ifndef MATHDECLARATIONS_H
#define MATHDECLARATIONS_H

#include <Windows.h>

#include <DirectXMath.h>
using namespace DirectX;

#include <utility>
using std::move;

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
#define SSE_MATH_AVAILABLE
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0f)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG(rad) ((rad) * 180.0f / PI)
#endif

#ifndef max
	#define max(a,b)	(((a) > (b) ? (a) : (b)))
#endif

#ifndef min
	#define min(a,b)	(((a) < (b) ? (a) : (b)))
#endif

#endif