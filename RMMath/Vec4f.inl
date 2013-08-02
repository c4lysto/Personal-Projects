
#pragma region Vec4f Constructors
inline Vec4f::Vec4f()
#ifndef SSE_MATH_AVAILABLE
	: r(0), g(0), b(0), a(0)
#endif
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, g_ZeroVec4);
#endif
}

inline Vec4f::Vec4f(const Vec4f& vVector)
#ifndef SSE_MATH_AVAILABLE
	: r(vVector.r), g(vVector.g), b(vVector.b), a(vVector.a)
#endif
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#endif
}

inline Vec4f::Vec4f(Vec4f&& vVector)
#ifndef SSE_MATH_AVAILABLE
	: r(vVector.r), g(vVector.g), b(vVector.b), a(vVector.a)
#endif
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#endif
}

inline Vec4f::Vec4f(XMVECTOR&& vVector)
{
	XMStoreFloat4((XMFLOAT4*)this, vVector);
}

inline Vec4f::Vec4f(float fR, float fG, float fB, float fA) : r(fR), g(fG), b(fB), a(fA)
{

}

inline Vec4f::Vec4f(const Vec3f& vVector, float fA)
{
	position = vVector;
	a = fA;
}
#pragma endregion

#pragma region Vec4f Operators
inline Vec4f Vec4f::operator-()
{
	return Vec4f(-x, -y, -z, -w);
}

inline Vec4f& Vec4f::operator=(Vec4f&& vVector)
{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#else
		x = vVector.x;
		y = vVector.y;
		z = vVector.z;
		w = vVector.w;
#endif

	return *this;
}

inline Vec4f& Vec4f::operator=(XMVECTOR&& vVector)
{
	XMStoreFloat4((XMFLOAT4*)this, vVector);

	return *this;
}

inline Vec4f Vec4f::operator-(const Vec4f& vVector) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_sub_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(r - vVector.r, g - vVector.g, b - vVector.b, a - vVector.a);
#endif
}

inline Vec4f& Vec4f::operator-=(const Vec4f& vVector)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_sub_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	r -= vVector.r;
	g -= vVector.g;
	b -= vVector.b;
	a -= vVector.a;
#endif
	return *this;
}

inline Vec4f Vec4f::operator+(const Vec4f& vVector) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_add_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(r + vVector.r, g + vVector.g, b + vVector.b, a + vVector.a);
#endif
}

inline Vec4f& Vec4f::operator+=(const Vec4f& vVector)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_add_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	r += vVector.r;
	g += vVector.g;
	b += vVector.b;
	a += vVector.a;
#endif

	return *this;
}

inline Vec4f Vec4f::operator/(float fScalar) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_div_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
	return result;
#else
	fScalar = 1 / fScalar;
	return Vec4f(r * fScalar, g * fScalar, b * fScalar, a * fScalar);
#endif
}

inline Vec4f& Vec4f::operator/=(float fScalar)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_div_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
#else
	fScalar = 1 / fScalar;

	r *= fScalar;
	g *= fScalar;
	b *= fScalar;
	a *= fScalar;
#endif

	return *this;
}

inline Vec4f Vec4f::operator*(const Vec4f& vVector) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
	return result;
#else
	return Vec4f(x * vVector.x, y * vVector.y, z * vVector.z, w * vVector.w);
#endif
}

inline Vec4f& Vec4f::operator*=(const Vec4f& vVector)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_mul_ps(_mm_loadu_ps(color), _mm_loadu_ps(vVector.color)));
#else
	r *= vVector.r;
	g *= vVector.g;
	b *= vVector.b;
	a *= vVector.a;
#endif

	return *this;
}

inline Vec4f Vec4f::operator*(float fScalar) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
	return result;
#else
	return Vec4f(r * fScalar, g * fScalar, b * fScalar, a * fScalar);
#endif
}

inline Vec4f& Vec4f::operator*=(float fScalar)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, _mm_mul_ps(_mm_loadu_ps(color), _mm_set1_ps(fScalar)));
#else
	r *= fScalar;
	g *= fScalar;
	b *= fScalar;
	a *= fScalar;
#endif

	return *this;
}

inline Vec4f Vec4f::operator*(const Matrix4f& mMatrix) const
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(x);
	tmp2 = _mm_mul_ps(_mm_loadu_ps(mMatrix.m), tmp1);
	tmp1 = _mm_set1_ps(y);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 4), tmp1), tmp2);
	tmp1 = _mm_set1_ps(z);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 8), tmp1), tmp2);
	tmp1 = _mm_set1_ps(w);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 12), tmp1), tmp2);

	_mm_storeu_ps(result.color, tmp2);

	return result;
#else
	return Vec4f(Xx * mMatrix.Xx + Xy * mMatrix.Yx + Xz * mMatrix.Zx + Xw * mMatrix.Wx,
				 Xx * mMatrix.Xy + Xy * mMatrix.Yy + Xz * mMatrix.Zy + Xw * mMatrix.Wy,
				 Xx * mMatrix.Xz + Xy * mMatrix.Yz + Xz * mMatrix.Zz + Xw * mMatrix.Wz,
				 Xx * mMatrix.Xw + Xy * mMatrix.Yw + Xz * mMatrix.Zw + Xw * mMatrix.Ww);
#endif
}

inline Vec4f& Vec4f::operator*=(const Matrix4f& mMatrix)
{
	return (*this) = (*this) * mMatrix;
}

inline Vec4f operator*(float fScalar, const Vec4f& vVector)
{
#ifdef SSE_MATH_AVAILABLE
	Vec4f result;
	_mm_storeu_ps(result.color, _mm_mul_ps(_mm_loadu_ps(vVector.color), _mm_set1_ps(fScalar)));
	return result;
#else
	return Vec4f(vVector.r * fScalar, vVector.g * fScalar, vVector.b * fScalar, vVector.a * fScalar);
#endif
}
#pragma endregion

#pragma region Vec4f Math Funcs
inline float Vec4f::magnitude() const
{
#ifdef SSE_MATH_AVAILABLE
	__m128 vec = _mm_setr_ps(x, y, z, w);

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
	__m128 vec = _mm_setr_ps(x, y, z, w);

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
		*this = _mm_mul_ps(_mm_setr_ps(x, y, z, w), _mm_set1_ps(mag));
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
#ifdef SSE_MATH_AVAILABLE
	__m128 vec = _mm_setr_ps(x, y, z, w);
	__m128 other = _mm_load_ps(vVector.color);

	vec = _mm_mul_ps(vec, other);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
#else
	return x * vVector.x + y * vVector.y + z * vVector.z + w * vVector.w;
#endif
}

inline Vec4f& Vec4f::zero_out()
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(color, g_ZeroVec4);
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

inline float DotProduct(const Vec4f& vVectorA, const Vec4f& vVectorB)
{
	return vVectorA.dot_product(vVectorB);
}

inline Vec4f Lerp(const Vec4f& vVectorA, const Vec4f& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}
#pragma endregion