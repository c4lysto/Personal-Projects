#if SSE_AVAILABLE
#ifndef VEC3V_INL
#define VEC3V_INL

typedef Vec3V& Vec3V_Ref;
typedef const Vec3V& Vec3V_ConstRef;
typedef Vec3V_ConstRef Vec3V_In;

// Other Vec3V Aliases
typedef Vec3V float3V;

class Vec3V
{
#define DEFINE_VEC3V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec3V(enumeration) { row = _mm_setr_ps(valueToInit, valueToInit, valueToInit, VEC_FILL_VAL); }

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
		__m128 row;

		union
		{
			struct
			{
				float x, y, z;
			};
		};

		float fillW;
	};

public:
	Vec3V(){}
	Vec3V(float fX, float fY, float fZ);
	Vec3V(Vec3V_In vVector);
	Vec3V(Vec3V&& vVector);
	explicit Vec3V(const __m128& vVector);
	explicit Vec3V(__m128&& vVector);

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

	VEC3V_ACCESSOR(float&, GetXRef, x)
	VEC3V_ACCESSOR(float&, GetYRef, y)
	VEC3V_ACCESSOR(float&, GetZRef, z)
#undef VEC3V_ACCESSOR
#undef VEC3V_ACCESSOR_CONST
#else
#error VEC3V ACCESSORS NOT DEFINED
#endif

#if defined(VEC3V_MUTATOR)
		VEC3V_MUTATOR(SetX, float, x)
		VEC3V_MUTATOR(SetY, float, y)
		VEC3V_MUTATOR(SetZ, float, z)
#undef VEC3V_MUTATOR
#else
#error VEC3V MUTATORS NOT DEFINED
#endif

	Vec3V operator-();

	Vec3V_Ref operator=(Vec3V_In vVector);
	Vec3V_Ref operator=(Vec3V&& vVector);

	Vec3V_Ref operator=(const __m128& vVector);
	Vec3V_Ref operator=(__m128&& vVector);

	Vec3V_Ref operator*=(float fScalar);
	Vec3V_Ref operator*=(Vec3V_In vScale);

	Vec3V operator/(float fScalar) const;
	Vec3V operator/(Vec3V_In vScale) const;

	Vec3V_Ref operator/=(float fScalar);
	Vec3V_Ref operator/=(Vec3V_In vScale);

	Vec3V operator*(float fScalar) const;
	friend Vec3V operator*(const float fScalar, Vec3V_In vVector);
	Vec3V operator*(Vec3V_In vScale) const;

	Vec3V_Ref operator+=(Vec3V_In vVector);
	Vec3V operator+(Vec3V_In vVector) const;

	Vec3V_Ref operator-=(Vec3V_In vVector);
	Vec3V operator-(Vec3V_In vVector) const;

	bool operator==(Vec3V_In vVector) const;

	bool operator!=(Vec3V_In vVector) const;

	Vec3V_Ref Normalize();

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;
};

Vec3V CrossProduct(Vec3V_In vVectorA, Vec3V_In vVectorB);
Vec3V Normalize(Vec3V_In vVector);
float DotProduct(Vec3V_In vVectorA, Vec3V_In vVectorB);

extern const __declspec(selectany) Vec3V g_IdentityX3V = Vec3V(1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec3V g_IdentityY3V = Vec3V(0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec3V g_IdentityZ3V = Vec3V(0.0f, 0.0f, 1.0f);
extern const __declspec(selectany) Vec3V g_WorldUp3V = g_IdentityY3V;

__forceinline Vec3V::Vec3V(float fX, float fY, float fZ)
{
	row = _mm_setr_ps(fX, fY, fZ, VEC_FILL_VAL);
}

__forceinline Vec3V::Vec3V(Vec3V_In vVector)
{
	row = vVector.row;
}

__forceinline Vec3V::Vec3V(Vec3V&& vVector)
{
	row = vVector.row;
}

__forceinline Vec3V::Vec3V(const __m128& vVector)
{
	row = vVector;
}

__forceinline Vec3V::Vec3V(__m128&& vVector)
{
	row = vVector;
}

__forceinline Vec3V Vec3V::operator-()
{
	return Vec3V(-x, -y, -z);
}

__forceinline Vec3V_Ref Vec3V::operator=(Vec3V_In vVector)
{
	if(this != &vVector)
	{
		row = vVector.row;
	}

	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator=(Vec3V&& vVector)
{
	row = vVector.row;
	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator=(const __m128& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator=(__m128&& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator*=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator*=(Vec3V_In vScale)
{
	x *= vScale.x;
	y *= vScale.y;
	z *= vScale.z;

	return *this;
}

__forceinline Vec3V Vec3V::operator/(float fScalar) const
{
	Assert(fScalar, "Divide by Zero, undefined results!");
	return Vec3V(_mm_div_ps(row, _mm_set1_ps(fScalar)));
}

__forceinline Vec3V Vec3V::operator/(Vec3V_In vScale) const
{
	Vec3V retVal = Vec3V(_mm_div_ps(row, vScale.row));
	return retVal;
}

__forceinline Vec3V_Ref Vec3V::operator/=(float fScalar)
{
	row = _mm_div_ps(row, _mm_set1_ps(fScalar));
	return *this;
}

__forceinline Vec3V_Ref Vec3V::operator/=(Vec3V_In vScale)
{
	row = _mm_div_ps(row, vScale.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator*(float fScalar) const
{
	return Vec3V(x * fScalar, y * fScalar, z * fScalar);
}

__forceinline Vec3V Vec3V::operator*(Vec3V_In vScale) const
{
	return Vec3V(_mm_mul_ps(row, vScale.row));
}

__forceinline Vec3V operator*(const float fScalar, Vec3V_In vVector)
{
	return Vec3V(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar);
}

__forceinline Vec3V_Ref Vec3V::operator+=(Vec3V_In vVector)
{
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;

	return *this;
}

__forceinline Vec3V Vec3V::operator+(Vec3V_In vVector) const
{
	return Vec3V(_mm_add_ps(row, vVector.row));
}

__forceinline Vec3V_Ref Vec3V::operator-=(Vec3V_In vVector)
{
	row = _mm_sub_ps(row, vVector.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator-(Vec3V_In vVector) const
{
	return Vec3V(_mm_sub_ps(row, vVector.row));
}

__forceinline bool Vec3V::operator==(Vec3V_In vVector) const
{
	if(x != vVector.x)
		return false;

	if(y != vVector.y)
		return false;

	if(z != vVector.z)
		return false;

	return true;
}

__forceinline bool Vec3V::operator!=(Vec3V_In vVector) const
{
	if(x != vVector.x)
		return true;

	if(y != vVector.y)
		return true;

	if(z != vVector.z)
		return true;

	return false;
}

__forceinline Vec3V_Ref Vec3V::Normalize()
{
	float mag = Mag();

	// protection against divide by zero
	if(mag)
	{
		*this = _mm_div_ps(_mm_setr_ps(x, y, z, 0), _mm_set1_ps(mag));
	}

	return *this;
}

__forceinline float Vec3V::Mag() const
{
	return sqrt(x*x + y*y + z*z);
}

__forceinline float Vec3V::Length() const
{
	return Mag();
}

__forceinline float Vec3V::MagSq() const
{
	return x*x + y*y + z*z;
}

__forceinline float Vec3V::LengthSq() const
{
	return MagSq();
}
#endif //VEC3V_INL
#endif //SSE_AVAILABLE