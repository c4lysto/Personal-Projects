
#pragma region Vec2f Constructors
template<typename Type>
inline Vec2<Type>::Vec2() : x(0), y(0)
{

}

template<typename Type>
inline Vec2<Type>::Vec2(const Vec2& vVector) : x(vVector.x), y(vVector.y)
{

}

template<typename Type>
inline Vec2<Type>::Vec2(Vec2&& vVector) : x(vVector.x), y(vVector.y)
{

}

template<typename Type>
inline Vec2<Type>::Vec2(Type fX, Type fY) : x(fX), y(fY)
{

}
#pragma endregion

#pragma region Vec2 Operators
template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator=(const Vec2<Type>& vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
	}

	return *this;
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator=(Vec2&& vVector)
{
	x = vVector.x;
	y = vVector.y;

	return *this;
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator=(const POINT vVector)
{
	x = (float)vVector.x;
	y = (float)vVector.y;

	return *this;
}

template<typename Type>
inline Vec2<Type> Vec2<Type>::operator+(const Vec2& vVector) const
{
	return Vec2(x + vVector.x, y + vVector.y);
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator+=(const Vec2& vVector)
{
	x += vVector.x;
	y += vVector.y;

	return *this;
}

template<typename Type>
inline Vec2<Type> Vec2<Type>::operator-(const Vec2& vVector) const
{
	return Vec2(x - vVector.x, y - vVector.y);
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator-=(const Vec2& vVector)
{
	x -= vVector.x;
	y -= vVector.y;

	return *this;
}

template<typename Type>
inline Vec2<Type> Vec2<Type>::operator-()
{
	return Vec2(-x, -y);
}

template<typename Type>
inline Vec2<Type> Vec2<Type>::operator*(float fScalar) const
{
	return Vec2(x * fScalar, y * fScalar);
}

template<typename Type>
inline Vec2<Type> operator*(float fScalar, const Vec2<Type>& vVector)
{
	return Vec2(vVector.x * fScalar, vVector.y * fScalar);
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator*=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;

	return *this;
}

template<typename Type>
inline Vec2<Type> Vec2<Type>::operator/(float fScalar) const
{
	fScalar = 1 / fScalar;
	return Vec2(x * fScalar, y * fScalar);
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::operator/=(float fScalar)
{
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;

	return *this;
}
#pragma endregion

#pragma region Vec2 Math Funcs
template<typename Type>
inline float Vec2<Type>::dot_product(const Vec2& vVector) const
{
	return (x * vVector.x) + (y * vVector.y);
}

template<typename Type>
inline float Vec2<Type>::magnitude() const
{
	return sqrt(x * x + y * y);
}

template<typename Type>
inline float Vec2<Type>::length() const
{
	return magnitude();
}

template<typename Type>
inline float Vec2<Type>::sq_magnitude() const
{
	return x * x + y * y;
}

template<typename Type>
inline float Vec2<Type>::sq_length() const
{
	return sq_magnitude();
}

template<typename Type>
inline Vec2<Type>& Vec2<Type>::normalize()
{
	float mag = magnitude();

	if(mag)
	{
		mag = 1 / mag;

		x = x * mag;
		y = y * mag;
	}

	return *this;
}

template<typename Type>
inline float Vec2<Type>::angle_between(const Vec2& vVector) const
{
	float dotProduct = (x * vVector.x) + (y * vVector.y);

	float length = magnitude() * vVector.magnitude();

	if(!length)
		return 0.0f;

	float angle = acos(dotProduct / length);

	if(_isnan(angle))
		return 0.0f;

	return angle;
}
#pragma endregion

#pragma region Vec2 Global Funcs
template<typename Type>
inline Vec2<Type> Normalize(const Vec2<Type>& vVector)
{
	Vec2<Type> tmp(vVector);
	return tmp.normalize();
}

template<typename Type>
inline float DotProduct(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB)
{
	return vVectorA.dot_product(vVectorB);
}

template<typename Type>
inline Vec2<Type> Lerp(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}

template<typename Type>
inline float AngleBetween(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB)
{
	return vVectorA.angle_between(vVectorB);
}
#pragma endregion