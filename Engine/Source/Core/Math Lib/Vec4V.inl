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

typedef Vec4V Vec4V_Out;

typedef Vec4V_Ref Vec4V_InOut;

// Other Vec4V Aliases
typedef Vec4V float4V;

ALIGN(16) class Vec4V
{
#define DEFINE_VEC4V_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec4V(enumeration) { row = VectorSet(valueToInit); }

#define VEC4V_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC4V_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC4V_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(const inType & rhs) { modifiedVal = rhs; }


	friend class Mat44V;

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
	Vec4V(){}
	//Vec4V(Vec4V_In vVector);
#if !_WIN64
	Vec4V(Vec4V&& vVector);
#endif // !_WIN64
	explicit Vec4V(float fX, float fY, float fZ, float fW);
	explicit Vec4V(Vec3V_In vVector, float fA);
	explicit Vec4V(Vector_In vVector);
#if !_WIN64
	Vec4V(Vector&& vVector);
#endif // !_WIN64

#if defined(VEC4V_ACCESSOR) && defined(VEC4V_ACCESSOR_CONST)
	VEC4V_ACCESSOR_CONST(float, GetX, x)
	VEC4V_ACCESSOR_CONST(float, GetY, y)
	VEC4V_ACCESSOR_CONST(float, GetZ, z)
	VEC4V_ACCESSOR_CONST(float, GetW, w)
	__forceinline Vec3V GetXYZ() const {return Vec3V(row);}

	// Returns the Vector Intrinsic Data Type
	__forceinline Vector_Out GetVector() const {return row;}

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

	__forceinline void SetXYZ(Vec3V_In rhs) {row = VectorSet(rhs.x, rhs.y, rhs.z, w);}
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
	DEFINE_VEC4V_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC4V_ENUM_CONSTRUCTOR
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

	Vec4V operator/(float fScalar) const;
	Vec4V_Out operator/=(float fScalar);

	Vec4V operator*(float fScalar) const;
	Vec4V_Out operator*=(float fScalar);
	friend Vec4V_Out operator*(const float fScalar, Vec4V_In vVector);
	Vec4V operator*(Vec4V_In vVector) const;
	Vec4V_Out operator*=(Vec4V_In vVector);

	bool operator==(Vec4V_In vVector) const;
	bool operator!=(Vec4V_In vVector) const;

	
	// Friend Functions For Easier Access To Data Members:
	friend float DotProduct(Vec4V_In vVectorA, Vec4V_In vVectorB);

	friend float Mag(Vec4V_In vVector);
	friend float Length(Vec4V_In vVector);

	friend float MagSq(Vec4V_In vVector);
	friend float LengthSq(Vec4V_In vVector);

	Vec4V_Out Normalize();
};

extern const __declspec(selectany) Vec4V g_IdentityX4V = Vec4V(1.0f, 0.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityY4V = Vec4V(0.0f, 1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityZ4V = Vec4V(0.0f, 0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec4V g_IdentityW4V = Vec4V(0.0f, 0.0f, 0.0f, 1.0f);

//__forceinline Vec4V::Vec4V(Vec4V_In vVector)
//{
//	row = vVector.row;
//}

#if !_WIN64
__forceinline Vec4V::Vec4V(Vec4V&& vVector)
{
	row = vVector.row;
}
#endif // !_WIN64

//__forceinline Vec4V::Vec4V(Vector_In vVector)
//{
//	row = vVector;
//}

#if !_WIN64
__forceinline Vec4V::Vec4V(Vector&& vVector)
{
	row = move(vVector);
}
#endif // !_WIN64

__forceinline Vec4V::Vec4V(float fX, float fY, float fZ, float fW)
{
	row = VectorSet(fX, fY, fZ, fW);
}

__forceinline Vec4V::Vec4V(Vec3V_In vVector, float fW)
{
	row = vVector.row;
	w = fW;
}
__forceinline Vec4V Vec4V::operator-() const
{
	return Vec4V(VectorNegate(row));
}

__forceinline Vec4V_Out Vec4V::operator=(Vec4V_In vVector)
{
	if(this != &vVector) { row = vVector.row; }
	return *this;
}

#if !_WIN64
__forceinline Vec4V_Out Vec4V::operator=(Vec4V&& vVector)
{
	if(this != &vVector)
		row = move(vVector.row);
	return *this;
}
#endif // !_WIN64

__forceinline Vec4V Vec4V::operator-(Vec4V_In vVector) const
{
	return Vec4V(VectorSubtract(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator-=(Vec4V_In vVector)
{
	row = VectorSubtract(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator+(Vec4V_In vVector) const
{
	return Vec4V(VectorAdd(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator+=(Vec4V_In vVector)
{
	row = VectorAdd(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator/(float fScalar) const
{
	return Vec4V(VectorDivide(row, VectorSet(fScalar)));
}

__forceinline Vec4V_Out Vec4V::operator/=(float fScalar)
{
	row = VectorDivide(row, VectorSet(fScalar));
	return *this;
}

__forceinline Vec4V Vec4V::operator*(Vec4V_In vVector) const
{
	return Vec4V(VectorMultiply(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator*=(Vec4V_In vVector)
{
	row = VectorMultiply(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator*(float fScalar) const
{
	return Vec4V(VectorMultiply(row, VectorSet(fScalar)));
}

__forceinline Vec4V_Out Vec4V::operator*=(float fScalar)
{
	row = VectorMultiply(row, VectorSet(fScalar));
	return *this;
}

__forceinline Vec4V operator*(float fScalar, Vec4V_In vVector)
{
	return Vec4V(VectorMultiply(vVector.row, VectorSet(fScalar)));
}

__forceinline bool Vec4V::operator==(Vec4V_In vVector) const
{
	return IsEqualXYZW(row, vVector.row);
}

__forceinline bool Vec4V::operator!=(Vec4V_In vVector) const
{
	return IsNotEqualXYZW(row, vVector.row);
}

__forceinline Vec4V_Out Vec4V::Normalize()
{
	float mag = Mag(*this);
	if(mag) { row = VectorDivide(row, VectorSet(mag)); }
	return *this;
}

// Friend Functions For Easier Access To Data Members:
__forceinline float DotProduct(Vec4V_In vVectorA, Vec4V_In vVectorB)
{
	Vector dp = vVectorA.row * vVectorB.row;
	dp = VectorHAdd(dp, dp);
	return VectorExtract<VecElem::X>(VectorHAdd(dp, dp));
}

__forceinline float Mag(Vec4V_In vVector)
{
	Vector magSq = vVector.row * vVector.row;
	magSq = VectorHAdd(magSq, magSq);
	magSq = VectorHAdd(magSq, magSq);
	return VectorExtract<VecElem::X>(Sqrt(magSq));
}

__forceinline float Length(Vec4V_In vVector)
{
	return Mag(vVector);
}

__forceinline float MagSq(Vec4V_In vVector)
{
	return DotProduct(vVector, vVector);	
}

__forceinline float LengthSq(Vec4V_In vVector)
{
	return MagSq(vVector);
}

#endif //VEC4V_INL
#endif //SSE_AVAILABLE