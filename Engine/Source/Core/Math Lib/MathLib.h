#ifndef MATHLIB_H
#define MATHLIB_H

#include "MathDeclarations.h"

#if SSE_AVAILABLE
#include "VectorWrapper.h"
#endif

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Mat33f.h"
#include "Mat44f.h"

#if SSE_AVAILABLE
#include "ScalarV.h"
#include "Vec2V.h"
#include "Vec3V.h"
#include "Vec4V.h"
#include "Mat33V.h"
#include "Mat44V.h"
#endif

#include "GlobalMathFuncs.h"

#endif //MATHLIB_H