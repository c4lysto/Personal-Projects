#ifdef SSE_MATH_AVAILABLE

#pragma region Vec4fA Constructors
inline Vec4fA::Vec4fA()
{
	row = g_ZeroVec4;
}

inline Vec4fA::Vec4fA(const Vec4fA& vVector)
{
	row = vVector.row;
}

inline Vec4fA::Vec4fA(Vec4fA&& vVector)
{
	row = vVector.row;
}

inline Vec4fA::Vec4fA(__m128&& vVector)
{
	row = vVector;
}

inline Vec4fA::Vec4fA(float fR, float fG, float fB, float fA)
{
	row = _mm_setr_ps(fR, fG, fB, fA);
}

inline Vec4fA::Vec4fA(Vec3f vVector, float fA)
{
	row = _mm_setr_ps(vVector.x, vVector.y, vVector.z, fA);
}
#pragma endregion

#pragma region Vec4fA Operators
inline Vec4fA Vec4fA::operator-()
{
	return Vec4fA(-x, -y, -z, -w);
}

inline Vec4fA Vec4fA::operator=(const Vec4fA& vVector)
{
	if(this != &vVector)
		row = vVector.row;

	return *this;
}

inline Vec4fA& Vec4fA::operator=(Vec4fA&& vVector)
{
	row = vVector.row;

	return *this;
}

inline Vec4fA& Vec4fA::operator=(__m128&& vVector)
{
	row = vVector;

	return *this;
}

inline Vec4fA Vec4fA::operator-(const Vec4fA& vVector) const
{
	Vec4fA result;
	result.row = _mm_sub_ps(row, vVector.row);
	return result;
}

inline Vec4fA& Vec4fA::operator-=(const Vec4fA& vVector)
{
	row = _mm_sub_ps(row, vVector.row);

	return *this;
}

inline Vec4fA Vec4fA::operator+(const Vec4fA& vVector) const
{
	Vec4fA result;
	result.row = _mm_add_ps(row, vVector.row);
	return result;
}

inline Vec4fA& Vec4fA::operator+=(const Vec4fA& vVector)
{
	row = _mm_add_ps(row, vVector.row);
	return *this;
}

inline Vec4fA Vec4fA::operator/(float fScalar) const
{
	Vec4fA result;
	result.row = _mm_div_ps(row, _mm_set1_ps(fScalar));
	return result;
}

inline Vec4fA& Vec4fA::operator/=(float fScalar)
{
	row = _mm_div_ps(row, _mm_set1_ps(fScalar));
	return *this;
}

inline Vec4fA Vec4fA::operator*(const Vec4fA& vVector) const
{
	Vec4fA result;
	result.row = _mm_mul_ps(row, vVector.row);
	return result;
}

inline Vec4fA& Vec4fA::operator*=(const Vec4fA& vVector)
{
	row = _mm_mul_ps(row, vVector.row);
	return *this;
}

inline Vec4fA Vec4fA::operator*(float fScalar) const
{
	Vec4fA result;
	result.row = _mm_mul_ps(row, _mm_set1_ps(fScalar));
	return result;
}

inline Vec4fA& Vec4fA::operator*=(float fScalar)
{
	row = _mm_mul_ps(row, _mm_set1_ps(fScalar));
	return *this;
}

inline Vec4fA Vec4fA::operator*(const Matrix4fA& mMatrix) const
{
	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(x);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(y);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(z);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(w);
	return Vec4fA(_mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2));
}

inline Vec4fA& Vec4fA::operator*=(const Matrix4fA& mMatrix)
{
	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(x);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(y);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(z);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(w);

	return *this = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);
}

inline Vec4fA operator*(float fScalar, const Vec4fA& vVector)
{
	Vec4fA result;
	result.row = _mm_mul_ps(vVector.row, _mm_set1_ps(fScalar));
	return result;
}
#pragma endregion

#pragma region Vec4fA Math Funcs
inline float Vec4fA::magnitude() const
{
	__m128 vec = _mm_mul_ps(row, row);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return sqrt(_mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0]);
}

inline float Vec4fA::length() const
{
	return magnitude();
}

inline float Vec4fA::sq_magnitude() const
{
	__m128 vec = _mm_mul_ps(row, row);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
}

inline float Vec4fA::sq_length() const
{
	return sq_magnitude();
}

inline Vec4fA& Vec4fA::normalize()
{
	float mag = magnitude();

	if(mag)
		row = _mm_div_ps(row, _mm_set1_ps(mag));

	return *this;
}

inline float Vec4fA::dot_product(const Vec4fA& vVector) const
{
	__m128 vec = _mm_mul_ps(row, vVector.row);

	__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

	vec = _mm_add_ps(vec, tmp);

	return _mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0];
}

inline Vec4fA& Vec4fA::zero_out()
{
	row = g_ZeroVec4;
	return *this;
}

inline Vec4fA& Vec4fA::full_color()
{
	row = _mm_set1_ps(1.0f);
	return *this;
}
#pragma endregion

#pragma region Vec4fA Global Funcs
inline Vec4fA Normalize(const Vec4fA& vVector)
{
	Vec4fA tmp(vVector);
	return tmp.normalize();
}

inline float DotProduct(const Vec4fA& vVectorA, const Vec4fA& vVectorB)
{
	return vVectorA.dot_product(vVectorB);
}

inline Vec4fA Lerp(const Vec4fA& vVectorA, const Vec4fA& vVectorB, const float fLambda)
{
	return vVectorA + (vVectorB - vVectorA) * fLambda;
}
#pragma endregion

#endif // SSE_MATH_AVAILABLE