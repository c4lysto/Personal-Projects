
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
	_mm_storeu_ps(m, _mm_set_ps(fXw, fXz, fXy, fXx));
	_mm_storeu_ps(m + 4, _mm_set_ps(fYw, fYz, fYy, fYx));
	_mm_storeu_ps(m + 8, _mm_set_ps(fZw, fZz, fZy, fZx));
	_mm_storeu_ps(m + 12, _mm_set_ps(fWw, fWz, fWy, fWx));
#else
	Xx = fXx;	Xy = fXy;	Xz = fXz;	Xw = fXw;
	Yx = fYx;	Yy = fYy;	Yz = fYz;	Yw = fYw;
	Zx = fZx;	Zy = fZy;	Zz = fZz;	Zw = fZw;
	Wx = fWx;	Wy = fWy;	Wz = fWz;	Ww = fWw;
#endif
}

inline Matrix4f::Matrix4f(const Matrix4f&& mMatrix)
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

inline Matrix4f::Matrix4f(const XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
}

inline Matrix4f::Matrix4f(const Vec4f&& vXAxis,
						  const Vec4f&& vYAxis,
						  const Vec4f&& vZAxis,
						  const Vec4f&& vWAxis)
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

inline float Matrix4f::operator[](size_t ucIndex) const 
{
	return m[ucIndex]; 
}

inline Matrix4f& Matrix4f::operator=(const Matrix4f&& mMatrix)
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

inline Matrix4f& Matrix4f::operator=(const XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);

	return *this;
}

inline Matrix4f Matrix4f::operator*(const Matrix4f& mMatrix) const
{
	return XMMatrixMultiply(XMLoadFloat4x4((XMFLOAT4X4*)this), XMLoadFloat4x4((XMFLOAT4X4*)&mMatrix));
}

inline Matrix4f& Matrix4f::operator*=(const Matrix4f& mMatrix)
{
	*this = *this * mMatrix;

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
	_mm_storeu_ps(m, _mm_set_ss(1.0f));
	_mm_storeu_ps(m + 4, _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_mm_storeu_ps(m + 8, _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f));
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
	position += zAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::MoveBackward(float fMovement) 
{
	position -= zAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::MoveLeft(float fMovement)
{
	position -= xAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::MoveRight(float fMovement)
{
	position += xAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::MoveUp(float fMovement)
{
	position += yAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::MoveDown(float fMovement)
{
	position -= yAxis * fMovement; return *this;
}

inline Matrix4f& Matrix4f::Transpose()
{
	return *this = XMMatrixTranspose(XMLoadFloat4x4((XMFLOAT4X4*)this));
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

inline Matrix4f& Matrix4f::LookAt(const Vec3f& mPos)
{
	return *this = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&position), XMLoadFloat3((XMFLOAT3*)&mPos), g_XMIdentityR1.v);
}

inline Matrix4f& Matrix4f::TurnTo(const Vec3f& vPos, float fTurnModifier)
{
	
}
#pragma endregion

#pragma region Matrix4f Camera Funcs
inline Matrix4f& Matrix4f::MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	return *this = XMMatrixPerspectiveFovLH(fFOV, fAspectRatio, fNearClip, fFarClip);
}

inline Matrix4f& Matrix4f::MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar)
{
	return *this = XMMatrixOrthographicLH(fWidth, fHeight, fNear, fFar);
}

inline Matrix4f& Matrix4f::OrthoNormalInvert()
{
	Matrix4f tmp(*this);

	Transpose3x3();

	Wx = -Dot_Product(tmp.position, tmp.xAxis);
	Wy = -Dot_Product(tmp.position, tmp.yAxis);
	Wz = -Dot_Product(tmp.position, tmp.zAxis);

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