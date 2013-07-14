
#pragma region Vec4f Constructors
inline Vec4f::Vec4f() : r(0), g(0), b(0), a(0)
{

}

inline Vec4f::Vec4f(const Vec4f&& vVector)
{
	*this = move(vVector);
}

inline Vec4f::Vec4f(const XMVECTOR&& vVector)
{
	*this = move(vVector);
}

inline Vec4f::Vec4f(float fR, float fG, float fB, float fA) : r(fR), g(fG), b(fB), a(fA)
{

}

inline Vec4f::Vec4f(Vec3f vVector, float fA)
{
	position = vVector;
	a = fA;
}
#pragma endregion

#pragma region Vec4f Operators
inline Vec4f& Vec4f::operator=(const Vec4f&& vVector)
{
	if(this != &vVector)
	{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#else
		x = vVector.x;
		y = vVector.y;
		z = vVector.z;
		w = vVector.w;
#endif
	}

	return *this;
}

inline Vec4f& Vec4f::operator=(const XMVECTOR&& vVector)
{
	if(this != (Vec4f*)&vVector)
		XMStoreFloat4((XMFLOAT4*)this, vVector);

	return *this;
}

inline Vec4f Vec4f::operator-(const Vec4f& vVector) const
{
	return XMVectorSubtract(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
}

inline Vec4f& Vec4f::operator-=(const Vec4f& vVector)
{
	return *this = XMVectorSubtract(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
}

inline Vec4f Vec4f::operator+(const Vec4f& vVector) const
{
	return XMVectorAdd(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
}

inline Vec4f& Vec4f::operator+=(const Vec4f& vVector)
{
	return *this = XMVectorAdd(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
}

inline Vec4f Vec4f::operator/(const float fScalar) const
{
	return XMVectorDivide(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
}

inline Vec4f& Vec4f::operator/=(const float fScalar)
{
	return *this = XMVectorDivide(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
}

inline Vec4f Vec4f::operator*(const float fScalar) const
{
	return XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
}

inline Vec4f& Vec4f::operator*=(const float fScalar)
{
	return *this = XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
}

inline Vec4f Vec4f::operator*(const Matrix4f& mMatrix) const
{
	return XMVector4Transform(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4x4((XMFLOAT4X4*)&mMatrix));
}

inline Vec4f& Vec4f::operator*=(const Matrix4f& mMatrix)
{
	return (*this) = (*this) * mMatrix;
}

inline Vec4f operator*(const float fScalar, const Vec4f& vVector)
{
	return XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)&vVector), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
}

inline Vec4f operator-(const Vec4f& vVector)
{
	return vVector * -1;
}
#pragma endregion

#pragma region Vec4f Math Funcs
inline float Vec4f::magnitude() const
{
#ifdef SSE_MATH_AVAILABLE
	__m128 vec = _mm_set_ps(x, y, z, w);

	vec = _mm_mul_ps(vec, vec);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return sqrt(_mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0]);
#else
	return sqrt(x*x + y*y + z*z + w*w);
#endif
}

inline float Vec4f::length() const
{
	return magnitude();
}

inline float Vec4f::sq_magnitude() const
{
#ifdef SSE_MATH_AVAILABLE
	__m128 vec = _mm_set_ps(x, y, z, w);

	vec = _mm_mul_ps(vec, vec);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
#else
	return x*x + y*y + z*z + w*w;
#endif
}

inline float Vec4f::sq_length() const
{
	return sq_magnitude();
}

inline Vec4f& Vec4f::normalize()
{
	float mag = magnitude();

	if(mag)
	{
		mag = 1 / mag;

#ifdef SSE_MATH_AVAILABLE
		*this = _mm_mul_ps(_mm_set_ps(w, z, y, x), _mm_set1_ps(mag));
#else
		x *= mag;
		y *= mag;
		z *= mag;
		w *= mag;
#endif
	}

	return *this;
}

inline float Vec4f::dot_product(const Vec4f& vVector) const
{
	__m128 vec = _mm_set_ps(x, y, z, w);
	__m128 other = _mm_load_ps(vVector.color);

	vec = _mm_mul_ps(vec, other);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
}

inline Vec4f& Vec4f::zero_out()
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_setzero_ps());
#else
	x = y = z = w = 0;
#endif
	return *this;
}

inline Vec4f& Vec4f::full_color()
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_set1_ps(1.0f));
#else
	x = y = z = w = 1;
#endif
	return *this;
}
#pragma endregion

#pragma region Vec4f Global Funcs
inline Vec4f Normalize(const Vec4f& vVector)
{
	Vec4f tmp(vVector);
	return tmp.normalize();
}

inline float Dot_Product(const Vec4f& vVectorA, const Vec4f& vVectorB)
{
	return vVectorA.dot_product(vVectorB);
}

inline Vec4f Lerp(const Vec4f& vVectorA, const Vec4f& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}
#pragma endregion