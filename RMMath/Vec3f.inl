
#pragma region Vec3f Constructors

inline Vec3f::Vec3f() : x(0), y(0), z(0)
{

}

inline Vec3f::Vec3f(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ)
{

}

inline Vec3f::Vec3f(float val) : x(val), y(val), z(val)
{

}

inline Vec3f::Vec3f(const Vec3f& vVector) : x(vVector.x), y(vVector.y), z(vVector.z)
{

}

inline Vec3f::Vec3f(Vec3f&& vVector) : x(vVector.x), y(vVector.y), z(vVector.z)
{

}

inline Vec3f::Vec3f(XMVECTOR&& vVector)
{
	XMStoreFloat3((XMFLOAT3*)this, vVector);
}

#pragma endregion

#pragma region Vec3f operators
inline Vec3f Vec3f::operator-()
{
	return Vec3f(-x, -y, -z);
}

inline Vec3f& Vec3f::operator=(const Vec3f& vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
		z = vVector.z;
	}

	return *this;
}

inline Vec3f& Vec3f::operator=(Vec3f&& vVector)
{
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;

	return *this;
}

inline Vec3f& Vec3f::operator=(XMVECTOR&& vVector)
{
	XMStoreFloat3((XMFLOAT3*)this, vVector);

	return *this;
}

inline Vec3f& Vec3f::operator=(const XMVECTOR& vVector)
{
	if(this != (Vec3f*)&vVector)
		XMStoreFloat3((XMFLOAT3*)this, vVector);

	return *this;
}

inline Vec3f& Vec3f::operator*=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

inline Vec3f& Vec3f::operator*=(const Vec3f& vScale)
{
	x *= vScale.x;
	y *= vScale.y;
	z *= vScale.z;

	return *this;
}

inline Vec3f Vec3f::operator*(const Matrix3f& mMatrix)
{
	return XMVector3Transform(XMLoadFloat3((XMFLOAT3*)this), XMLoadFloat3x3((XMFLOAT3X3*)&mMatrix));
}

inline Vec3f& Vec3f::operator*=(const Matrix3f& mMatrix)
{
	return *this = *this * mMatrix;
}

inline Vec3f Vec3f::operator/(float fScalar) const
{
#ifdef SSE_MATH_AVAILABLE
	__m128 result = _mm_div_ps(_mm_setr_ps(x, y, z ,0), _mm_set1_ps(fScalar));

	return Vec3f(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
#else
	fScalar = 1 / fScalar;

	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
#endif
}

inline Vec3f Vec3f::operator/(const Vec3f& vScale) const
{
	return Vec3f(x / vScale.x, y / vScale.y, z / vScale.z);
}

inline Vec3f& Vec3f::operator/=(float fScalar)
{
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

inline Vec3f& Vec3f::operator/=(const Vec3f& vScale)
{
	x /= vScale.x;
	y /= vScale.y;
	z /= vScale.z;

	return *this;
}

inline Vec3f Vec3f::operator*(float fScalar) const
{
	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
}

inline Vec3f Vec3f::operator*(const Vec3f& vScale) const
{
	return Vec3f(x * vScale.x, y * vScale.y, z * vScale.z);
}

inline Vec3f operator*(float fScalar, const Vec3f& vVector)
{
	return Vec3f(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar);
}

inline Vec3f& Vec3f::operator+=(const Vec3f& vVector)
{
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;

	return *this;
}

inline Vec3f Vec3f::operator+(const Vec3f& vVector) const
{
	return Vec3f(x + vVector.x, y + vVector.y, z + vVector.z);
}

inline Vec3f& Vec3f::operator-=(const Vec3f& vVector)
{
	x -= vVector.x;
	y -= vVector.y;
	z -= vVector.z;

	return *this;
}

inline Vec3f Vec3f::operator-(const Vec3f& vVector) const
{
	return Vec3f(x - vVector.x, y - vVector.y, z - vVector.z);
}

inline bool Vec3f::operator==(const Vec3f& vVector) const
{
	if( x != vVector.x )
		return false;

	if( y != vVector.y )
		return false;

	if( z != vVector.z )
		return false;

	return true;
}

inline bool Vec3f::operator!=(const Vec3f& vVector) const
{
	if(x != vVector.x)
		return true;

	if(y != vVector.y)
		return true;

	if(z != vVector.z)
		return true;

	return false;
}
#pragma endregion

#pragma region Vec3f Math Funcs
inline Vec3f& Vec3f::zero_out()
{
	x = y = z = 0;
	return *this;
}

inline Vec3f& Vec3f::normalize()
{
	float mag = magnitude();

	// protection against divide by zero
	if(mag)
	{
		mag = 1 / mag;

#ifdef SSE_MATH_AVAILABLE
		*this = _mm_mul_ps(_mm_setr_ps(x, y, z, 0), _mm_set1_ps(mag));
#else 
		x *= mag;
		y *= mag;
		z *= mag;			
#endif
	}

	return *this;
}

inline Vec3f& Vec3f::negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

inline float Vec3f::magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}

inline float Vec3f::length() const
{
	return magnitude();
}

inline float Vec3f::sq_magnitude() const 
{
	return x*x + y*y + z*z;
}

inline float Vec3f::sq_length() const
{
	return sq_magnitude();
}

inline float Vec3f::dot_product(const Vec3f& vVectorB) const
{
	return x * vVectorB.x + y * vVectorB.y + z * vVectorB.z;
}

inline Vec3f Vec3f::cross_product(const Vec3f& vVectorB) const 
{
	return Vec3f(y * vVectorB.z - z * vVectorB.y,
				 z * vVectorB.x - x * vVectorB.z,
				 x * vVectorB.y - y * vVectorB.x);
}
#pragma endregion

#pragma region Vec3f Global Funcs
inline Vec3f Normalize(const Vec3f& vVector)
{
	Vec3f tmp(vVector);
	return tmp.normalize();
}

inline float DotProduct(const Vec3f& vVectorA, const Vec3f& vVectorB)
{
	return vVectorA.x * vVectorB.x + vVectorA.y * vVectorB.y + vVectorA.z * vVectorB.z;
}

inline Vec3f CrossProduct(const Vec3f& vVectorA, const Vec3f& vVectorB)
{
	return Vec3f(vVectorA.y * vVectorB.z - vVectorA.z * vVectorB.y,
				 vVectorA.z * vVectorB.x - vVectorA.x * vVectorB.z,
				 vVectorA.x * vVectorB.y - vVectorA.y * vVectorB.x);
}

inline Vec3f Lerp(const Vec3f& vVectorA, const Vec3f& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}
#pragma endregion