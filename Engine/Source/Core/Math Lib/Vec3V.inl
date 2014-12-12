#if SSE_AVAILABLE
#ifndef VEC3V_INL
#define VEC3V_INL

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
#define DEFINE_VEC3V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec3V(enumeration) { row = VectorSet(valueToInit); }

#define VEC3V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC3V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC3V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }

	friend class Vec4V;

private:
	union
	{
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
	explicit Vec3V(float fX, float fY, float fZ);
	explicit Vec3V(float fX, float fY, float fZ, float fW);
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
	DEFINE_VEC3V_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC3V_ENUM_CONSTRUCTOR
#endif //DEFINE_VEC3_ENUM_CONSTRUCTOR

#if defined(VEC3V_ACCESSOR) && defined(VEC3V_ACCESSOR_CONST)
	VEC3V_ACCESSOR_CONST(float, GetX, x)
	VEC3V_ACCESSOR_CONST(float, GetY, y)
	VEC3V_ACCESSOR_CONST(float, GetZ, z)
	VEC3V_ACCESSOR_CONST(float, GetW, w)

	VEC3V_ACCESSOR(float&, GetXRef, x)
	VEC3V_ACCESSOR(float&, GetYRef, y)
	VEC3V_ACCESSOR(float&, GetZRef, z)
	VEC3V_ACCESSOR(float&, GetWRef, w)
#undef VEC3V_ACCESSOR
#undef VEC3V_ACCESSOR_CONST
#else
#error VEC3V ACCESSORS NOT DEFINED
#endif

#if defined(VEC3V_MUTATOR)
		VEC3V_MUTATOR(SetX, float, x)
		VEC3V_MUTATOR(SetY, float, y)
		VEC3V_MUTATOR(SetZ, float, z)
		VEC3V_MUTATOR(SetW, float, w)
#undef VEC3V_MUTATOR
#else
#error VEC3V MUTATORS NOT DEFINED
#endif

	Vec3V operator-() const;

	Vec3V_Out operator=(Vec3V_In vVector);
#if !_WIN64
	Vec3V_Out operator=(Vec3V&& vVector);
#endif // !_WIN64

	Vec3V_Out operator*=(float fScalar);
	Vec3V_Out operator*=(Vec3V_In rhs);

	Vec3V operator/(float fScalar) const;
	Vec3V operator/(Vec3V_In rhs) const;

	Vec3V_Out operator/=(float fScalar);
	Vec3V_Out operator/=(Vec3V_In rhs);

	Vec3V operator*(float fScalar) const;
	friend Vec3V operator*(const float fScalar, Vec3V_In vVector);
	Vec3V operator*(Vec3V_In rhs) const;

	Vec3V_Out operator+=(Vec3V_In vVector);
	Vec3V operator+(Vec3V_In vVector) const;

	Vec3V_Out operator-=(Vec3V_In vVector);
	Vec3V operator-(Vec3V_In vVector) const;

	bool operator==(Vec3V_In vVector) const;

	bool operator!=(Vec3V_In vVector) const;

	Vec3V_Out Normalize();


	// Friend Functions For Easier Access To Data Members:
	friend float DotProduct(Vec3V_In vVectorA, Vec3V_In vVectorB);

	friend float Mag(Vec3V_In vVector);
	friend float Length(Vec3V_In vVector);

	friend float MagSq(Vec3V_In vVector);
	friend float LengthSq(Vec3V_In vVector);
};

Vec3V CrossProduct(Vec3V_In vVectorA, Vec3V_In vVectorB);
Vec3V Normalize(Vec3V_In vVector);

extern const __declspec(selectany) Vec3V g_IdentityX3V = Vec3V(1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec3V g_IdentityY3V = Vec3V(0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec3V g_IdentityZ3V = Vec3V(0.0f, 0.0f, 1.0f);
extern const __declspec(selectany) Vec3V g_WorldUp3V = g_IdentityY3V;

__forceinline Vec3V::Vec3V(float fX, float fY, float fZ)
{
	row = VectorSet(fX, fY, fZ, VEC_FILL_VAL);
}

__forceinline Vec3V::Vec3V(float fX, float fY, float fZ, float fW)
{
	row = VectorSet(fX, fY, fZ, fW);
}

//__forceinline Vec3V::Vec3V(Vec3V_In vVector)
//{
//	row = vVector.row;
//}

#if !_WIN64
__forceinline Vec3V::Vec3V(Vec3V&& vVector)
{
	row = move(vVector.row);
}
#endif // !_WIN64

__forceinline Vec3V::Vec3V(Vector_In vVector)
{
	row = vVector;
}

#if !_WIN64
__forceinline Vec3V::Vec3V(Vector&& vVector)
{
	row = move(vVector);
}
#endif // !_WIN64

__forceinline Vec3V Vec3V::operator-() const
{
	return Vec3V(VectorNegate(row));
}

__forceinline Vec3V_Out Vec3V::operator=(Vec3V_In vVector)
{
	if(this != &vVector) { row = vVector.row; }
	return *this;
}

#if !_WIN64
__forceinline Vec3V_Out Vec3V::operator=(Vec3V&& vVector)
{
	if(this != &vVector)
		row = move(vVector.row);
	return *this;
}
#endif // !_WIN_64

__forceinline Vec3V_Out Vec3V::operator*=(float fScalar)
{
	row = VectorDivide(row, VectorSet(fScalar));
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator*=(Vec3V_In rhs)
{
	row = VectorMultiply(row, rhs.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator/(float fScalar) const
{
	return Vec3V(VectorDivide(row, VectorSet(fScalar)));
}

__forceinline Vec3V Vec3V::operator/(Vec3V_In rhs) const
{
	return Vec3V(VectorDivide(row, rhs.row));
}

__forceinline Vec3V_Out Vec3V::operator/=(float fScalar)
{
	row = VectorDivide(row, VectorSet(fScalar));
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator/=(Vec3V_In rhs)
{
	row = VectorDivide(row, rhs.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator*(float fScalar) const
{
	return Vec3V(VectorMultiply(row, VectorSet(fScalar)));
}

__forceinline Vec3V Vec3V::operator*(Vec3V_In rhs) const
{
	return Vec3V(VectorMultiply(row, rhs.row));
}

__forceinline Vec3V operator*(const float fScalar, Vec3V_In vVector)
{
	return Vec3V(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar);
}

__forceinline Vec3V_Out Vec3V::operator+=(Vec3V_In vVector)
{
	row = VectorAdd(row, vVector.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator+(Vec3V_In vVector) const
{
	return Vec3V(VectorAdd(row, vVector.row));
}

__forceinline Vec3V_Out Vec3V::operator-=(Vec3V_In vVector)
{
	row = VectorSubtract(row, vVector.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator-(Vec3V_In vVector) const
{
	return Vec3V(VectorSubtract(row, vVector.row));
}

__forceinline bool Vec3V::operator==(Vec3V_In vVector) const
{
	return IsEqualXYZ(row, vVector.row);
}

__forceinline bool Vec3V::operator!=(Vec3V_In vVector) const
{
	return IsNotEqualXYZ(row, vVector.row);
}

__forceinline Vec3V_Out Vec3V::Normalize()
{
	float mag = Mag(*this);

	// protection against divide by zero
	if(mag)	{ row = VectorDivide(row, VectorSet(mag)); }
	return *this;
}

__forceinline float DotProduct(Vec3V_In vVectorA, Vec3V_In vVectorB)
{
#if _WIN64
	vVectorA.w = 0.0f;
	Vec3V dp = vVectorA * vVectorB;
#else
	Vec3V tmp(vVectorA.x, vVectorA.y, vVectorA.z, 0.0f);
	Vec3V dp = tmp * vVectorB;
#endif

	Vector result = VectorHAdd(dp.row, dp.row);
	return VectorExtract<VecElem::X>(VectorHAdd(result, result));
}

__forceinline float Mag(Vec3V_In vVector)
{
	return sqrtf(MagSq(vVector));
}

__forceinline float Length(Vec3V_In vVector)
{
	return Mag(vVector);
}

__forceinline float MagSq(Vec3V_In vVector)
{
	return DotProduct(vVector, vVector);
}

__forceinline float LengthSq(Vec3V_In vVector)
{
	return MagSq(vVector);
}

#endif //VEC3V_INL
#endif //SSE_AVAILABLE