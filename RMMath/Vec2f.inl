
#pragma region Vec2f Constructors
inline Vec2f::Vec2f() : x(0), y(0)
{

}

inline Vec2f::Vec2f(const Vec2f&& vVector)
{
	*this = move(vVector);
}

inline Vec2f::Vec2f(const XMVECTOR&& vVector)
{
	*this = move(vVector);
}

inline Vec2f::Vec2f(float fX, float fY) : x(fX), y(fY)
{

}
#pragma endregion

#pragma region Vec2f Operators
inline Vec2f& Vec2f::operator=(const Vec2f&& vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
	}

	return *this;
}

inline Vec2f& Vec2f::operator=(const POINT vVector)
{
	x = (float)vVector.x;
	y = (float)vVector.y;

	return *this;
}

inline Vec2f& Vec2f::operator=(const XMVECTOR&& vVector)
{
	if(this != (Vec2f*)&vVector)
		XMStoreFloat2((XMFLOAT2*)this, vVector);

	return *this;
}

inline Vec2f& Vec2f::operator=(const XMVECTOR& vVector)
{
	if(this != (Vec2f*)&vVector)
		XMStoreFloat2((XMFLOAT2*)this, vVector);

	return *this;
}

inline Vec2f Vec2f::operator+(const Vec2f& vVector) const
{
	Vec2f vec(*this);

	vec.x += vVector.x;
	vec.y += vVector.y;

	return vec;
}

inline Vec2f& Vec2f::operator+=(const Vec2f& vVector)
{
	x += vVector.x;
	y += vVector.y;

	return *this;
}

inline Vec2f Vec2f::operator-(const Vec2f& vVector) const
{
	Vec2f vec(*this);

	vec.x -= vVector.x;
	vec.y -= vVector.y;

	return vec;
}

inline Vec2f& Vec2f::operator-=(const Vec2f& vVector)
{
	x -= vVector.x;
	y -= vVector.y;

	return *this;
}

inline Vec2f operator-(const Vec2f& vVector)
{
	return vVector * -1;
}

inline Vec2f Vec2f::operator*(const float fScalar) const
{
	Vec2f vec(*this);

	vec.x *= fScalar;
	vec.y *= fScalar;

	return vec;
}

inline Vec2f operator*(float fScalar, const Vec2f& vVector)
{
	return vVector * fScalar;
}

inline Vec2f& Vec2f::operator*=(const float fScalar)
{
	x *= fScalar;
	y *= fScalar;

	return *this;
}

inline Vec2f Vec2f::operator/(const float fScalar) const
{
	Vec2f vec(*this);

	vec.x /= fScalar;
	vec.y /= fScalar;

	return vec;
}

inline Vec2f& Vec2f::operator/=(const float fScalar)
{
	x /= fScalar;
	y /= fScalar;

	return *this;
}
#pragma endregion

#pragma region Vec2f Math Funcs
inline float Vec2f::dot_product(const Vec2f& vVector) const
{
	return (x * vVector.x) + (y * vVector.y);
}

inline float Vec2f::magnitude() const
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

inline float Vec2f::length() const
{
	return magnitude();
}

inline float Vec2f::sq_magnitude() const
{
	return pow(x, 2) + pow(y, 2);
}

inline float Vec2f::sq_length() const
{
	return sq_magnitude();
}

inline Vec2f& Vec2f::normalize()
{
	float mag = magnitude();

	if(mag)
	{
		mag = 1 / mag;

		x *= mag;
		y *= mag;
	}

	return *this;
}

inline float Vec2f::angle_between(const Vec2f& vVector) const
{
	return XMVector2AngleBetweenVectors(XMLoadFloat2((XMFLOAT2*)this), XMLoadFloat2((XMFLOAT2*)&vVector)).m128_f32[0];
}
#pragma endregion

#pragma region Vec2f Global Funcs
inline Vec2f Normalize(const Vec2f& vVector)
{
	Vec2f tmp(vVector);
	return tmp.normalize();
}

inline float Dot_Product(const Vec2f& vVectorA, const Vec2f& vVectorB)
{
	return vVectorA.dot_product(vVectorB);
}

inline Vec2f Lerp(const Vec2f& vVectorA, const Vec2f& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}

inline float AngleBetween(const Vec2f& vVectorA, const Vec2f& vVectorB)
{
	return vVectorA.angle_between(vVectorB);
}
#pragma endregion