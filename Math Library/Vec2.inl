#ifndef VEC2_INL
#define VEC2_INL

template<typename Type>
struct Vec2
{
#define DEFINE_VEC2_ENUM_CONSTRUCTOR(enumeration, valueToInit)\
	explicit __forceinline Vec2(enumeration) : x(valueToInit), y(valueToInit){}

#define VEC2_ACCESSOR(retType, funcName, retVal) \
	__forceinline retType funcName() { return retVal; }

#define VEC2_ACCESSOR_CONST(retType, funcName, retVal) \
	__forceinline retType funcName() const { return retVal; }

#define VEC2_MUTATOR(funcName, inType, modifiedVal) \
	__forceinline void funcName(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		Type pos[2];

		struct
		{
			Type x, y;
		};
	};

public:

	Vec2(){}

#ifdef	DEFINE_VEC2_ENUM_CONSTRUCTOR
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eZeroInitializer, 0)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eOneInitializer, 1)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTwoInitializer, 2)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eThreeInitializer, 3)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFourInitializer, 4)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eFiveInitializer, 5)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eSixInitializer, 6)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eSevenInitializer, 7)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eEightInitializer, 8)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eNineInitializer, 9)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTenInitializer, 10)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_VEC2_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
#undef DEFINE_VEC2_ENUM_CONSTRUCTOR
#endif //DEFINE_VEC2_ENUM_CONSTRUCTOR

	Vec2(const Vec2<Type>& vVector);
	Vec2(Vec2<Type>&& vVector);
	Vec2(Type fX, Type fY);

#if defined(VEC2_ACCESSOR) && defined(VEC2_ACCESSOR_CONST)
	VEC2_ACCESSOR_CONST(Type, GetX, x)
	VEC2_ACCESSOR_CONST(Type, GetY, y)

	VEC2_ACCESSOR(Type, GetXRef, x)
	VEC2_ACCESSOR(Type, GetYRef, y)
#undef VEC2_ACCESSOR
#undef VEC2_ACCESSOR_CONST
#else
#error VEC2 ACCESSORS NOT DEFINED
#endif

#if defined(VEC2_MUTATOR)
	VEC2_MUTATOR(SetX, Type, x)
	VEC2_MUTATOR(SetY, Type, y)
#undef VEC2_MUTATOR
#else
#error VEC2 MUTATORS NOT DEFINED
#endif

	Vec2<Type> operator-();

	Vec2<Type>& operator=(const Vec2<Type>& vVector);
	Vec2<Type>& operator=(Vec2<Type>&& vVector);
	Vec2<Type>& operator=(const POINT vVector);

	Vec2<Type> operator+(const Vec2<Type>& vVector) const;
	Vec2<Type>& operator+=(const Vec2<Type> & vVector);

	Vec2<Type> operator-(const Vec2<Type>& vVector) const;
	Vec2<Type>& operator-=(const Vec2<Type>& vVector);

	Vec2<Type> operator*(float fScalar) const;
	friend Vec2<Type> operator*(const float fScalar, const Vec2<Type>& vVector);
	Vec2<Type>& operator*=(float fScalar);

	Vec2<Type> operator/(float fScalar) const;
	Vec2<Type>& operator/=(float fScalar);

	float Mag() const;
	float Length() const;

	float MagSq() const;
	float LengthSq() const;

	Vec2<Type>& Normalize();
};

typedef Vec2<float> Vec2f;
typedef Vec2f float2;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2ui;
typedef Vec2<short> Vec2s;
typedef Vec2<unsigned short> Vec2us;
typedef Vec2<double> Vec2d;

template<typename Type>
__forceinline Vec2<Type>::Vec2(const Vec2& vVector) : x(vVector.x), y(vVector.y)
{

}

template<typename Type>
__forceinline Vec2<Type>::Vec2(Vec2&& vVector) : x(vVector.x), y(vVector.y)
{

}

template<typename Type>
__forceinline Vec2<Type>::Vec2(Type fX, Type fY) : x(fX), y(fY)
{

}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator=(const Vec2<Type>& vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
	}

	return *this;
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator=(Vec2&& vVector)
{
	x = vVector.x;
	y = vVector.y;

	return *this;
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator=(const POINT vVector)
{
	x = (float)vVector.x;
	y = (float)vVector.y;

	return *this;
}

template<typename Type>
__forceinline Vec2<Type> Vec2<Type>::operator+(const Vec2& vVector) const
{
	return Vec2(x + vVector.x, y + vVector.y);
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator+=(const Vec2& vVector)
{
	x += vVector.x;
	y += vVector.y;

	return *this;
}

template<typename Type>
__forceinline Vec2<Type> Vec2<Type>::operator-(const Vec2& vVector) const
{
	return Vec2(x - vVector.x, y - vVector.y);
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator-=(const Vec2& vVector)
{
	x -= vVector.x;
	y -= vVector.y;

	return *this;
}

template<typename Type>
__forceinline Vec2<Type> Vec2<Type>::operator-()
{
	return Vec2(-x, -y);
}

template<typename Type>
__forceinline Vec2<Type> Vec2<Type>::operator*(float fScalar) const
{
	return Vec2(x * fScalar, y * fScalar);
}

template<typename Type>
__forceinline Vec2<Type> operator*(float fScalar, const Vec2<Type>& vVector)
{
	return Vec2(vVector.x * fScalar, vVector.y * fScalar);
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator*=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;

	return *this;
}

template<typename Type>
__forceinline Vec2<Type> Vec2<Type>::operator/(float fScalar) const
{
	fScalar = 1 / fScalar;
	return Vec2(x * fScalar, y * fScalar);
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::operator/=(float fScalar)
{
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;

	return *this;
}

template<typename Type>
__forceinline float Vec2<Type>::Mag() const
{
	return sqrt(x * x + y * y);
}

template<typename Type>
__forceinline float Vec2<Type>::Length() const
{
	return Mag();
}

template<typename Type>
__forceinline float Vec2<Type>::MagSq() const
{
	return x * x + y * y;
}

template<typename Type>
__forceinline float Vec2<Type>::LengthSq() const
{
	return MagSq();
}

template<typename Type>
__forceinline Vec2<Type>& Vec2<Type>::Normalize()
{
	float mag = Mag();

	if(mag)
	{
		mag = 1 / mag;

		x = x * mag;
		y = y * mag;
	}

	return *this;
}

#endif //VEC2_INL