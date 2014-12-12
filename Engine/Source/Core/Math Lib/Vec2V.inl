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

typedef Vec2V Vec2V_Out;

typedef Vec2V_Ref Vec2V_InOut;

// Other Vec2V Aliases
typedef Vec2V float2V;


ALIGN(16) class Vec2V
{
#define DEFINE_VEC2V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec2V(enumeration) { row = VectorSet(valueToInit); }

#define VEC2V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC2V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC2V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


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
	Vec2V(){}
	explicit Vec2V(float fX, float fY);
	explicit Vec2V(float fX, float fY, float fZ, float fW);
	//Vec2V(Vec2V_In vVector);
#if !_WIN64
	Vec2V(Vec2V&& vVector);
#endif // !_WIN64
	~Vec2V(){}

	explicit Vec2V(Vector_In rhs);
#if !_WIN64
	Vec2V(Vector&& rhs);
#endif // !_WIN64

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

	Vec2V operator-() const;

	Vec2V_Out operator=(Vec2V_In vVector);
#if !_WIN64
	Vec2V_Out operator=(Vec2V&& vVector);
#endif // !_WIN64

	Vec2V operator+(Vec2V_In vVector) const;
	Vec2V_Out operator+=(Vec2V_In vVector);

	Vec2V operator-(Vec2V_In vVector) const;
	Vec2V_Out operator-=(Vec2V_In vVector);

	Vec2V operator*(Vec2V_In vVector) const;
	Vec2V operator*(float fScalar) const;
	friend Vec2V operator*(const float fScalar, Vec2V_In vVector);

	Vec2V_Out operator*=(Vec2V_In vVector);
	Vec2V_Out operator*=(float fScalar);

	Vec2V operator/(Vec2V_In vVector) const;
	Vec2V operator/(float fScalar) const;

	Vec2V_Out operator/=(Vec2V_In vVector);
	Vec2V_Out operator/=(float fScalar);

	bool operator==(Vec2V_In vVector) const;
	bool operator!=(Vec2V_In vVector) const;

	// Friend Functions For Easier Access To Data Members:
	friend float DotProduct(Vec2V_In vVectorA, Vec2V_In vVectorB);

	friend float Mag(Vec2V_In vVector);
	friend float Length(Vec2V_In vVector);

	friend float MagSq(Vec2V_In vVector);
	friend float LengthSq(Vec2V_In vVector);

	Vec2V_Out Normalize();
};

__forceinline Vec2V::Vec2V(float fX, float fY)
{
	row = VectorSet(fX, fY, VEC_FILL_VAL, VEC_FILL_VAL);
}

__forceinline Vec2V::Vec2V(float fX, float fY, float fZ, float fW)
{
	row = VectorSet(fX, fY, fZ, fW);
}

//__forceinline Vec2V::Vec2V(Vec2V_In vVector)
//{
//	row = vVector.row;
//}

#if !_WIN64
__forceinline Vec2V::Vec2V(Vec2V&& vVector)
{
	row = move(vVector.row);
}
#endif // !_WIN64

__forceinline Vec2V::Vec2V(Vector_In rhs)
{
	row = rhs;
}

#if !_WIN64
__forceinline Vec2V::Vec2V(Vector&& rhs)
{
	row = move(rhs);
}
#endif // !_WIN64

__forceinline Vec2V Vec2V::operator-() const
{
	return Vec2V(VectorNegate(row));
}

__forceinline Vec2V_Out Vec2V::operator=(Vec2V_In vVector)
{
	if(this != &vVector) { row = vVector.row; }
	return *this;
}

#if !_WIN64
__forceinline Vec2V_Out Vec2V::operator=(Vec2V&& vVector)
{
	if(this != vVector)
		row = move(vVector.row);
	return *this;
}
#endif // !_WIN64

__forceinline Vec2V Vec2V::operator+(Vec2V_In vVector) const
{
	return Vec2V(VectorAdd(row, vVector.row));
}

__forceinline Vec2V_Out Vec2V::operator+=(Vec2V_In vVector)
{
	row = VectorAdd(row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator-(Vec2V_In vVector) const
{
	return Vec2V(VectorSubtract(row, vVector.row));
}

__forceinline Vec2V_Out Vec2V::operator-=(Vec2V_In vVector)
{
	row = VectorSubtract(row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator*(Vec2V_In vVector) const
{
	return Vec2V(VectorMultiply(row, vVector.row));
}

__forceinline Vec2V Vec2V::operator*(float fScalar) const
{
	return Vec2V(VectorMultiply(row, VectorSet(fScalar)));
}

__forceinline Vec2V operator*(const float fScalar, Vec2V_In vVector)
{
	return Vec2V(VectorMultiply(vVector.row, VectorSet(fScalar)));
}

__forceinline Vec2V_Out Vec2V::operator*=(float fScalar)
{
	row = VectorMultiply(row, VectorSet(fScalar));
	return *this;
}

__forceinline Vec2V_Out Vec2V::operator*=(Vec2V_In vVector)
{
	row = VectorMultiply(row, vVector.row);
	return *this;
}

__forceinline Vec2V Vec2V::operator/(Vec2V_In vVector) const
{
	return Vec2V(VectorDivide(row, vVector.row));
}

__forceinline Vec2V Vec2V::operator/(float fScalar) const
{
	return Vec2V(VectorDivide(row, VectorSet(fScalar)));
}

__forceinline Vec2V_Out Vec2V::operator/=(Vec2V_In vVector)
{
	row = VectorDivide(row, vVector.row);
	return *this;
}

__forceinline Vec2V_Out Vec2V::operator/=(float fScalar)
{
	row = VectorDivide(row, VectorSet(fScalar));
	return *this;
}

__forceinline bool Vec2V::operator==(Vec2V_In vVector) const
{
	return IsEqualXY(row, vVector.row);
}

__forceinline bool Vec2V::operator!=(Vec2V_In vVector) const
{
	return IsNotEqualXY(row, vVector.row);
}

// Friend Functions For Easier Access To Data Members:
__forceinline float DotProduct(Vec2V_In vVectorA, Vec2V_In vVectorB)
{
	Vector dp = vVectorA.row * vVectorB.row;
	return VectorExtract<VecElem::X>(VectorHAdd(dp, dp));
}

__forceinline float Mag(Vec2V_In vVector)
{
	return sqrtf(MagSq(vVector));
}

__forceinline float Length(Vec2V_In vVector)
{
	return Mag(vVector);
}

__forceinline float MagSq(Vec2V_In vVector)
{
	return DotProduct(vVector, vVector);
}

__forceinline float LengthSq(Vec2V_In vVector)
{
	return MagSq(vVector);
}

__forceinline Vec2V_Out Vec2V::Normalize()
{
	float mag = Mag(*this);

	// protection against divide by zero
	if(mag)	{ row = VectorDivide(row, VectorSet(mag)); }
	return *this;
}

#endif //VEC2V_INL
#endif //SSE_AVAILABLE