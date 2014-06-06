#ifndef VEC3_INL
#define VEC3_INL

typedef Vec3f& Vec3f_Ref;
typedef const Vec3f& Vec3f_ConstRef;
typedef Vec3f_ConstRef Vec3f_In;

// Other Vec3f Aliases
typedef Vec3f float3;

class Vec3f
{
#define DEFINE_VEC3_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec3f(enumeration) : x(valueToInit), y(valueToInit), z(valueToInit){}

#define VEC3_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC3_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC3_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		float vector[3];

		struct
		{
			float x, y, z;
		};
	};

public:
	Vec3f(){}
	Vec3f(float fX, float fY, float fZ);
	Vec3f(Vec3f_In vVector);
	Vec3f(Vec3f&& vVector);

#if SSE_AVAILABLE
	explicit Vec3f(const __m128& vVector);
#endif

#ifdef DEFINE_VEC3_ENUM_CONSTRUCTOR
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC3_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC3_ENUM_CONSTRUCTOR
#endif //DEFINE_VEC3_ENUM_CONSTRUCTOR

#if defined(VEC3_ACCESSOR) && defined(VEC3_ACCESSOR_CONST)
	VEC3_ACCESSOR_CONST(float, GetX, x)
	VEC3_ACCESSOR_CONST(float, GetY, y)
	VEC3_ACCESSOR_CONST(float, GetZ, z)

	VEC3_ACCESSOR(float&, GetXRef, x)
	VEC3_ACCESSOR(float&, GetYRef, y)
	VEC3_ACCESSOR(float&, GetZRef, z)
#undef VEC3_ACCESSOR
#undef VEC3_ACCESSOR_CONST
#else
#error VEC3 ACCESSORS NOT DEFINED
#endif

#if defined(VEC3_MUTATOR)
	VEC3_MUTATOR(SetX, float, x)
	VEC3_MUTATOR(SetY, float, y)
	VEC3_MUTATOR(SetZ, float, z)
#undef VEC3_MUTATOR
#else
#error VEC3 MUTATORS NOT DEFINED
#endif

	Vec3f operator-();

	Vec3f_Ref operator=(Vec3f_In vVector);
	Vec3f_Ref operator=(Vec3f&& vVector);
#if SSE_AVAILABLE
	Vec3f_Ref operator=(const __m128& vVector);
	Vec3f_Ref operator=(const __m128&& vVector);
#endif

	Vec3f_Ref operator*=(float fScalar);
	Vec3f_Ref operator*=(Vec3f_In vScale);

	Vec3f operator/(float fScalar) const;
	Vec3f operator/(Vec3f_In vScale) const;

	Vec3f_Ref operator/=(float fScalar);
	Vec3f_Ref operator/=(Vec3f_In vScale);

	Vec3f operator*(float fScalar) const;
	friend Vec3f operator*(const float fScalar, Vec3f_In vVector);
	Vec3f operator*(Vec3f_In vScale) const;

	Vec3f_Ref operator+=(Vec3f_In vVector);
	Vec3f operator+(Vec3f_In vVector) const;

	Vec3f_Ref operator-=(Vec3f_In vVector);
	Vec3f operator-(Vec3f_In vVector) const;

	bool operator==(Vec3f_In vVector) const;

	bool operator!=(Vec3f_In vVector) const;

	Vec3f_Ref Normalize();

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;
};

Vec3f CrossProduct(Vec3f_In vVectorA, Vec3f_In vVectorB);
float DotProduct(Vec3f_In vVectorA, Vec3f_In vVectorB);

extern const __declspec(selectany) Vec3f g_IdentityX3 = Vec3f(1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec3f g_IdentityY3 = Vec3f(0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec3f g_IdentityZ3 = Vec3f(0.0f, 0.0f, 1.0f);

extern const __declspec(selectany) Vec3f g_WorldUp = 
#ifdef WORLD_UP
	WORLD_UP;
#else
	g_IdentityY3;
#define WORLD_UP g_WorldUp
#endif

__forceinline Vec3f::Vec3f(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ)
{

}

__forceinline Vec3f::Vec3f(Vec3f_In vVector) : x(vVector.x), y(vVector.y), z(vVector.z)
{

}

__forceinline Vec3f::Vec3f(Vec3f&& vVector) : x(vVector.x), y(vVector.y), z(vVector.z)
{

}

#if SSE_AVAILABLE
__forceinline Vec3f::Vec3f(const __m128& vVector)
{
	x = vVector.m128_f32[0];
	y = vVector.m128_f32[1];
	z = vVector.m128_f32[2];
}
#endif

__forceinline Vec3f Vec3f::operator-()
{
	return Vec3f(-x, -y, -z);
}

__forceinline Vec3f_Ref Vec3f::operator=(Vec3f_In vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
		z = vVector.z;
	}

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator=(Vec3f&& vVector)
{
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;

	return *this;
}

#if SSE_AVAILABLE
__forceinline Vec3f_Ref Vec3f::operator=(const __m128& vVector)
{
	x = vVector.m128_f32[0];
	y = vVector.m128_f32[1];
	z = vVector.m128_f32[2];
	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator=(const __m128&& vVector)
{
	x = vVector.m128_f32[0];
	y = vVector.m128_f32[1];
	z = vVector.m128_f32[2];
	return *this;
}
#endif

__forceinline Vec3f_Ref Vec3f::operator*=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator*=(Vec3f_In vScale)
{
	x *= vScale.x;
	y *= vScale.y;
	z *= vScale.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator/(float fScalar) const
{
#if SSE_AVAILABLE
	__m128 result = _mm_div_ps(_mm_setr_ps(x, y, z, 0), _mm_set1_ps(fScalar));

	return Vec3f(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
#else
	fScalar = 1 / fScalar;

	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
#endif
}

__forceinline Vec3f Vec3f::operator/(Vec3f_In vScale) const
{
	return Vec3f(x / vScale.x, y / vScale.y, z / vScale.z);
}

__forceinline Vec3f_Ref Vec3f::operator/=(float fScalar)
{
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator/=(Vec3f_In vScale)
{
	x /= vScale.x;
	y /= vScale.y;
	z /= vScale.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator*(float fScalar) const
{
	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
}

__forceinline Vec3f Vec3f::operator*(Vec3f_In vScale) const
{
	return Vec3f(x * vScale.x, y * vScale.y, z * vScale.z);
}

__forceinline Vec3f operator*(const float fScalar, Vec3f_In vVector)
{
	return Vec3f(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar);
}

__forceinline Vec3f_Ref Vec3f::operator+=(Vec3f_In vVector)
{
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator+(Vec3f_In vVector) const
{
	return Vec3f(x + vVector.x, y + vVector.y, z + vVector.z);
}

__forceinline Vec3f_Ref Vec3f::operator-=(Vec3f_In vVector)
{
	x -= vVector.x;
	y -= vVector.y;
	z -= vVector.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator-(Vec3f_In vVector) const
{
	return Vec3f(x - vVector.x, y - vVector.y, z - vVector.z);
}

__forceinline bool Vec3f::operator==(Vec3f_In vVector) const
{
	if(x != vVector.x)
		return false;

	if(y != vVector.y)
		return false;

	if(z != vVector.z)
		return false;

	return true;
}

__forceinline bool Vec3f::operator!=(Vec3f_In vVector) const
{
	if(x != vVector.x)
		return true;

	if(y != vVector.y)
		return true;

	if(z != vVector.z)
		return true;

	return false;
}

__forceinline Vec3f_Ref Vec3f::Normalize()
{
	float mag = Mag();

	// protection against divide by zero
	if(mag)
	{
#if SSE_AVAILABLE
		*this = Vec3f(_mm_div_ps(_mm_setr_ps(x, y, z, 0), _mm_set1_ps(mag)));
#else 
		mag = 1 / mag;

		x *= mag;
		y *= mag;
		z *= mag;
#endif
	}

	return *this;
}

__forceinline float Vec3f::Mag() const
{
	return sqrt(x*x + y*y + z*z);
}

__forceinline float Vec3f::Length() const
{
	return Mag();
}

__forceinline float Vec3f::MagSq() const
{
	return x*x + y*y + z*z;
}

__forceinline float Vec3f::LengthSq() const
{
	return MagSq();
}

#endif VEC3_INL