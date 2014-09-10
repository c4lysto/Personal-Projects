#if SSE_AVAILABLE
#ifndef VEC2V_INL
#define VEC2V_INL

typedef Vec2V& Vec2V_Ref;
typedef const Vec2V& Vec2V_ConstRef;

#if _WIN64
typedef Vec2V Vec2V_In;
#else
typedef Vec2V_ConstRef Vec2V_In;
#endif

// Other Vec2V Aliases
typedef Vec2V float2V;


ALIGN(16) class Vec2V
{
#define DEFINE_VEC2V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec2V(enumeration) { row = _mm_set1_ps(valueToInit); }

#define VEC2V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC2V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC2V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }

#define VEC2V_VEC_OP(name, lhs, rhs) name<VS_VALID,VS_VALID,VS_INVALID,VS_INVALID>(lhs, rhs)


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
	Vec2V(){}
	explicit Vec2V(float fX, float fY);
	explicit Vec2V(float fX, float fY, float fZ, float fW);
	Vec2V(Vec2V_ConstRef vVector);
	Vec2V(Vec2V&& vVector);
	~Vec2V(){}

	explicit Vec2V(const __m128& rhs);
	Vec2V(__m128&& rhs);

#if defined(DEFINE_VEC2V_ENUM_CONSTRUCTOR)
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
	DEFINE_VEC2V_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC2V_ENUM_CONSTRUCTOR
#else
#error VEC2V ENUM CONSTRUCTORS NOT DEFINED
#endif

#if defined(VEC2V_ACCESSOR) && defined(VEC2V_ACCESSOR_CONST)
	VEC2V_ACCESSOR_CONST(float, GetX, x)
	VEC2V_ACCESSOR_CONST(float, GetY, y)
	VEC2V_ACCESSOR_CONST(float, GetZ, z)
	VEC2V_ACCESSOR_CONST(float, GetW, w)

	VEC2V_ACCESSOR(float&, GetXRef, x)
	VEC2V_ACCESSOR(float&, GetYRef, y)
	VEC2V_ACCESSOR(float&, GetZRef, z)
	VEC2V_ACCESSOR(float&, GetWRef, w)
#undef VEC2V_ACCESSOR
#undef VEC2V_ACCESSOR_CONST
#else
#error VEC2V ACCESSORS NOT DEFINED
#endif

#if defined(VEC2V_MUTATOR)
	VEC2V_MUTATOR(SetX, float, x)
	VEC2V_MUTATOR(SetY, float, y)
	VEC2V_MUTATOR(SetZ, float, z)
	VEC2V_MUTATOR(SetW, float, w)

#undef VEC2V_MUTATOR
#else
#error VEC2V MUTATORS NOT DEFINED
#endif

	Vec2V operator-();

	Vec2V_Ref operator=(Vec2V_ConstRef vVector);
	Vec2V_Ref operator=(Vec2V&& vVector);
	Vec2V_Ref operator=(const POINT vVector);
	Vec2V_Ref operator=(const __m128& vVector);
	Vec2V_Ref operator=(__m128&& vVector);

	Vec2V operator+(Vec2V_In vVector) const;
	Vec2V_Ref operator+=(const Vec2V & vVector);

	Vec2V operator-(Vec2V_In vVector) const;
	Vec2V_Ref operator-=(Vec2V_In vVector);

	Vec2V operator*(Vec2V_In vVector) const;
	Vec2V operator*(float fScalar) const;
	friend Vec2V operator*(const float fScalar, Vec2V_In vVector);

	Vec2V_Ref operator*=(Vec2V_In vVector);
	Vec2V_Ref operator*=(float fScalar);

	Vec2V operator/(Vec2V_In vVector) const;
	Vec2V operator/(float fScalar) const;

	Vec2V_Ref operator/=(Vec2V_In vVector);
	Vec2V_Ref operator/=(float fScalar);

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;

	Vec2V_Ref Normalize();
};

__forceinline Vec2V::Vec2V(float fX, float fY)
{
	row = _mm_setr_ps(fX, fY, VEC_FILL_VAL, VEC_FILL_VAL);
}

__forceinline Vec2V::Vec2V(float fX, float fY, float fZ, float fW)
{
	row = _mm_setr_ps(fX, fY, fZ, fW);
}

__forceinline Vec2V::Vec2V(Vec2V_ConstRef vVector)
{
	row = vVector.row;
}

__forceinline Vec2V::Vec2V(Vec2V&& vVector)
{
	row = vVector.row;
}

__forceinline Vec2V::Vec2V(const __m128& rhs)
{
	row = rhs;
}

__forceinline Vec2V::Vec2V(__m128&& rhs)
{
	row = rhs;
}

__forceinline Vec2V Vec2V::operator-()
{
	return Vec2V(-x, -y);
}

__forceinline Vec2V_Ref Vec2V::operator=(Vec2V_ConstRef vVector)
{
	if(this != &vVector)
		row = vVector.row;
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator=(Vec2V&& vVector)
{
	row = vVector.row;
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator=(const POINT vVector)
{
	x = (float)vVector.x;
	y = (float)vVector.y;
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator=(const __m128& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator=(__m128&& vVector)
{
	row = vVector;
	return *this;
}

__forceinline Vec2V Vec2V::operator+(Vec2V_In vVector) const
{
	return Vec2V(VEC2V_VEC_OP(VectorAdd, row, vVector.row));
}

__forceinline Vec2V_Ref Vec2V::operator+=(const Vec2V & vVector)
{
	row = VEC2V_VEC_OP(VectorAdd, row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator-(Vec2V_In vVector) const
{
	return Vec2V(VEC2V_VEC_OP(VectorSubtract, row, vVector.row));
}

__forceinline Vec2V_Ref Vec2V::operator-=(Vec2V_In vVector)
{
	row = VEC2V_VEC_OP(VectorSubtract, row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator*(Vec2V_In vVector) const
{
	return Vec2V(VEC2V_VEC_OP(VectorMultiply, row, vVector.row));
}

__forceinline Vec2V Vec2V::operator*(float fScalar) const
{
	return Vec2V(VEC2V_VEC_OP(VectorMultiply, row, _mm_set1_ps(fScalar)));
}

__forceinline Vec2V operator*(const float fScalar, Vec2V_In vVector)
{
	return Vec2V(VEC2V_VEC_OP(VectorMultiply, vVector.row, _mm_set1_ps(fScalar)));
}

__forceinline Vec2V_Ref Vec2V::operator*=(float fScalar)
{
	row = VEC2V_VEC_OP(VectorMultiply, row, _mm_set1_ps(fScalar));
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator*=(Vec2V_In vVector)
{
	row = VEC2V_VEC_OP(VectorMultiply, row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator/(Vec2V_In vVector) const
{
	return Vec2V(VEC2V_VEC_OP(VectorDivide, row, vVector.row));
}

__forceinline Vec2V Vec2V::operator/(float fScalar) const
{
	return Vec2V(VEC2V_VEC_OP(VectorDivide, row, _mm_set1_ps(fScalar)));
}

__forceinline Vec2V_Ref Vec2V::operator/=(Vec2V_In vVector)
{
	row = VEC2V_VEC_OP(VectorDivide, row, vVector.row);
	return *this;
}

__forceinline Vec2V_Ref Vec2V::operator/=(float fScalar)
{
	row = VEC2V_VEC_OP(VectorDivide, row, _mm_set1_ps(fScalar));
	return *this;
}

__forceinline float Vec2V::Mag() const
{
	return sqrtf(x*x + y*y);
}

__forceinline float Vec2V::Length() const
{
	return Mag();
}

__forceinline float Vec2V::MagSq() const
{
	return x*x + y*y;
}

__forceinline float Vec2V::LengthSq() const
{
	return MagSq();
}

__forceinline Vec2V_Ref Vec2V::Normalize()
{
	float mag = Mag();

	// protection against divide by zero
	if(mag)
	{
		row = VEC2V_VEC_OP(VectorDivide, row, _mm_set1_ps(mag));
	}

	return *this;
}

#undef VEC2V_VEC_OP

#endif //VEC2V_INL
#endif //SSE_AVAILABLE