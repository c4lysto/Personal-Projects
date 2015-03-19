#ifndef VEC2V_H
#define VEC2V_H

#include "MathDeclarations.h"
#if SSE_AVAILABLE

#include "ScalarV.h"

class Vec2V;

typedef Vec2V& Vec2V_Ref;
typedef const Vec2V& Vec2V_ConstRef;

#if _WIN64
typedef Vec2V Vec2V_In;
#else
typedef Vec2V_ConstRef Vec2V_In;
#endif

typedef Vec2V Vec2V_Out;

typedef Vec2V_Ref Vec2V_InOut;

// Other Vec2V Aliases
typedef Vec2V float2V;


ALIGN(16) class Vec2V
{
#define DEFINE_VEC2V_ENUM_VAL_CONSTRUCTOR(enumeration, xVal, yVal)\
	explicit __forceinline Vec2V(enumeration) { row = VectorSet(xVal, yVal, VEC_FILL_VAL, VEC_FILL_VAL); }

#define DEFINE_VEC2V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec2V(enumeration) { row = VectorSet(valueToInit); }

#define VEC2V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC2V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC2V_ACCESSOR_SCALARV_CONST(funcName, elementIndex) \
	__forceinline ScalarV_Out funcName() const \
		{ return ScalarV(VectorPermute< VecElem::##elementIndex , VecElem::##elementIndex , VecElem::##elementIndex , VecElem::##elementIndex >(row)); }

#define VEC2V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


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
	Vec2V(){}
	explicit Vec2V(const float& fVal);
	explicit Vec2V(const float& fX, const float& fY);
	explicit Vec2V(ScalarV_In vVal);
	explicit Vec2V(ScalarV_In vX, ScalarV_In vY);
	//Vec2V(Vec2V_In vVector);
#if !_WIN64
	Vec2V(Vec2V&& vVector);
#endif // !_WIN64
	~Vec2V(){}

	explicit Vec2V(Vector_In rhs);
#if !_WIN64
	Vec2V(Vector&& rhs);
#endif // !_WIN64

#if defined(DEFINE_VEC2V_ENUM_CONSTRUCTOR) && defined(DEFINE_VEC2V_ENUM_VAL_CONSTRUCTOR)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eTwoInitializer, 2)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eTwoPIInitializer, TWO_PI)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
	DEFINE_VEC2V_ENUM_VAL_CONSTRUCTOR(eXAxisInitializer, 1.0f, 0.0f)
	DEFINE_VEC2V_ENUM_VAL_CONSTRUCTOR(eYAxisInitializer, 0.0f, 1.0f)
#undef DEFINE_VEC2V_ENUM_CONSTRUCTOR
#undef DEFINE_VEC2V_ENUM_VAL_CONSTRUCTOR
#else
#error VEC2V ENUM CONSTRUCTORS NOT DEFINED
#endif

#if defined(VEC2V_ACCESSOR) && defined(VEC2V_ACCESSOR_CONST) && defined(VEC2V_ACCESSOR_SCALARV_CONST)
	VEC2V_ACCESSOR_SCALARV_CONST(GetX, X)
	VEC2V_ACCESSOR_SCALARV_CONST(GetY, Y)
	VEC2V_ACCESSOR_SCALARV_CONST(GetZ, Z)
	VEC2V_ACCESSOR_SCALARV_CONST(GetW, W)
	VEC2V_ACCESSOR_CONST(const float&, GetXRef, x)
	VEC2V_ACCESSOR_CONST(const float&, GetYRef, y)
	VEC2V_ACCESSOR_CONST(const float&, GetZRef, z)
	VEC2V_ACCESSOR_CONST(const float&, GetWRef, w)

	VEC2V_ACCESSOR_CONST(Vector_Out, GetVector, row)

	VEC2V_ACCESSOR(float&, GetXRef, x)
	VEC2V_ACCESSOR(float&, GetYRef, y)
	VEC2V_ACCESSOR(float&, GetZRef, z)
	VEC2V_ACCESSOR(float&, GetWRef, w)
#undef VEC2V_ACCESSOR
#undef VEC2V_ACCESSOR_CONST
#undef VEC2V_ACCESSOR_SCALARV_CONST
#else
#error VEC2V ACCESSORS NOT DEFINED
#endif

#if defined(VEC2V_MUTATOR)
	void SetX(ScalarV_In xVal);
	void SetY(ScalarV_In yVal);
	void SetZ(ScalarV_In zVal);
	void SetW(ScalarV_In wVal);

	VEC2V_MUTATOR(SetX, const float&, x)
	VEC2V_MUTATOR(SetY, const float&, y)
	VEC2V_MUTATOR(SetZ, const float&, z)
	VEC2V_MUTATOR(SetW, const float&, w)

#undef VEC2V_MUTATOR
#else
#error VEC2V MUTATORS NOT DEFINED
#endif

	Vec2V_Out operator-() const;

	Vec2V_Out operator=(Vec2V_In vVector);
#if !_WIN64
	Vec2V_Out operator=(Vec2V&& vVector);
#endif // !_WIN64

	Vec2V_Out operator+(Vec2V_In vVector) const;
	Vec2V_Out operator+=(Vec2V_In vVector);

	Vec2V_Out operator-(Vec2V_In vVector) const;
	Vec2V_Out operator-=(Vec2V_In vVector);

	Vec2V_Out operator*(Vec2V_In vVector) const;
	Vec2V_Out operator*(ScalarV_In vScalar) const;
	friend Vec2V_Out operator*(ScalarV_Ref vScalar, Vec2V_In vVector);

	Vec2V_Out operator*=(Vec2V_In vVector);
	Vec2V_Out operator*=(ScalarV_In vScalar);

	Vec2V_Out operator/(Vec2V_In vVector) const;
	Vec2V_Out operator/(ScalarV_In fScalar) const;

	Vec2V_Out operator/=(Vec2V_In vVector);
	Vec2V_Out operator/=(ScalarV_In fScalar);

	bool operator==(Vec2V_In vVector) const;
	bool operator!=(Vec2V_In vVector) const;

	Vec2V_Out operator&(Vec2V_In vVector) const;
	Vec2V_Out operator&=(Vec2V_In vVector);

	Vec2V_Out operator|(Vec2V_In vVector) const;
	Vec2V_Out operator|=(Vec2V_In vVector);

	Vec2V_Out operator^(Vec2V_In vVector) const;
	Vec2V_Out operator^=(Vec2V_In vVector);

	Vec2V_Out operator~() const;

	const float& operator[](int index) const;
	float& operator[](int index);

	Vec2V_Out Normalize();
};

Vec2V_Out Vec2VInt(int intVal);
Vec2V_Out Vec2VInt(int intX, int intY);

Vec2V_Out Vec2VIntToFloat(Vec2V_In vec);
Vec2V_Out Vec2VFloatToInt(Vec2V_In vec);

ScalarV Dot(Vec2V_In lhs, Vec2V_In rhs);

ScalarV Mag(Vec2V_In vVector);
ScalarV Length(Vec2V_In vVector);
ScalarV MagSq(Vec2V_In vVector);
ScalarV LengthSq(Vec2V_In vVector);

Vec2V_Out Normalize(Vec2V_In vVector);
Vec2V_Out Cross(Vec2V_In vVectorA, Vec2V_In vVectorB);

#include "Vec2V.inl"

#endif //SSE_AVAILABLE

#endif // VEC2V_H