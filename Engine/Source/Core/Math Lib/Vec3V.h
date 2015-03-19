#ifndef VEC3V_H
#define VEC3V_H

#include "MathDeclarations.h"

#if SSE_AVAILABLE

#include "ScalarV.h"

typedef Vec3V& Vec3V_Ref;
typedef const Vec3V& Vec3V_ConstRef;

#if _WIN64
typedef Vec3V Vec3V_In;
#else
typedef Vec3V_ConstRef Vec3V_In;
#endif

typedef Vec3V Vec3V_Out;

typedef Vec3V_Ref Vec3V_InOut;

// Other Vec3V Aliases
typedef Vec3V float3V;

ALIGN(16) class Vec3V
{
#define DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR(enumeration, xVal, yVal, zVal)\
	explicit __forceinline Vec3V(enumeration) { row = VectorSet(xVal, yVal, zVal, VEC_FILL_VAL); }

#define DEFINE_VEC3V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec3V(enumeration) { row = VectorSet(valueToInit); }

#define DEFINE_VEC3V_ENUM_VEC_CONSTRUCTOR(enumeration, vec)\
	explicit __forceinline Vec3V(enumeration) { row = vec.row; }

#define VEC3V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC3V_ACCESSOR_SCALARV_CONST(element) \
	__forceinline ScalarV_Out Get##element () const \
	{ return ScalarV(VectorPermute<VecElem::##element , VecElem::##element , VecElem::##element , VecElem::##element >(row)); }

#define VEC3V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC3V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }

	friend class Vec4V;

private:
	union
	{
		float floatArr[4];
		Vector row;

		union
		{
			struct
			{
				float x, y, z, w;
			};
		};
	};

public:
	Vec3V(){}
	explicit Vec3V(const float& fVal);
	explicit Vec3V(const float& fX, const float& fY, const float& fZ);
	explicit Vec3V(ScalarV_In vVal);
	explicit Vec3V(ScalarV_In vX, ScalarV_In vY, ScalarV_In vZ);
	explicit Vec3V(Vec2V_In vXY, ScalarV_In fZ);
	explicit Vec3V(ScalarV_In fX, Vec2V_In vYZ);
	//Vec3V(Vec3V_In vVector);
#if !_WIN64
	Vec3V(Vec3V&& vVector);
#endif // !_WIN_64
	explicit Vec3V(Vector_In vVector);
#if !_WIN64
	Vec3V(Vector&& vVector);
#endif // !_WIN64

#ifdef DEFINE_VEC3V_ENUM_CONSTRUCTOR
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eTwoPIInitializer, TWO_PI)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
	DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR(eXAxisInitializer, 1.0f, 0.0f, 0.0f)
	DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR(eYAxisInitializer, 0.0f, 1.0f, 0.0f)
	DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR(eZAxisInitializer, 0.0f, 0.0f, 1.0f)
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eWAxisInitializer, 0.0f)
	DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR(eUpAxisInitializer, 0.0f, 1.0f, 0.0f)
#undef DEFINE_VEC3V_ENUM_CONSTRUCTOR
#undef DEFINE_VEC3V_ENUM_VAL_CONSTRUCTOR
#endif //DEFINE_VEC3_ENUM_CONSTRUCTOR

#if defined(VEC3V_ACCESSOR) && defined(VEC3V_ACCESSOR_CONST) && defined(VEC3V_ACCESSOR_SCALARV_CONST)
	VEC3V_ACCESSOR_SCALARV_CONST(X)
	VEC3V_ACCESSOR_SCALARV_CONST(Y)
	VEC3V_ACCESSOR_SCALARV_CONST(Z)
	VEC3V_ACCESSOR_SCALARV_CONST(W)
	VEC3V_ACCESSOR_CONST(const float&, GetXRef, x)
	VEC3V_ACCESSOR_CONST(const float&, GetYRef, y)
	VEC3V_ACCESSOR_CONST(const float&, GetZRef, z)
	VEC3V_ACCESSOR_CONST(const float&, GetWRef, w)

	// Returns the Vector Intrinsic Data Type
	VEC3V_ACCESSOR_CONST(Vector_Out, GetVector, row)

	VEC3V_ACCESSOR(float&, GetXRef, x)
	VEC3V_ACCESSOR(float&, GetYRef, y)
	VEC3V_ACCESSOR(float&, GetZRef, z)
	VEC3V_ACCESSOR(float&, GetWRef, w)
#undef VEC3V_ACCESSOR
#undef VEC3V_ACCESSOR_CONST
#undef VEC3V_ACCESSOR_SCALARV_CONST
#else
#error VEC3V ACCESSORS NOT DEFINED
#endif

#if defined(VEC3V_MUTATOR)
	void SetX(ScalarV_In vX);
	void SetY(ScalarV_In vY);
	void SetZ(ScalarV_In vZ);
	void SetW(ScalarV_In vW);

	VEC3V_MUTATOR(SetX, const float&, x)
	VEC3V_MUTATOR(SetY, const float&, y)
	VEC3V_MUTATOR(SetZ, const float&, z)
	VEC3V_MUTATOR(SetW, const float&, w)
#undef VEC3V_MUTATOR
#else
#error VEC3V MUTATORS NOT DEFINED
#endif

	Vec3V operator-() const;

	Vec3V_Out operator=(Vec3V_In vVector);
#if !_WIN64
	Vec3V_Out operator=(Vec3V&& vVector);
#endif // !_WIN64
	Vec3V_Out operator=(Vector_In vVector);

	Vec3V_Out operator*=(ScalarV_In vScalar);
	Vec3V_Out operator*=(Vec3V_In rhs);

	Vec3V operator/(ScalarV_In vScalar) const;
	Vec3V operator/(Vec3V_In rhs) const;

	Vec3V_Out operator/=(ScalarV_In vScalar);
	Vec3V_Out operator/=(Vec3V_In rhs);

	Vec3V operator*(ScalarV_In vScalar) const;
	friend Vec3V operator*(ScalarV_Ref vScalar, Vec3V_In vVector);
	Vec3V operator*(Vec3V_In rhs) const;

	Vec3V_Out operator+=(Vec3V_In vVector);
	Vec3V operator+(Vec3V_In vVector) const;

	Vec3V_Out operator-=(Vec3V_In vVector);
	Vec3V operator-(Vec3V_In vVector) const;

	bool operator==(Vec3V_In vVector) const;

	bool operator!=(Vec3V_In vVector) const;

	Vec3V_Out operator&(Vec3V_In vVector) const;
	Vec3V_Out operator&=(Vec3V_In vVector);

	Vec3V_Out operator|(Vec3V_In vVector) const;
	Vec3V_Out operator|=(Vec3V_In vVector);

	Vec3V_Out operator^(Vec3V_In vVector) const;
	Vec3V_Out operator^=(Vec3V_In vVector);

	Vec3V_Out operator~() const;

	const float& operator[](int index) const;
	float& operator[](int index);

	Vec3V_Out Normalize();
};

ScalarV Dot(Vec3V_In lhs, Vec3V_In rhs);

ScalarV Mag(Vec3V_In vVector);
ScalarV Length(Vec3V_In vVector);
ScalarV MagSq(Vec3V_In vVector);
ScalarV LengthSq(Vec3V_In vVector);

Vec3V_Out Normalize(Vec3V_In vVector);
Vec3V_Out Cross(Vec3V_In vVectorA, Vec3V_In vVectorB);

Vec3V_Out Vec3VInt(int intVal);
Vec3V_Out Vec3VInt(int intX, int intY, int intZ);

Vec3V_Out Vec3VIntToFloat(Vec3V_In vec);
Vec3V_Out Vec3VFloatToInt(Vec3V_In vec);


GLOBALCONST Vec3V g_IdentityX3V = Vec3V(I_X_AXIS);
GLOBALCONST Vec3V g_IdentityY3V = Vec3V(I_Y_AXIS);
GLOBALCONST Vec3V g_IdentityZ3V = Vec3V(I_Z_AXIS);
GLOBALCONST Vec3V g_IdentityW3V	= Vec3V(I_ZERO);
GLOBALCONST Vec3V g_WorldUp3V	= Vec3V(I_UP_AXIS);

#include "Vec3V.inl"

#endif // SSE_AVAILABLE

#endif // VEC3V_H