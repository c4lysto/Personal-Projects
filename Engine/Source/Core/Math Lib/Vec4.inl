#ifndef VEC4_INL
#define VEC4_INL

typedef Vec4f& Vec4f_Ref;
typedef const Vec4f& Vec4f_ConstRef;

typedef Vec4f_ConstRef Vec4f_In;

// Other Vec4f Aliases
typedef Vec4f float4;

class Vec4f
{
#if SSE_AVAILABLE
#define DEFINE_VEC4_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec4f(enumeration)\
	{\
	_mm_storeu_ps(vector, _mm_set1_ps(valueToInit)); \
}
#else
#define DEFINE_VEC4_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec4f(enumeration) : x(valueToInit), y(valueToInit), z(valueToInit), w(valueToInit){}
#endif //SSE_AVAILABLE

#define VEC4_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC4_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC4_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(const inType& rhs) { modifiedVal = rhs; }

#define VEC4_MUTATOR_MOVE(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType&& rhs) { modifiedVal = move(rhs); }


	friend class Mat44;

private:
	union
	{
		float color[4];
		float vector[4];

		union
		{
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
	Vec4f(float fX, float fY, float fZ, float fW);
	Vec4f(Vec3f_In vVector, float fA);

#if SSE_AVAILABLE
	explicit Vec4f(const __m128& vVector);
	explicit Vec4f(__m128&& vVector);
#endif

#if defined(VEC4_ACCESSOR) && defined(VEC4_ACCESSOR_CONST)
	VEC4_ACCESSOR_CONST(float, GetX, x)
	VEC4_ACCESSOR_CONST(float, GetY, y)
	VEC4_ACCESSOR_CONST(float, GetZ, z)
	VEC4_ACCESSOR_CONST(float, GetW, w)
	VEC4_ACCESSOR_CONST(Vec3f, GetXYZ, position)

	VEC4_ACCESSOR(float&, GetXRef, x)
	VEC4_ACCESSOR(float&, GetYRef, y)
	VEC4_ACCESSOR(float&, GetZRef, z)
	VEC4_ACCESSOR(float&, GetWRef, w)
#undef VEC4_ACCESSOR
#undef VEC4_ACCESSOR_CONST
#else
#error VEC4 ACCESSORS NOT DEFINED
#endif

#if defined(VEC4_MUTATOR) && defined(VEC4_MUTATOR_MOVE)
	VEC4_MUTATOR(SetX, float, x)
	VEC4_MUTATOR(SetY, float, y)
	VEC4_MUTATOR(SetZ, float, z)
	VEC4_MUTATOR(SetW, float, w)
	VEC4_MUTATOR(SetXYZ, Vec3f, position)
	VEC4_MUTATOR_MOVE(SetXYZ, Vec3f, position)
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
	DEFINE_VEC4_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC4_ENUM_CONSTRUCTOR
#endif //DEFINE_VEC4_ENUM_CONSTRUCTOR

	Vec3f GetXYZ() {return position;}

	Vec4f operator-();

	Vec4f_Ref operator=(Vec4f_In vVector);
	Vec4f_Ref operator=(Vec4f&& vVector);

#if SSE_AVAILABLE
	Vec4f_Ref operator=(const __m128& vVector);
	Vec4f_Ref operator=(__m128&& vVector);
#endif

	Vec4f operator-(Vec4f_In vVector) const;
	Vec4f_Ref operator-=(Vec4f_In vVector);

	Vec4f operator+(Vec4f_In vVector) const;
	Vec4f_Ref operator+=(Vec4f_In vVector);

	Vec4f operator/(float fScalar) const;
	Vec4f_Ref operator/=(float fScalar);

	Vec4f operator*(float fScalar) const;
	Vec4f_Ref operator*=(float fScalar);
	friend Vec4f operator*(const float fScalar, Vec4f_In vVector);
	Vec4f operator*(Vec4f_In vVector) const;
	Vec4f_Ref operator*=(Vec4f_In vVector);

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;

	Vec4f_Ref Normalize();
};


extern const __declspec(selectany) Vec4f g_IdentityX4 = Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityY4 = Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityZ4 = Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityW4 = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

__forceinline Vec4f::Vec4f(Vec4f_In vVector)
#if !SSE_AVAILABLE
: x(vVector.x), y(vVector.y), z(vVector.z), w(vVector.w)
#endif
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#endif
}

__forceinline Vec4f::Vec4f(Vec4f&& vVector)
#if !SSE_AVAILABLE
: x(vVector.x), y(vVector.y), z(vVector.z), w(vVector.w)
#endif
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#endif
}

#if SSE_AVAILABLE
__forceinline Vec4f::Vec4f(const __m128& vVector)
{
	_mm_storeu_ps(vector, vVector);
}

__forceinline Vec4f::Vec4f(__m128&& vVector)
{
	_mm_storeu_ps(vector, vVector);
}
#endif //SSE_AVAILABLE

__forceinline Vec4f::Vec4f(float fX, float fY, float fZ, float fW) : x(fX), y(fY), z(fZ), w(fW)
{

}

__forceinline Vec4f::Vec4f(Vec3f_In vVector, float fW)
{
	position = vVector;
	w = fW;
}
__forceinline Vec4f Vec4f::operator-()
{
	return Vec4f(-x, -y, -z, -w);
}

__forceinline Vec4f_Ref Vec4f::operator=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#else
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;
	w = vVector.w;
#endif

	return *this;
}

__forceinline Vec4f_Ref Vec4f::operator=(Vec4f&& vVector)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#else
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;
	w = vVector.w;
#endif
	return *this;
}

#if SSE_AVAILABLE
__forceinline Vec4f_Ref Vec4f::operator=(const __m128& vVector)
{
	_mm_storeu_ps(vector, vVector);
	return *this;
}

__forceinline Vec4f_Ref Vec4f::operator=(__m128&& vVector)
{
	_mm_storeu_ps(vector, vVector);
	return *this;
}
#endif //SSE_AVAILABLE

__forceinline Vec4f Vec4f::operator-(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_sub_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(x - vVector.x, y - vVector.y, z - vVector.z, w - vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator-=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_sub_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	x -= vVector.x;
	y -= vVector.y;
	z -= vVector.z;
	w -= vVector.w;
#endif
	return *this;
}

__forceinline Vec4f Vec4f::operator+(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_add_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(x + vVector.x, y + vVector.y, z + vVector.z, w + vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator+=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_add_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;
	w += vVector.w;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator/(float fScalar) const
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_div_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
	return result;
#else
	fScalar = 1 / fScalar;
	return Vec4f(x * fScalar, y * fScalar, z * fScalar, w * fScalar);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator/=(float fScalar)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_div_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
#else
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator*(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(x * vVector.x, y * vVector.y, z * vVector.z, w * vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator*=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_mul_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	x *= vVector.x;
	y *= vVector.y;
	z *= vVector.z;
	w *= vVector.w;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator*(float fScalar) const
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
	return result;
#else
	return Vec4f(x * fScalar, y * fScalar, z * fScalar, w * fScalar);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator*=(float fScalar)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(color, _mm_mul_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
#else
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
#endif

	return *this;
}

__forceinline Vec4f operator*(float fScalar, Vec4f_In vVector)
{
#if SSE_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(vVector.color), _mm_set1_ps(fScalar)));
	return result;
#else
	return Vec4f(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar, vVector.w * fScalar);
#endif
}

__forceinline float Vec4f::Mag() const
{
#if SSE_AVAILABLE
	__m128 vec = _mm_setr_ps(x, y, z, w);

	vec = _mm_mul_ps(vec, vec);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return sqrt(_mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0]);
#else
	return sqrt(x*x + y*y + z*z + w*w);
#endif
}

__forceinline float Vec4f::Length() const
{
	return Mag();
}

__forceinline float Vec4f::MagSq() const
{
#if SSE_AVAILABLE
	__m128 vec = _mm_setr_ps(x, y, z, w);

	vec = _mm_mul_ps(vec, vec);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
#else
	return x*x + y*y + z*z + w*w;
#endif
}

__forceinline float Vec4f::LengthSq() const
{
	return MagSq();
}

__forceinline Vec4f_Ref Vec4f::Normalize()
{
	float mag = Mag();

	if(mag)
	{
#if SSE_AVAILABLE
		*this = _mm_div_ps(_mm_setr_ps(x, y, z, w), _mm_set1_ps(mag));
#else
		mag = 1 / mag;

		x *= mag;
		y *= mag;
		z *= mag;
		w *= mag;
#endif
	}

	return *this;
}

#endif //VEC4_INL