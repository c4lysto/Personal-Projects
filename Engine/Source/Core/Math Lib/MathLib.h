#ifndef MATHLIB_H
#define MATHLIB_H

#include "MathDeclarations.h"

#if SSE_AVAILABLE
#include "VectorWrapper.h"
#endif

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Matrix44.inl"

#if SSE_AVAILABLE

#include "Vec2V.inl"
#include "Vec3V.inl"
#include "Vec4V.inl"
#include "Mat44V.inl"
#endif

#include "GlobalMathFuncs.inl"

#endif //MATHLIB_H