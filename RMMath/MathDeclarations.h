#ifndef MATHDECLARATIONS_H
#define MATHDECLARATIONS_H

#include <Windows.h>

#include <DirectXMath.h>
using namespace DirectX;

#include <utility>
using std::move;

#if defined(_M_IX86) || defined(_M_AMD64)
#define SSE_MATH_AVAILABLE
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif

#ifndef _PI_OVER_180
#define _PI_OVER_180 0.0174532925f
#endif

#ifndef _180_OVER_PI
#define _180_OVER_PI 57.2957795131f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(deg) ((deg) * _PI_OVER_180)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG(rad) ((rad) * _180_OVER_PI)
#endif

#ifndef max
	#define max(a,b)	(((a) > (b) ? (a) : (b)))
#endif

#ifndef min
	#define min(a,b)	(((a) < (b) ? (a) : (b)))
#endif

#endif