
#pragma region Matrix4f Constructors
inline Matrix4f::Matrix4f()
{
	make_identity();
}

inline Matrix4f::Matrix4f(float fXx, float fXy, float fXz, float fXw,
						  float fYx, float fYy, float fYz, float fYw,
						  float fZx, float fZy, float fZz, float fZw,
						  float fWx, float fWy, float fWz, float fWw)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(fXx, fXy, fXz, fXw));
	_mm_storeu_ps(m + 4, _mm_setr_ps(fYx, fYy, fYz, fYw));
	_mm_storeu_ps(m + 8, _mm_setr_ps(fZx, fZy, fZz, fZw));
	_mm_storeu_ps(m + 12, _mm_setr_ps(fWx, fWy, fWz, fWw));
#else
	Xx = fXx;	Xy = fXy;	Xz = fXz;	Xw = fXw;
	Yx = fYx;	Yy = fYy;	Yz = fYz;	Yw = fYw;
	Zx = fZx;	Zy = fZy;	Zz = fZz;	Zw = fZw;
	Wx = fWx;	Wy = fWy;	Wz = fWz;	Ww = fWw;
#endif
}

inline Matrix4f::Matrix4f(const Matrix4f& mMatrix)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
	_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
	_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
	_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
	Xx = mMatrix.Xx;	Xy = mMatrix.Xy;	Xz = mMatrix.Xz;	Xw = mMatrix.Xw;
	Yx = mMatrix.Yx;	Yy = mMatrix.Yy;	Yz = mMatrix.Yz;	Yw = mMatrix.Yw;
	Zx = mMatrix.Zx;	Zy = mMatrix.Zy;	Zz = mMatrix.Zz;	Zw = mMatrix.Zw;
	Wx = mMatrix.Wx;	Wy = mMatrix.Wy;	Wz = mMatrix.Wz;	Ww = mMatrix.Ww;
#endif
}

inline Matrix4f::Matrix4f(Matrix4f&& mMatrix)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
	_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
	_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
	_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
	Xx = mMatrix.Xx;	Xy = mMatrix.Xy;	Xz = mMatrix.Xz;	Xw = mMatrix.Xw;
	Yx = mMatrix.Yx;	Yy = mMatrix.Yy;	Yz = mMatrix.Yz;	Yw = mMatrix.Yw;
	Zx = mMatrix.Zx;	Zy = mMatrix.Zy;	Zz = mMatrix.Zz;	Zw = mMatrix.Zw;
	Wx = mMatrix.Wx;	Wy = mMatrix.Wy;	Wz = mMatrix.Wz;	Ww = mMatrix.Ww;
#endif
}

inline Matrix4f::Matrix4f(XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
}

inline Matrix4f::Matrix4f(const Vec4f& vXAxis,
						  const Vec4f& vYAxis,
						  const Vec4f& vZAxis,
						  const Vec4f& vWAxis)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_loadu_ps(vXAxis.vector));
	_mm_storeu_ps(m + 4, _mm_loadu_ps(vYAxis.vector));
	_mm_storeu_ps(m + 8, _mm_loadu_ps(vZAxis.vector));
	_mm_storeu_ps(m + 12, _mm_loadu_ps(vWAxis.vector));
#else
	xAxis = vXAxis.position; Xw = vXAxis.w;
	yAxis = vYAxis.position; Yw = vYAxis.w;
	zAxis = vZAxis.position; Zw = vZAxis.w;
	wAxis = vWAxis.position; Ww = vWAxis.w;
#endif
}
#pragma endregion

#pragma region Matrix4f Operators
inline XMMATRIX Matrix4f::toXMMatrix()
{
	return XMLoadFloat4x4((XMFLOAT4X4*)this);
}

inline Matrix3f Matrix4f::Get3x3()
{
	return Matrix3f(xAxis, yAxis, zAxis);
}

inline Matrix4f& Matrix4f::operator=(const Matrix4f& mMatrix)
{
	if(this != &mMatrix)
	{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
		_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
		_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
		_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
		xAxis = mMatrix.xAxis; Xw = mMatrix.Xw;
		yAxis = mMatrix.yAxis; Yw = mMatrix.Yw;
		zAxis = mMatrix.zAxis; Zw = mMatrix.Zw;
		wAxis = mMatrix.wAxis; Ww = mMatrix.Ww;
#endif
	}

	return *this;
}

inline Matrix4f& Matrix4f::operator=(Matrix4f&& mMatrix)
{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
		_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
		_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
		_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
		xAxis = mMatrix.xAxis; Xw = mMatrix.Xw;
		yAxis = mMatrix.yAxis; Yw = mMatrix.Yw;
		zAxis = mMatrix.zAxis; Zw = mMatrix.Zw;
		wAxis = mMatrix.wAxis; Ww = mMatrix.Ww;
#endif

	return *this;
}

inline Matrix4f& Matrix4f::operator=(const XMMATRIX& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
	return *this;
}

inline Matrix4f& Matrix4f::operator=(XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
	return *this;
}

inline Matrix4f Matrix4f::operator*(const Matrix4f& mMatrix) const
{
	Matrix4f result;

#ifdef SSE_MATH_AVAILABLE
	const __m128 otherX = _mm_loadu_ps(mMatrix.m);
	const __m128 otherY = _mm_loadu_ps(mMatrix.m + 4);
	const __m128 otherZ = _mm_loadu_ps(mMatrix.m + 8);
	const __m128 otherW = _mm_loadu_ps(mMatrix.m + 12);

	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(Xx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Xy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(result.m, tmp2);

	// get 2nd row
	tmp1 = _mm_set1_ps(Yx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Yy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(result.m + 4, tmp2);

	// get 3rd row
	tmp1 = _mm_set1_ps(Zx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Zy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(result.m + 8, tmp2);

	// get bottom row
	tmp1 = _mm_set1_ps(Wx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Wy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Wz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Ww);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(result.m + 12, tmp2);
#else
	result.Xx = Xx * mMatrix.Xx + Xy * mMatrix.Yx + Xz * mMatrix.Zx + Xw * mMatrix.Wx;
	result.Xy = Xx * mMatrix.Xy + Xy * mMatrix.Yy + Xz * mMatrix.Zy + Xw * mMatrix.Wy;
	result.Xz = Xx * mMatrix.Xz + Xy * mMatrix.Yz + Xz * mMatrix.Zz + Xw * mMatrix.Wz;
	result.Xw = Xx * mMatrix.Xw + Xy * mMatrix.Yw + Xz * mMatrix.Zw + Xw * mMatrix.Ww;
			   
	result.Yx = Yx * mMatrix.Xx + Yy * mMatrix.Yx + Yz * mMatrix.Zx + Yw * mMatrix.Wx;
	result.Yy = Yx * mMatrix.Xy + Yy * mMatrix.Yy + Yz * mMatrix.Zy + Yw * mMatrix.Wy;
	result.Yz = Yx * mMatrix.Xz + Yy * mMatrix.Yz + Yz * mMatrix.Zz + Yw * mMatrix.Wz;
	result.Yw = Yx * mMatrix.Xw + Yy * mMatrix.Yw + Yz * mMatrix.Zw + Yw * mMatrix.Ww;
			   
	result.Zx = Zx * mMatrix.Xx + Zy * mMatrix.Yx + Zz * mMatrix.Zx + Zw * mMatrix.Wx;
	result.Zy = Zx * mMatrix.Xy + Zy * mMatrix.Yy + Zz * mMatrix.Zy + Zw * mMatrix.Wy;
	result.Zz = Zx * mMatrix.Xz + Zy * mMatrix.Yz + Zz * mMatrix.Zz + Zw * mMatrix.Wz;
	result.Zw = Zx * mMatrix.Xw + Zy * mMatrix.Yw + Zz * mMatrix.Zw + Zw * mMatrix.Ww;
		   
	result.Wx = Wx * mMatrix.Xx + Wy * mMatrix.Yx + Wz * mMatrix.Zx + Ww * mMatrix.Wx;
	result.Wy = Wx * mMatrix.Xy + Wy * mMatrix.Yy + Wz * mMatrix.Zy + Ww * mMatrix.Wy;
	result.Wz = Wx * mMatrix.Xz + Wy * mMatrix.Yz + Wz * mMatrix.Zz + Ww * mMatrix.Wz;
	result.Ww = Wx * mMatrix.Xw + Wy * mMatrix.Yw + Wz * mMatrix.Zw + Ww * mMatrix.Ww;
#endif

	return result;
}

inline Matrix4f& Matrix4f::operator*=(const Matrix4f& mMatrix)
{
#ifdef SSE_MATH_AVAILABLE
	const __m128 otherX = _mm_loadu_ps(mMatrix.m);
	const __m128 otherY = _mm_loadu_ps(mMatrix.m + 4);
	const __m128 otherZ = _mm_loadu_ps(mMatrix.m + 8);
	const __m128 otherW = _mm_loadu_ps(mMatrix.m + 12);

	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(Xx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Xy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(m, tmp2);

	// get 2nd row
	tmp1 = _mm_set1_ps(Yx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Yy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(m + 4, tmp2);

	// get 3rd row
	tmp1 = _mm_set1_ps(Zx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Zy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zw);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(m + 8, tmp2);

	// get bottom row
	tmp1 = _mm_set1_ps(Wx);
	tmp2 = _mm_mul_ps(otherX, tmp1);
	tmp1 = _mm_set1_ps(Wy);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherY, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Wz);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherZ, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Ww);
	tmp2 = _mm_add_ps(_mm_mul_ps(otherW, tmp1), tmp2);

	_mm_storeu_ps(m + 12, tmp2);
#else
	Xx = Xx * mMatrix.Xx + Xy * mMatrix.Yx + Xz * mMatrix.Zx + Xw * mMatrix.Wx;
	Xy = Xx * mMatrix.Xy + Xy * mMatrix.Yy + Xz * mMatrix.Zy + Xw * mMatrix.Wy;
	Xz = Xx * mMatrix.Xz + Xy * mMatrix.Yz + Xz * mMatrix.Zz + Xw * mMatrix.Wz;
	Xw = Xx * mMatrix.Xw + Xy * mMatrix.Yw + Xz * mMatrix.Zw + Xw * mMatrix.Ww;
				   
	Yx = Yx * mMatrix.Xx + Yy * mMatrix.Yx + Yz * mMatrix.Zx + Yw * mMatrix.Wx;
	Yy = Yx * mMatrix.Xy + Yy * mMatrix.Yy + Yz * mMatrix.Zy + Yw * mMatrix.Wy;
	Yz = Yx * mMatrix.Xz + Yy * mMatrix.Yz + Yz * mMatrix.Zz + Yw * mMatrix.Wz;
	Yw = Yx * mMatrix.Xw + Yy * mMatrix.Yw + Yz * mMatrix.Zw + Yw * mMatrix.Ww;
				   
	Zx = Zx * mMatrix.Xx + Zy * mMatrix.Yx + Zz * mMatrix.Zx + Zw * mMatrix.Wx;
	Zy = Zx * mMatrix.Xy + Zy * mMatrix.Yy + Zz * mMatrix.Zy + Zw * mMatrix.Wy;
	Zz = Zx * mMatrix.Xz + Zy * mMatrix.Yz + Zz * mMatrix.Zz + Zw * mMatrix.Wz;
	Zw = Zx * mMatrix.Xw + Zy * mMatrix.Yw + Zz * mMatrix.Zw + Zw * mMatrix.Ww;
				   
	Wx = Wx * mMatrix.Xx + Wy * mMatrix.Yx + Wz * mMatrix.Zx + Ww * mMatrix.Wx;
	Wy = Wx * mMatrix.Xy + Wy * mMatrix.Yy + Wz * mMatrix.Zy + Ww * mMatrix.Wy;
	Wz = Wx * mMatrix.Xz + Wy * mMatrix.Yz + Wz * mMatrix.Zz + Ww * mMatrix.Wz;
	Ww = Wx * mMatrix.Xw + Wy * mMatrix.Yw + Wz * mMatrix.Zw + Ww * mMatrix.Ww;
#endif

	return *this;
}
#pragma endregion

#pragma region Matrix4f Math Funcs
inline Matrix4f& Matrix4f::make_identity()
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, g_IdentityX4);
	_mm_storeu_ps(m + 4, g_IdentityY4);
	_mm_storeu_ps(m + 8, g_IdentityZ4);
	_mm_storeu_ps(m + 12, g_IdentityW4);
#else
	ZeroMemory(this, sizeof(Matrix4f));
	Xx = Yy = Zz = Ww = 1.0f;
#endif

	return *this;
}

inline Matrix4f& Matrix4f::make_identity_3x3()
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(1.0f, 0.0f, 0.0f, Xw));
	_mm_storeu_ps(m + 4, _mm_setr_ps(0.0f, 1.0f, 0.0f, Yw));
	_mm_storeu_ps(m + 8, _mm_setr_ps(0.0f, 0.0f, 1.0f, Zw));
#else
	ZeroMemory(this, 48);
	Xx = Yy = Zz = 1.0f;
#endif

	return *this;
}

// actually faster than DirectX Version :)
inline Matrix4f& Matrix4f::Rotate_GlobalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Yy = fCosAngle; tmp.Yz = fSinAngle;
	tmp.Zy = -fSinAngle; tmp.Zz = fCosAngle;

	return *this *= tmp;
}

inline Matrix4f& Matrix4f::Rotate_GlobalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Xx = fCosAngle; tmp.Xz = -fSinAngle;
	tmp.Zx = fSinAngle; tmp.Zz = fCosAngle;

	return *this *= tmp;
}

inline Matrix4f& Matrix4f::Rotate_GlobalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Xx = fCosAngle; tmp.Xy = fSinAngle;
	tmp.Yx = -fSinAngle; tmp.Yy = fCosAngle;

	return *this *= tmp;
}

inline Matrix4f& Matrix4f::Rotate_GlobalX_Degrees(float fDegrees)
{
	return Rotate_GlobalX_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Rotate_GlobalY_Degrees(float fDegrees)
{
	return Rotate_GlobalY_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Rotate_GlobalZ_Degrees(float fDegrees)
{
	return Rotate_GlobalZ_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Rotate_LocalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Yy = fCosAngle; tmp.Yz = fSinAngle;
	tmp.Zy = -fSinAngle; tmp.Zz = fCosAngle;

	return *this = tmp * (*this);
}

inline Matrix4f& Matrix4f::Rotate_LocalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Xx = fCosAngle; tmp.Xz = -fSinAngle;
	tmp.Zx = fSinAngle; tmp.Zz = fCosAngle;

	return *this = tmp * (*this);
}

inline Matrix4f& Matrix4f::Rotate_LocalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4f tmp;
	tmp.Xx = fCosAngle; tmp.Xy = fSinAngle;
	tmp.Yx = -fSinAngle; tmp.Yy = fCosAngle;

	return *this = tmp * (*this);
}

inline Matrix4f& Matrix4f::Rotate_LocalX_Degrees(float fDegrees)
{
	return Rotate_LocalX_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Rotate_LocalY_Degrees(float fDegrees)
{
	return Rotate_LocalY_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Rotate_LocalZ_Degrees(float fDegrees)
{
	return Rotate_LocalZ_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4f& Matrix4f::Scale(float fXScale, float fYScale, float fZScale)
{
	xAxis *= fXScale;
	yAxis *= fYScale;
	zAxis *= fZScale;

	return *this;
}

inline Matrix4f& Matrix4f::Scale(const Vec3f& vScale) 
{
	return Scale(vScale.x, vScale.y, vScale.z);
}

inline Matrix4f& Matrix4f::SetScale(float fXScale, float fYScale, float fZScale)
{
	xAxis.normalize();
	xAxis *= fXScale;

	yAxis.normalize();
	yAxis *= fYScale;

	zAxis.normalize();
	zAxis *= fZScale;

	return *this;
}

inline Matrix4f& Matrix4f::SetScale(const Vec3f& vScale)
{
	return SetScale(vScale.x, vScale.y, vScale.z);
}

inline Matrix4f& Matrix4f::SetScale(float fScale)
{
	return SetScale(fScale, fScale, fScale);
}

inline Vec3f Matrix4f::GetScale() const
{
	Vec3f retVal(xAxis.magnitude(), yAxis.magnitude(), zAxis.magnitude());
	return retVal;
}

inline Matrix4f& Matrix4f::Translate(float fX, float fY, float fZ)
{
	Wx += fX;
	Wy += fY;
	Wz += fZ;

	return *this;
}

inline Matrix4f& Matrix4f::Translate(Vec3f vTranslation) 
{
	position += vTranslation; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveForward(float fMovement) 
{
	position += zAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveBackward(float fMovement) 
{
	position -= zAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveLeft(float fMovement)
{
	position -= xAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveRight(float fMovement)
{
	position += xAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveUp(float fMovement)
{
	position += yAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::MoveDown(float fMovement)
{
	position -= yAxis * fMovement; 
	return *this;
}

inline Matrix4f& Matrix4f::Transpose()
{
#ifdef SSE_MATH_AVAILABLE
	__m128 row1 = _mm_loadu_ps(m);
	__m128 row2 = _mm_loadu_ps(m + 4);
	__m128 row3 = _mm_loadu_ps(m + 8);
	__m128 row4 = _mm_loadu_ps(m + 12);

	__m128 tmp1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 tmp2 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 tmp3 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 tmp4 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(3, 2, 3, 2));

	_mm_storeu_ps(m, _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0)));
	_mm_storeu_ps(m + 4, _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1)));
	_mm_storeu_ps(m + 8, _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0)));
	_mm_storeu_ps(m + 12, _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1)));
#else
	swap(Xy, Yx);
    swap(Xz, Zx);
    swap(Yz, Zy);
    swap(Wx, Xw);
    swap(Wy, Yw);
    swap(Wz, Zw);
#endif

	return *this;
}

inline Matrix4f& Matrix4f::Transpose3x3()
{
	std::swap(Xy, Yx);
	std::swap(Xz, Zx);
	std::swap(Yz, Zy);

	return *this;
}

inline Matrix4f& Matrix4f::NormalizeXYZ()
{
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

	return *this;
}

inline Matrix4f& Matrix4f::Invert()
{
	return *this = XMMatrixInverse(NULL, XMLoadFloat4x4((XMFLOAT4X4*)this));
}

inline Matrix4f& Matrix4f::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	zAxis = mPos - position;
	zAxis.normalize();

	xAxis = CrossProduct(vWorldUp, zAxis);
	xAxis.normalize();

	yAxis = CrossProduct(zAxis, xAxis);
	yAxis.normalize();

	return *this;
}

inline Matrix4f& Matrix4f::TurnTo(const Vec3f& vPos, float fTurnModifier)
{
	
}
#pragma endregion

#pragma region Matrix4f Camera Funcs
inline Matrix4f& Matrix4f::MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(xScale, 0.0f, 0.0f, 0.0f));
	_mm_storeu_ps(m + 4, _mm_setr_ps(0.0f, yScale, 0.0f, 0.0f));
	_mm_storeu_ps(m + 8, _mm_setr_ps(0.0f, 0.0f, fFarClip / (fFarClip - fNearClip), 1.0f));
	_mm_storeu_ps(m + 12, _mm_setr_ps(0.0f, 0.0f, -fNearClip * fFarClip / (fFarClip - fNearClip), 0.0f));	
#else
	Xx = xScale; Xy = Xz = Xw = 0;
	Yx = 0; Yy = yScale; Yz = yW = 0;
	Zx = Zy = 0; Zz = fFarClip / (fFarClip - fNearClip); Zw = 1;
	Wx = Wy = 0; Wz = -fNearClip * fFarClip / (fFarClip - fNearClip); Ww = 0;
#endif
	return *this;
}

inline Matrix4f& Matrix4f::MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(2 / fWidth, 0, 0, 0));
	_mm_storeu_ps(m + 4, _mm_setr_ps(0, 2 / fHeight, 0, 0));
	_mm_storeu_ps(m + 8, _mm_setr_ps(0, 0, 1 / (fFar - fNear), 0));
	_mm_storeu_ps(m + 12, _mm_setr_ps(0, 0, -fNear / (fFar - fNear), 1));
#else
	Xx = 2 / fWidth; Xy = Xz = Xw = 0;
	Yx = 0; Yy = 2 / fHeight; Yz = yW = 0;
	Zx = Zy = 0; Zz = 1 / (fFar - fNear); Zw = 0;
	Wx = Wy = 0; Wz = -fNear / (fFar - fNear); Ww = 1;
#endif
	return *this;
}

inline Matrix4f& Matrix4f::OrthoNormalInvert()
{
	Matrix4f tmp(*this);

	Transpose3x3();

	Wx = -DotProduct(tmp.position, tmp.xAxis);
	Wy = -DotProduct(tmp.position, tmp.yAxis);
	Wz = -DotProduct(tmp.position, tmp.zAxis);

	return *this;
}

inline Matrix4f& Matrix4f::MakeTextureMatrixOffsetLH(unsigned int unWidth, unsigned int unHeight)
{
#ifdef SSE_MATH_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(0.5f, 0.0f, 0.0f, 0.0f));
	_mm_storeu_ps(m + 4, _mm_setr_ps(0.0f, -0.5f, 0.0f, 0.0f));
	_mm_storeu_ps(m + 8, _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_mm_storeu_ps(m + 12, _mm_setr_ps(0.5f + (0.5f / unWidth), 0.5f + (0.5f / unHeight), 0.0f, 1.0f));
#else
	Xx = 0.5f; Xy = Xz = Xw = 0;
	Yx = 0; Yy = -0.5f; Yz = yW = 0;
	Zx = Zy = 0; Zz = 1; Zw = 0;
	Wx = 0.5f + (0.5f / unWidth); Wy = 0.5f + (0.5f / unHeight); Wz = 0.0f; Ww = 1;
#endif

	return *this;
}
#pragma endregion

#pragma region Matrix4f Global Funcs
inline Matrix4f MatrixTranspose(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	tmp.Transpose();

	return tmp;
}

inline Matrix4f MatrixTranspos3x3(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	tmp.Transpose3x3();

	return tmp;
}

inline Matrix4f MatrixScale(const Matrix4f& mMatrix, float fXScale, float fYScale, float fZScale)
{
	Matrix4f tmp(mMatrix);

	tmp.Scale(fXScale, fYScale, fZScale);

	return tmp;
}

inline Matrix4f MatrixInverse(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	tmp.Invert();

	return tmp;
}

inline Matrix4f Lerp(const Matrix4f& MatrixA, const Matrix4f& MatrixB, float fLambda)
{
	Matrix4f result;

	result.xAxis = Lerp(MatrixA.xAxis, MatrixB.xAxis, fLambda);
	result.yAxis = Lerp(MatrixA.yAxis, MatrixB.yAxis, fLambda);
	result.zAxis = Lerp(MatrixA.zAxis, MatrixB.zAxis, fLambda);
	result.wAxis = Lerp(MatrixA.wAxis, MatrixB.wAxis, fLambda);

	return result;
}
#pragma endregion