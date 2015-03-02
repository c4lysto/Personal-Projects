#ifndef VEC2F_H
#define VEC2F_H

#include "MathDeclarations.h"

class Vec2f;

typedef Vec2f& Vec2f_Ref;
typedef const Vec2f& Vec2f_ConstRef;

typedef Vec2f_ConstRef Vec2f_In;
typedef Vec2f_Ref Vec2f_InOut;

typedef Vec2f Vec2f_Out;

// Other Vec2f Aliases
typedef Vec2f float2;

class Vec2f
{
#define DEFINE_VEC2_ENUM_VAL_CONSTRUCTOR(enumeration, xVal, yVal)\
	explicit __forceinline Vec2f(enumeration) : x(xVal), y(yVal) {}

#define DEFINE_VEC2_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec2f(enumeration) : x(valueToInit), y(valueToInit){}

#define VEC2_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC2_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC2_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		float vector[2];

		struct
		{
			float x, y;
		};
	};

public:

	Vec2f(){}

#ifdef	DEFINE_VEC2_ENUM_CONSTRUCTOR
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
	DEFINE_VEC2_ENUM_VAL_CONSTRUCTOR(eXAxisInitializer, 1.0f, 0.0f)
	DEFINE_VEC2_ENUM_VAL_CONSTRUCTOR(eYAxisInitializer, 0.0f, 1.0f)
#undef DEFINE_VEC2_ENUM_CONSTRUCTOR
#undef DEFINE_VEC2_ENUM_VAL_CONSTRUCTOR
#else
#error "VEC2F ENUM CONSTRUCTORS NOT DEFINED"
#endif //DEFINE_VEC2_ENUM_CONSTRUCTOR

	Vec2f(Vec2f_In vVector);
	Vec2f(Vec2f&& vVector);
	explicit Vec2f(const float& fVal);
	explicit Vec2f(const float& fX, const float& fY);

#if SSE_AVAILABLE
	explicit Vec2f(Vector_In vVector);

#if !_WIN64
	explicit Vec2f(Vector&& vVector);
#endif
#endif

#if defined(VEC2_ACCESSOR) && defined(VEC2_ACCESSOR_CONST)
	VEC2_ACCESSOR_CONST(float, GetX, x)
	VEC2_ACCESSOR_CONST(float, GetY, y)
	VEC2_ACCESSOR_CONST(const float&, GetXRef, x)
	VEC2_ACCESSOR_CONST(const float&, GetYRef, y)
	VEC2_ACCESSOR_CONST(const float*, GetVector, vector)

	VEC2_ACCESSOR(float&, GetXRef, x)
	VEC2_ACCESSOR(float&, GetYRef, y)
	VEC2_ACCESSOR(float*, GetVector, vector)
#undef VEC2_ACCESSOR
#undef VEC2_ACCESSOR_CONST
#else
#error VEC2 ACCESSORS NOT DEFINED
#endif

#if defined(VEC2_MUTATOR)
	VEC2_MUTATOR(SetX, const float&, x)
	VEC2_MUTATOR(SetY, const float&, y)
#undef VEC2_MUTATOR
#else
#error VEC2 MUTATORS NOT DEFINED
#endif

	Vec2f operator-() const;

	Vec2f_Ref operator=(Vec2f_In vVector);
	Vec2f_Ref operator=(Vec2f&& vVector);

	Vec2f operator+(Vec2f_In vVector) const;
	Vec2f_Ref operator+=(Vec2f_In vVector);

	Vec2f operator-(Vec2f_In vVector) const;
	Vec2f_Ref operator-=(Vec2f_In vVector);

	Vec2f operator*(const float& fScalar) const;
	friend Vec2f operator*(const float& fScalar, Vec2f_In vVector);
	Vec2f_Ref operator*=(const float& fScalar);

	Vec2f operator/(const float& fScalar) const;
	Vec2f_Ref operator/=(const float& fScalar);

	const float& operator[](int index) const;
	float& operator[](int index);
};

float Dot(Vec2f_In lhs, Vec2f_In rhs);

float Mag(Vec2f_In vVector);
float Length(Vec2f_In vVector);
float MagSq(Vec2f_In vVector);
float LengthSq(Vec2f_In vVector);

Vec2f_Out Normalize(Vec2f_In vVector);
Vec2f_Out Cross(Vec2f_In vVectorA, Vec2f_In vVectorB);

#include "Vec2f.inl"

#endif // VEC2F_H