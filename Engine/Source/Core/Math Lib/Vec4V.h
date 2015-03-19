#ifndef VEC4V_H
#define VEC4V_H

#include "MathDeclarations.h"
#if SSE_AVAILABLE

typedef Vec4V& Vec4V_Ref;
typedef const Vec4V& Vec4V_ConstRef;

#if _WIN64
typedef Vec4V Vec4V_In;
#else
typedef Vec4V_ConstRef Vec4V_In;
#endif

typedef Vec4V Vec4V_Out;

typedef Vec4V_Ref Vec4V_InOut;

// Other Vec4V Aliases
typedef Vec4V float4V;

ALIGN(16) class Vec4V
{
#define DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(enumeration, xVal, yVal, zVal, wVal) \
	explicit __forceinline Vec4V(enumeration) { row = VectorSet(xVal, yVal, zVal, wVal); }

#define DEFINE_VEC4V_ENUM_CONSTRUCTOR(enumeration, valueToInit) \
	explicit __forceinline Vec4V(enumeration) { row = VectorSet(valueToInit); }

#define VEC4V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC4V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC4V_ACCESSOR_SCALARV_CONST(element) \
	__forceinline ScalarV_Out Get##element () const \
		{ return ScalarV(VectorPermute<VecElem::##element , VecElem::##element , VecElem::##element , VecElem::##element >(row)); }

#define VEC4V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


	friend class Mat44V;

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
	Vec4V(){}
	//Vec4V(Vec4V_In vVector);
#if !_WIN64
	Vec4V(Vec4V&& vVector);
#endif // !_WIN64
	explicit Vec4V(const float& fVal);
	explicit Vec4V(const float& fX, const float& fY, const float& fZ, const float& fW);
	explicit Vec4V(ScalarV_In vVal);
	explicit Vec4V(ScalarV_In vX, ScalarV_In vY, ScalarV_In vZ, ScalarV_In vW);
	explicit Vec4V(Vec2V_In vXY, Vec2V_In vZW);
	explicit Vec4V(ScalarV_In vX, ScalarV_In vY, Vec2V_In vZW);
	explicit Vec4V(Vec2V_In vXY, ScalarV_In vZ, ScalarV_In vW);
	explicit Vec4V(Vec3V_In vXYZ, ScalarV_In vW);
	explicit Vec4V(ScalarV_In vX, Vec3V_In vYZW);
	explicit Vec4V(Vector_In vVector);
#if !_WIN64
	Vec4V(Vector&& vVector);
#endif // !_WIN64

#if defined(VEC4V_ACCESSOR) && defined(VEC4V_ACCESSOR_CONST) && defined(VEC4V_ACCESSOR_SCALARV_CONST)
	VEC4V_ACCESSOR_SCALARV_CONST(X)
	VEC4V_ACCESSOR_SCALARV_CONST(Y)
	VEC4V_ACCESSOR_SCALARV_CONST(Z)
	VEC4V_ACCESSOR_SCALARV_CONST(W)
	VEC4V_ACCESSOR_CONST(const float&, GetXRef, x)
	VEC4V_ACCESSOR_CONST(const float&, GetYRef, y)
	VEC4V_ACCESSOR_CONST(const float&, GetZRef, z)
	VEC4V_ACCESSOR_CONST(const float&, GetWRef, w)
	__forceinline Vec3V GetXYZ() const {return Vec3V(row);}

	VEC4V_ACCESSOR_CONST(Vector_Out, GetVector, row);

	VEC4V_ACCESSOR(float&, GetXRef, x)
	VEC4V_ACCESSOR(float&, GetYRef, y)
	VEC4V_ACCESSOR(float&, GetZRef, z)
	VEC4V_ACCESSOR(float&, GetWRef, w)
#undef VEC4V_ACCESSOR
#undef VEC4V_ACCESSOR_CONST
#undef VEC4V_ACCESSOR_SCALARV_CONST
#else
#error VEC4V ACCESSORS NOT DEFINED
#endif

#if defined(VEC4V_MUTATOR)
	void SetX(ScalarV_In vX);
	void SetY(ScalarV_In vY);
	void SetZ(ScalarV_In vZ);
	void SetW(ScalarV_In vW);

	VEC4V_MUTATOR(SetX, const float&, x)
	VEC4V_MUTATOR(SetY, const float&, y)
	VEC4V_MUTATOR(SetZ, const float&, z)
	VEC4V_MUTATOR(SetW, const float&, w)

	__forceinline void SetXYZ(Vec3V_In rhs) {row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(rhs.GetVector(), row);}
	//__forceinline void SetXYZ(Vec3V&& rhs) {row = VectorSet(rhs.x, rhs.y, rhs.z, w);}

#undef VEC4V_MUTATOR
#else
#error VEC4V MUTATORS NOT DEFINED
#endif

#ifdef DEFINE_VEC4V_ENUM_CONSTRUCTOR
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eTwoPIInitializer, TWO_PI)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
	DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(eXAxisInitializer, 1.0f, 0.0f, 0.0f, 0.0f)
	DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(eYAxisInitializer, 0.0f, 1.0f, 0.0f, 0.0f)
	DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(eZAxisInitializer, 0.0f, 0.0f, 1.0f, 0.0f)
	DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(eWAxisInitializer, 0.0f, 0.0f, 0.0f, 1.0f)
	DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR(eUpAxisInitializer, 0.0f, 1.0f, 0.0f, 0.0f)
#undef DEFINE_VEC4V_ENUM_CONSTRUCTOR
#undef DEFINE_VEC4V_ENUM_VAL_CONSTRUCTOR
#endif //DEFINE_VEC4_ENUM_CONSTRUCTOR

	Vec4V operator-() const;

	Vec4V_Out operator=(Vec4V_In vVector);
#if !_WIN64
	Vec4V_Out operator=(Vec4V&& vVector);
#endif // !_WIN_64

	Vec4V operator-(Vec4V_In vVector) const;
	Vec4V_Out operator-=(Vec4V_In vVector);

	Vec4V operator+(Vec4V_In vVector) const;
	Vec4V_Out operator+=(Vec4V_In vVector);

	Vec4V operator/(ScalarV_In vScalar) const;
	Vec4V_Out operator/=(ScalarV_In vScalar);

	Vec4V operator*(ScalarV_In vScalar) const;
	Vec4V_Out operator*=(ScalarV_In vScalar);
	friend Vec4V_Out operator*(ScalarV_Ref vScalar, Vec4V_In vVector);
	Vec4V operator*(Vec4V_In vVector) const;
	Vec4V_Out operator*=(Vec4V_In vVector);

	bool operator==(Vec4V_In vVector) const;
	bool operator!=(Vec4V_In vVector) const;

	Vec4V_Out operator&(Vec4V_In vVector) const;
	Vec4V_Out operator&=(Vec4V_In vVector);

	Vec4V_Out operator|(Vec4V_In vVector) const;
	Vec4V_Out operator|=(Vec4V_In vVector);

	Vec4V_Out operator^(Vec4V_In vVector) const;
	Vec4V_Out operator^=(Vec4V_In vVector);

	Vec4V_Out operator~() const;

	const float& operator[](int index) const;
	float& operator[](int index);

	Vec4V_Out Normalize();
};

GLOBALCONST Vec4V g_IdentityX4V = Vec4V(I_X_AXIS);
GLOBALCONST Vec4V g_IdentityY4V = Vec4V(I_Y_AXIS);
GLOBALCONST Vec4V g_IdentityZ4V = Vec4V(I_Z_AXIS);
GLOBALCONST Vec4V g_IdentityW4V = Vec4V(I_W_AXIS);
GLOBALCONST Vec4V g_WorldUp4V	= Vec4V(I_UP_AXIS);

Vec4V_Out Vec4VInt(int intVal);
Vec4V_Out Vec4VInt(int intX, int intY, int intZ, int intW);

ScalarV Dot(Vec4V_In lhs, Vec4V_In rhs);

ScalarV Mag(Vec4V_In vVector);
ScalarV Length(Vec4V_In vVector);
ScalarV MagSq(Vec4V_In vVector);
ScalarV LengthSq(Vec4V_In vVector);

Vec4V_Out Normalize(Vec4V_In vVector);
Vec4V_Out Cross(Vec4V_In vVectorA, Vec4V_In vVectorB);

#include "Vec4V.inl"

#endif // SSE_AVAILABLE

#endif // VEC4V_H