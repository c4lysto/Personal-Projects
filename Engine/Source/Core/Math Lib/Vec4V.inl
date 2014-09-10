#if SSE_AVAILABLE
#ifndef VEC4V_INL
#define VEC4V_INL

typedef Vec4V& Vec4V_Ref;
typedef const Vec4V& Vec4V_ConstRef;

#if _WIN64
typedef Vec4V Vec4V_In;
#else
typedef Vec4V_ConstRef Vec4V_In;
#endif

// Other Vec4V Aliases
typedef Vec4V float4V;

ALIGN(16) class Vec4V
{
#define DEFINE_VEC4V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec4V(enumeration) { row = _mm_set1_ps(valueToInit); }

#define VEC4V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC4V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC4V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(const inType & rhs) { modifiedVal = rhs; }

#define VEC4V_VEC_OP(name, lhs, rhs) name<VS_VALID,VS_VALID,VS_VALID,VS_VALID>(lhs, rhs)


	friend class Mat44V;

private:
	union
	{
		__m128 row;

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
	Vec4V(Vec4V_ConstRef vVector);
	Vec4V(Vec4V&& vVector);
	explicit Vec4V(float fX, float fY, float fZ, float fW);
	explicit Vec4V(Vec3V_In vVector, float fA);
	explicit Vec4V(const __m128& vVector);
	Vec4V(__m128&& vVector);

#if defined(VEC4V_ACCESSOR) && defined(VEC4V_ACCESSOR_CONST)
	VEC4V_ACCESSOR_CONST(float, GetX, x)
	VEC4V_ACCESSOR_CONST(float, GetY, y)
	VEC4V_ACCESSOR_CONST(float, GetZ, z)
	VEC4V_ACCESSOR_CONST(float, GetW, w)
	__forceinline Vec3V GetXYZ() const {return Vec3V(row);}

	VEC4V_ACCESSOR(float&, GetXRef, x)
	VEC4V_ACCESSOR(float&, GetYRef, y)
	VEC4V_ACCESSOR(float&, GetZRef, z)
	VEC4V_ACCESSOR(float&, GetWRef, w)
#undef VEC4V_ACCESSOR
#undef VEC4V_ACCESSOR_CONST
#else
#error VEC4V ACCESSORS NOT DEFINED
#endif

#if defined(VEC4V_MUTATOR)
	VEC4V_MUTATOR(SetX, float, x)
	VEC4V_MUTATOR(SetY, float, y)
	VEC4V_MUTATOR(SetZ, float, z)
	VEC4V_MUTATOR(SetW, float, w)

	__forceinline void SetXYZ(Vec3V_ConstRef rhs) {row = _mm_setr_ps(rhs.x, rhs.y, rhs.z, w);}
	__forceinline void SetXYZ(Vec3V&& rhs) {row = _mm_setr_ps(rhs.x, rhs.y, rhs.z, w);}

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
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC4V_ENUM_CONSTRUCTOR
#endif //DEFINE_VEC4_ENUM_CONSTRUCTOR

	Vec4V operator-();

	Vec4V_Ref operator=(Vec4V_ConstRef vVector);
	Vec4V_Ref operator=(Vec4V&& vVector);

	Vec4V_Ref operator=(const __m128& vVector);
	Vec4V_Ref operator=(__m128&& vVector);

	Vec4V operator-(Vec4V_In vVector) const;
	Vec4V_Ref operator-=(Vec4V_In vVector);

	Vec4V operator+(Vec4V_In vVector) const;
	Vec4V_Ref operator+=(Vec4V_In vVector);

	Vec4V operator/(float fScalar) const;
	Vec4V_Ref operator/=(float fScalar);

	Vec4V operator*(float fScalar) const;
	Vec4V_Ref operator*=(float fScalar);
	friend Vec4V operator*(const float fScalar, Vec4V_In vVector);
	Vec4V operator*(Vec4V_In vVector) const;
	Vec4V_Ref operator*=(Vec4V_In vVector);

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;

	Vec4V_Ref Normalize();
};

extern const __declspec(selectany) Vec4V g_IdentityX4V = Vec4V(1.0f, 0.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityY4V = Vec4V(0.0f, 1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityZ4V = Vec4V(0.0f, 0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityW4V = Vec4V(0.0f, 0.0f, 0.0f, 1.0f);

__forceinline Vec4V::Vec4V(Vec4V_ConstRef vVector)
{
	row = vVector.row;
}

__forceinline Vec4V::Vec4V(Vec4V&& vVector)
{
	row = vVector.row;
}

__forceinline Vec4V::Vec4V(const __m128& vVector)
{
	row = vVector;
}

__forceinline Vec4V::Vec4V(__m128&& vVector)
{
	row = vVector;
}

__forceinline Vec4V::Vec4V(float fX, float fY, float fZ, float fW)
{
	row = _mm_setr_ps(fX, fY, fZ, fW);
}

__forceinline Vec4V::Vec4V(Vec3V_In vVector, float fW)
{
	row = vVector.row;
	w = fW;
}
__forceinline Vec4V Vec4V::operator-()
{
	return Vec4V(-x, -y, -z, -w);
}

__forceinline Vec4V_Ref Vec4V::operator=(Vec4V_ConstRef vVector)
{
	if(this != &vVector)
		row = vVector.row;
	return *this;
}

__forceinline Vec4V_Ref Vec4V::operator=(Vec4V&& vVector)
{
	row = vVector.row;
	return *this;
}

__forceinline Vec4V_Ref Vec4V::operator=(const __m128& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec4V_Ref Vec4V::operator=(__m128&& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec4V Vec4V::operator-(Vec4V_In vVector) const
{
	return Vec4V(VEC4V_VEC_OP(VectorSubtract, row, vVector.row));
}

__forceinline Vec4V_Ref Vec4V::operator-=(Vec4V_In vVector)
{
	row = VEC4V_VEC_OP(VectorSubtract, row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator+(Vec4V_In vVector) const
{
	return Vec4V(VEC4V_VEC_OP(VectorAdd, row, vVector.row));
}

__forceinline Vec4V_Ref Vec4V::operator+=(Vec4V_In vVector)
{
	row = VEC4V_VEC_OP(VectorAdd, row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator/(float fScalar) const
{
	return Vec4V(VEC4V_VEC_OP(VectorDivide, row, _mm_set1_ps(fScalar)));
}

__forceinline Vec4V_Ref Vec4V::operator/=(float fScalar)
{
	row = VEC4V_VEC_OP(VectorDivide, row, _mm_set1_ps(fScalar));
	return *this;
}

__forceinline Vec4V Vec4V::operator*(Vec4V_In vVector) const
{
	return Vec4V(VEC4V_VEC_OP(VectorMultiply, row, vVector.row));
}

__forceinline Vec4V_Ref Vec4V::operator*=(Vec4V_In vVector)
{
	row = VEC4V_VEC_OP(VectorMultiply, row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator*(float fScalar) const
{
	return Vec4V(VEC4V_VEC_OP(VectorMultiply, row, _mm_set1_ps(fScalar)));
}

__forceinline Vec4V_Ref Vec4V::operator*=(float fScalar)
{
	row = VEC4V_VEC_OP(VectorMultiply, row, _mm_set1_ps(fScalar));
	return *this;
}

__forceinline Vec4V operator*(float fScalar, Vec4V_In vVector)
{
	return Vec4V(VEC4V_VEC_OP(VectorMultiply, vVector.row, _mm_set1_ps(fScalar)));
}

__forceinline float Vec4V::Mag() const
{
	Vec4V tmp = Vec4V(VEC4V_VEC_OP(VectorMultiply, row, row));
	return sqrtf(tmp.x + tmp.y + tmp.z + tmp.w);
}

__forceinline float Vec4V::Length() const
{
	return Mag();
}

__forceinline float Vec4V::MagSq() const
{
	Vec4V tmp = Vec4V(VEC4V_VEC_OP(VectorMultiply, row, row));
	return tmp.x + tmp.y + tmp.z + tmp.w;
}

__forceinline float Vec4V::LengthSq() const
{
	return MagSq();
}

__forceinline Vec4V_Ref Vec4V::Normalize()
{
	float mag = Mag();

	if(mag)
	{
		*this = VEC4V_VEC_OP(VectorDivide, row, _mm_set1_ps(mag));
	}

	return *this;
}

#undef VEC4V_VEC_OP

#endif //VEC4V_INL
#endif //SSE_AVAILABLE