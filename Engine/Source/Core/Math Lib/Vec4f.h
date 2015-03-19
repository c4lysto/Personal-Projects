#ifndef VEC4F_H
#define VEC4F_H

#include "MathDeclarations.h"
#include "Vec3f.h"

class Vec4f;

typedef Vec4f& Vec4f_Ref;
typedef const Vec4f& Vec4f_ConstRef;

typedef Vec4f_ConstRef Vec4f_In;
typedef Vec4f_Ref Vec4f_InOut;

typedef Vec4f Vec4f_Out;

// Other Vec4f Aliases
typedef Vec4f float4;

class Vec4f
{
#if SSE_AVAILABLE && 0
#define DEFINE_VEC4_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec4f(enumeration)\
	{\
		VectorStoreU(VectorSet(valueToInit), vector); \
	}
#else
#define DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(enumeration, xVal, yVal, zVal, wVal)\
	explicit __forceinline Vec4f(enumeration) : x(xVal), y(yVal), z(zVal), w(wVal){}

#define DEFINE_VEC4_ENUM_CONSTRUCTOR(enumeration, valueToInit) DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(enumeration, valueToInit, valueToInit, valueToInit, valueToInit)
#endif //SSE_AVAILABLE

#define VEC4_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC4_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC4_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }

#define VEC4_MUTATOR_MOVE(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType&& rhs) { modifiedVal = move(rhs); }

private:
	union
	{
		float vector[4];

		union
		{
			struct
			{
				s32 iX, iY, iZ, iW;
			};
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				Vec3f position; float padW;
			};
		};
	};

public:
	Vec4f(){}
	Vec4f(Vec4f_In vVector);
	Vec4f(Vec4f&& vVector);
	explicit Vec4f(const float& fVal);
	explicit Vec4f(const float& fX, const float& fY, const float& fZ, const float& fW);
	explicit Vec4f(Vec2f_In vXY, Vec2f_In vZW);
	explicit Vec4f(const float& fX, const float& fY, Vec2f_In vZW);
	explicit Vec4f(Vec2f_In vXY, const float& fZ, const float& fW);
	explicit Vec4f(const float& fX, Vec3f_In vYZW);
	explicit Vec4f(Vec3f_In vVector, const float& fA);

#if SSE_AVAILABLE
	explicit Vec4f(Vector_In vVector);
#if !_WIN64
	explicit Vec4f(Vector&& vVector);
#endif // !_WIN64
#endif //SSE_AVAILABLE

#if defined(VEC4_ACCESSOR) && defined(VEC4_ACCESSOR_CONST)
	VEC4_ACCESSOR_CONST(float, GetX, x)
	VEC4_ACCESSOR_CONST(float, GetY, y)
	VEC4_ACCESSOR_CONST(float, GetZ, z)
	VEC4_ACCESSOR_CONST(float, GetW, w)
	VEC4_ACCESSOR_CONST(Vec3f_Out, GetXYZ, position)

	VEC4_ACCESSOR_CONST(s32, GetXi, iX)
	VEC4_ACCESSOR_CONST(s32, GetYi, iY)
	VEC4_ACCESSOR_CONST(s32, GetZi, iZ)
	VEC4_ACCESSOR_CONST(s32, GetWi, iW)

	VEC4_ACCESSOR_CONST(const float&, GetXRef, x)
	VEC4_ACCESSOR_CONST(const float&, GetYRef, y)
	VEC4_ACCESSOR_CONST(const float&, GetZRef, z)
	VEC4_ACCESSOR_CONST(const float&, GetWRef, w)
	VEC4_ACCESSOR_CONST(const float*, GetVector, vector);

	VEC4_ACCESSOR_CONST(const s32&, GetXiRef, iX)
	VEC4_ACCESSOR_CONST(const s32&, GetYiRef, iY)
	VEC4_ACCESSOR_CONST(const s32&, GetZiRef, iZ)
	VEC4_ACCESSOR_CONST(const s32&, GetWiRef, iW)


	VEC4_ACCESSOR(float&, GetXRef, x)
	VEC4_ACCESSOR(float&, GetYRef, y)
	VEC4_ACCESSOR(float&, GetZRef, z)
	VEC4_ACCESSOR(float&, GetWRef, w)
	VEC4_ACCESSOR(float*, GetVector, vector)

	VEC4_ACCESSOR(s32&, GetXiRef, iX)
	VEC4_ACCESSOR(s32&, GetYiRef, iY)
	VEC4_ACCESSOR(s32&, GetZiRef, iZ)
	VEC4_ACCESSOR(s32&, GetWiRef, iW)
#undef VEC4_ACCESSOR
#undef VEC4_ACCESSOR_CONST
#else
#error VEC4 ACCESSORS NOT DEFINED
#endif

#if defined(VEC4_MUTATOR) && defined(VEC4_MUTATOR_MOVE)
	VEC4_MUTATOR(SetX, const float&, x)
	VEC4_MUTATOR(SetY, const float&, y)
	VEC4_MUTATOR(SetZ, const float&, z)
	VEC4_MUTATOR(SetW, const float&, w)
	VEC4_MUTATOR(SetXYZ, Vec3f_In, position)
	VEC4_MUTATOR_MOVE(SetXYZ, Vec3f, position)

	VEC4_MUTATOR(SetXi, const s32&, iX)
	VEC4_MUTATOR(SetYi, const s32&, iY)
	VEC4_MUTATOR(SetZi, const s32&, iZ)
	VEC4_MUTATOR(SetWi, const s32&, iW)
#undef VEC4_MUTATOR
#undef VEC4_MUTATOR_MOVE
#else
#error VEC4 MUTATORS NOT DEFINED
#endif

#ifdef DEFINE_VEC4_ENUM_CONSTRUCTOR
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eTwoPIInitializer, TWO_PI)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
	DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(eXAxisInitializer, 1.0f, 0.0f, 0.0f, 0.0f)
	DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(eYAxisInitializer, 0.0f, 1.0f, 0.0f, 0.0f)
	DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(eZAxisInitializer, 0.0f, 0.0f, 1.0f, 0.0f)
	DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(eWAxisInitializer, 0.0f, 0.0f, 0.0f, 1.0f)
	DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR(eUpAxisInitializer, 0.0f, 1.0f, 0.0f, 0.0f)
#undef DEFINE_VEC4_ENUM_CONSTRUCTOR
#undef DEFINE_VEC4_ENUM_VAL_CONSTRUCTOR
#endif //DEFINE_VEC4_ENUM_CONSTRUCTOR

	Vec3f GetXYZ() {return position;}

	Vec4f operator-();

	Vec4f_Ref operator=(Vec4f_In vVector);
	Vec4f_Ref operator=(Vec4f&& vVector);

	Vec4f operator-(Vec4f_In vVector) const;
	Vec4f_Ref operator-=(Vec4f_In vVector);

	Vec4f operator+(Vec4f_In vVector) const;
	Vec4f_Ref operator+=(Vec4f_In vVector);

	Vec4f operator/(const float& fScalar) const;
	Vec4f_Ref operator/=(const float& fScalar);

	Vec4f operator*(const float& fScalar) const;
	Vec4f_Ref operator*=(const float& fScalar);
	friend Vec4f operator*(const float& fScalar, Vec4f_In vVector);
	Vec4f operator*(Vec4f_In vVector) const;
	Vec4f_Ref operator*=(Vec4f_In vVector);

	bool operator==(Vec4f_In vVector);
	bool operator!=(Vec4f_In vVector);

	Vec4f_Out operator&(Vec4f_In vVector) const;
	Vec4f_Ref operator&=(Vec4f_In vVector);

	Vec4f_Out operator|(Vec4f_In vVector) const;
	Vec4f_Ref operator|=(Vec4f_In vVector);

	Vec4f_Out operator^(Vec4f_In vVector) const;
	Vec4f_Ref operator^=(Vec4f_In vVector);

	Vec4f_Out operator~() const;

	const float& operator[](int index) const;
	float& operator[](int index);
};

Vec4f_Out Vec4fInt(const s32& intVal);
Vec4f_Out Vec4fInt(const s32& intX, const s32& intY, const s32& intZ, const s32& intW);


GLOBALCONST Vec4f g_IdentityX4 = Vec4f(I_X_AXIS);
GLOBALCONST Vec4f g_IdentityY4 = Vec4f(I_Y_AXIS);
GLOBALCONST Vec4f g_IdentityZ4 = Vec4f(I_Z_AXIS);
GLOBALCONST Vec4f g_IdentityW4 = Vec4f(I_W_AXIS);
GLOBALCONST Vec4f g_WorldUp4   = Vec4f(I_UP_AXIS);

float Dot(Vec4f_In lhs, Vec4f_In rhs);

float Mag(Vec4f_In vVector);
float Length(Vec4f_In vVector);
float MagSq(Vec4f_In vVector);
float LengthSq(Vec4f_In vVector);

Vec4f_Out Normalize(Vec4f_In vVector);
Vec4f_Out Cross(Vec4f_In vVectorA, Vec4f_In vVectorB);

#include "Vec4f.inl"

#endif // VEC4F_H