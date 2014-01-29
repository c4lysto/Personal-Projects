#ifdef SSE_MATH_AVAILABLE
#pragma region Matrix4fA Constructors
inline Matrix4fA::Matrix4fA()
{
	row1 = g_IdentityX4;
	row2 = g_IdentityY4;
	row3 = g_IdentityZ4;
	row4 = g_IdentityW4;
}

inline Matrix4fA::Matrix4fA(float fXx, float fXy, float fXz, float fXw,
							float fYx, float fYy, float fYz, float fYw,
							float fZx, float fZy, float fZz, float fZw,
							float fWx, float fWy, float fWz, float fWw)
{
	row1 = _mm_setr_ps(fXx, fXy, fXz, fXw);
	row2 = _mm_setr_ps(fYx, fYy, fYz, fYw);
	row3 = _mm_setr_ps(fZx, fZy, fZz, fZw);
	row4 = _mm_setr_ps(fWx, fWy, fWz, fWw);
}

inline Matrix4fA::Matrix4fA(const Matrix4f& mMatrix)
{
	row1 = _mm_loadu_ps(mMatrix.m);
	row2 = _mm_loadu_ps(mMatrix.m + 4);
	row3 = _mm_loadu_ps(mMatrix.m + 8);
	row4 = _mm_loadu_ps(mMatrix.m + 12);
}

inline Matrix4fA::Matrix4fA(const Matrix4fA& mMatrix)
{
	row1 = mMatrix.row1;
	row2 = mMatrix.row2;
	row3 = mMatrix.row3;
	row4 = mMatrix.row4;
}

inline Matrix4fA::Matrix4fA(Matrix4fA&& mMatrix)
{
	row1 = mMatrix.row1;
	row2 = mMatrix.row2;
	row3 = mMatrix.row3;
	row4 = mMatrix.row4;
}

inline Matrix4fA::Matrix4fA(XMMATRIX&& mMatrix)
{
	row1 = mMatrix.r[0];
	row2 = mMatrix.r[1];
	row3 = mMatrix.r[2];
	row4 = mMatrix.r[3];
}

inline Matrix4fA::Matrix4fA(const Vec4fA& vXAxis,
						    const Vec4fA& vYAxis,
						    const Vec4fA& vZAxis,
						    const Vec4fA& vWAxis)
{
	row1 = vXAxis.row;
	row2 = vYAxis.row;
	row3 = vZAxis.row;
	row4 = vWAxis.row;
}
#pragma endregion

#pragma region Matrix4fA Operators
inline XMMATRIX Matrix4fA::toXMMatrix() const
{
	return *(XMMATRIX*)this;
}

inline Matrix3f Matrix4fA::Get3x3() const
{
	return Matrix3f(xAxis, yAxis, zAxis);
}

inline Matrix4fA& Matrix4fA::operator=(Matrix4fA&& mMatrix)
{
	row1 = mMatrix.row1;
	row2 = mMatrix.row2;
	row3 = mMatrix.row3;
	row4 = mMatrix.row4;

	return *this;
}

inline Matrix4fA& Matrix4fA::operator=(XMMATRIX&& mMatrix)
{
	row1 = mMatrix.r[0];
	row2 = mMatrix.r[1];
	row3 = mMatrix.r[2];
	row4 = mMatrix.r[3];

	return *this;
}

inline Matrix4fA Matrix4fA::operator*(const Matrix4fA& mMatrix) const
{
	Matrix4fA result;

	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(Xx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Xy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	result.row1 = tmp2;

	// get 2nd row
	tmp1 = _mm_set1_ps(Yx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Yy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	result.row2 = tmp2;

	// get 3rd row
	tmp1 = _mm_set1_ps(Zx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Zy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	result.row3 = tmp2;

	// get 4th row
	tmp1 = _mm_set1_ps(Wx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Wy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Wz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Ww);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	result.row4 = tmp2;

	return result;
}

inline Matrix4fA& Matrix4fA::operator*=(const Matrix4fA& mMatrix)
{
	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(Xx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Xy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Xw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	row1 = tmp2;

	// get 2nd row
	tmp1 = _mm_set1_ps(Yx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Yy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Yw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	row2 = tmp2;

	// get 3rd row
	tmp1 = _mm_set1_ps(Zx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Zy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Zw);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	row3 = tmp2;

	// get 4th row
	tmp1 = _mm_set1_ps(Wx);
	tmp2 = _mm_mul_ps(mMatrix.row1, tmp1);
	tmp1 = _mm_set1_ps(Wy);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row2, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Wz);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row3, tmp1), tmp2);
	tmp1 = _mm_set1_ps(Ww);
	tmp2 = _mm_add_ps(_mm_mul_ps(mMatrix.row4, tmp1), tmp2);

	row4 = tmp2;

	return *this;
}
#pragma endregion

#pragma region Matrix4fA Math Funcs
inline Matrix4fA& Matrix4fA::make_identity()
{
	row1 = g_IdentityX4;
	row2 = g_IdentityY4;
	row3 = g_IdentityZ4;
	row4 = g_IdentityW4;

	return *this;
}

inline Matrix4fA& Matrix4fA::make_identity_3x3()
{
	row1 = _mm_setr_ps(1.0f, 0.0f, 0.0f, Xw);
	row2 = _mm_setr_ps(0.0f, 1.0f, 0.0f, Yw);
	row3 = _mm_setr_ps(0.0f, 0.0f, 1.0f, Zw);

	return *this;
}

// actually faster than DirectX Version :)
inline Matrix4fA& Matrix4fA::Rotate_GlobalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row2 = _mm_setr_ps(0.0f, fCosAngle, fSinAngle, 0.0f);
	tmp.row3 = _mm_setr_ps(0.0f, -fSinAngle, fCosAngle, 0.0f);

	return *this *= tmp;
}

inline Matrix4fA& Matrix4fA::Rotate_GlobalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row1 = _mm_setr_ps(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	tmp.row3 = _mm_setr_ps(fSinAngle, 0.0f, fCosAngle, 0.0f);

	return *this *= tmp;
}

inline Matrix4fA& Matrix4fA::Rotate_GlobalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row1 = _mm_setr_ps(fCosAngle, fSinAngle, 0.0f, 0.0f);
	tmp.row2 = _mm_setr_ps(-fSinAngle, fCosAngle, 0.0f, 0.0f);

	return *this *= tmp;
}

inline Matrix4fA& Matrix4fA::Rotate_GlobalX_Degrees(float fDegrees)
{
	return Rotate_GlobalX_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Rotate_GlobalY_Degrees(float fDegrees)
{
	return Rotate_GlobalY_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Rotate_GlobalZ_Degrees(float fDegrees)
{
	return Rotate_GlobalZ_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Rotate_LocalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row2 = _mm_setr_ps(0.0f, fCosAngle, fSinAngle, 0.0f);
	tmp.row3 = _mm_setr_ps(0.0f, -fSinAngle, fCosAngle, 0.0f);

	return *this = tmp * (*this);
}

inline Matrix4fA& Matrix4fA::Rotate_LocalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row1 = _mm_setr_ps(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	tmp.row3 = _mm_setr_ps(fSinAngle, 0.0f, fCosAngle, 0.0f);

	return *this = tmp * (*this);
}

inline Matrix4fA& Matrix4fA::Rotate_LocalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Matrix4fA tmp;
	tmp.row1 = _mm_setr_ps(fCosAngle, fSinAngle, 0.0f, 0.0f);
	tmp.row2 = _mm_setr_ps(-fSinAngle, fCosAngle, 0.0f, 0.0f);

	return *this = tmp * (*this);
}

inline Matrix4fA& Matrix4fA::Rotate_LocalX_Degrees(float fDegrees)
{
	return Rotate_LocalX_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Rotate_LocalY_Degrees(float fDegrees)
{
	return Rotate_LocalY_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Rotate_LocalZ_Degrees(float fDegrees)
{
	return Rotate_LocalZ_Radians(DEG_TO_RAD(fDegrees));
}

inline Matrix4fA& Matrix4fA::Scale(float fXScale, float fYScale, float fZScale)
{
	xAxis *= fXScale;
	yAxis *= fYScale;
	zAxis *= fZScale;
}

inline Matrix4fA& Matrix4fA::Scale(const Vec3f& vScale) 
{
	return Scale(vScale.x, vScale.y, vScale.z);
}

inline Matrix4fA& Matrix4fA::SetScale(float fXScale, float fYScale, float fZScale)
{
	xAxis.normalize();
	xAxis *= fXScale;

	yAxis.normalize();
	yAxis *= fYScale;

	zAxis.normalize();
	zAxis *= fZScale;
}

inline Matrix4fA& Matrix4fA::SetScale(const Vec3f& vScale)
{
	return SetScale(vScale.x, vScale.y, vScale.z);
}

inline Matrix4fA& Matrix4fA::SetScale(float fScale)
{
	return SetScale(fScale, fScale, fScale);
}

inline Vec3f Matrix4fA::GetScale() const
{
	Vec3f retVal(xAxis.magnitude(), yAxis.magnitude(), zAxis.magnitude());
	return retVal;
}

inline Matrix4fA& Matrix4fA::Translate(float fX, float fY, float fZ)
{
	row4 = _mm_add_ps(row4, _mm_setr_ps(fX, fY, fZ, 0.0f));

	return *this;
}

inline Matrix4fA& Matrix4fA::Translate(Vec3f vTranslation) 
{
	position += vTranslation; 

	return *this;
}

inline Matrix4fA& Matrix4fA::MoveForward(float fMovement) 
{
	position += zAxis * fMovement; 
	return *this;
}

inline Matrix4fA& Matrix4fA::MoveBackward(float fMovement) 
{
	position -= zAxis * fMovement;
	return *this;
}

inline Matrix4fA& Matrix4fA::MoveLeft(float fMovement)
{
	position -= xAxis * fMovement;
	return *this;
}

inline Matrix4fA& Matrix4fA::MoveRight(float fMovement)
{
	position += xAxis * fMovement;
	return *this;
}

inline Matrix4fA& Matrix4fA::MoveUp(float fMovement)
{
	position += yAxis * fMovement;
	return *this;
}

inline Matrix4fA& Matrix4fA::MoveDown(float fMovement)
{
	position -= yAxis * fMovement;
	return *this;
}

inline Matrix4fA& Matrix4fA::Transpose()
{
	__m128 tmp1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 tmp2 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 tmp3 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 tmp4 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(3, 2, 3, 2));

	row1 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	row2 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	row3 = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	row4 = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1));

	return *this;
}

inline Matrix4fA& Matrix4fA::Transpose3x3()
{
	std::swap(Xy, Yx);
	std::swap(Xz, Zx);
	std::swap(Yz, Zy);

	return *this;
}

inline Matrix4fA& Matrix4fA::NormalizeXYZ()
{
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

	return *this;
}

inline Matrix4fA& Matrix4fA::Invert()
{
	return *this = XMMatrixInverse(NULL, XMLoadFloat4x4((XMFLOAT4X4*)this));
}

inline Matrix4fA& Matrix4fA::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	Vec4fA vecToPos(mPos, Ww);

	vecToPos.row = _mm_sub_ps(vecToPos.row, row4);
	vecToPos.normalize();

	// zAxis
	zAxis = vecToPos.position;

	// xAxis;
	xAxis = CrossProduct(vWorldUp, zAxis);
	xAxis.normalize();

	// yAxis;
	yAxis = CrossProduct(zAxis, xAxis);
	yAxis.normalize();

	return *this;
}

inline Matrix4fA& Matrix4fA::TurnTo(const Vec3f& vPos, float fTurnModifier)
{
	
}
#pragma endregion

#pragma region Matrix4fA Camera Funcs
inline Matrix4fA& Matrix4fA::MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	float yScale = 1 / tan(fFOV * 0.5f);
	float xScale = yScale / fAspectRatio;

	row1 = _mm_setr_ps(xScale, 0.0f, 0.0f, 0.0f);
	row2 = _mm_setr_ps(0.0f, yScale, 0.0f, 0.0f);
	row3 = _mm_setr_ps(0.0f, 0.0f, fFarClip / (fFarClip - fNearClip), 1.0f);
	row4 = _mm_setr_ps(0.0f, 0.0f, -fNearClip * fFarClip / (fFarClip - fNearClip), 0.0f);

	return *this;
}

inline Matrix4fA& Matrix4fA::MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar)
{
	row1 = _mm_setr_ps(2 / fWidth, 0, 0, 0);
	row2 = _mm_setr_ps(0, 2 / fHeight, 0, 0);
	row3 = _mm_setr_ps(0, 0, 1 / (fFar - fNear), 0);
	row4 = _mm_setr_ps(0, 0, -fNear / (fFar - fNear), 1);

	return *this;
}

inline Matrix4fA& Matrix4fA::OrthoNormalInvert()
{
	Matrix4fA tmp(*this);

	Transpose3x3();

	Wx = -DotProduct(tmp.position, tmp.xAxis);
	Wy = -DotProduct(tmp.position, tmp.yAxis);
	Wz = -DotProduct(tmp.position, tmp.zAxis);

	return *this;
}

inline Matrix4fA& Matrix4fA::MakeTextureMatrixOffsetLH(unsigned int unWidth, unsigned int unHeight)
{
	row1 = _mm_setr_ps(0.5f, 0.0f, 0.0f, 0.0f);
	row2 = _mm_setr_ps(0.0f, -0.5f, 0.0f, 0.0f);
	row3 =  _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
	row4 = _mm_setr_ps(0.5f + (0.5f / unWidth), 0.5f + (0.5f / unHeight), 0.0f, 1.0f);

	return *this;
}
#pragma endregion

#pragma region Matrix4fA Global Funcs
inline Matrix4fA MatrixTranspose(const Matrix4fA& mMatrix)
{
	Matrix4fA tmp(mMatrix);

	tmp.Transpose();

	return tmp;
}

inline Matrix4fA MatrixTranspos3x3(const Matrix4fA& mMatrix)
{
	Matrix4fA tmp(mMatrix);

	tmp.Transpose3x3();

	return tmp;
}

inline Matrix4fA MatrixScale(const Matrix4fA& mMatrix, float fXScale, float fYScale, float fZScale)
{
	Matrix4fA tmp(mMatrix);

	tmp.Scale(fXScale, fYScale, fZScale);

	return tmp;
}

inline Matrix4fA MatrixInverse(const Matrix4fA& mMatrix)
{
	Matrix4fA tmp(mMatrix);

	tmp.Invert();

	return tmp;
}

inline Matrix4fA Lerp(const Matrix4fA& MatrixA, const Matrix4fA& MatrixB, float fLambda)
{
	Matrix4fA result;

	result.xAxis = Lerp(MatrixA.xAxis, MatrixB.xAxis, fLambda);
	result.yAxis = Lerp(MatrixA.yAxis, MatrixB.yAxis, fLambda);
	result.zAxis = Lerp(MatrixA.zAxis, MatrixB.zAxis, fLambda);
	result.wAxis = Lerp(MatrixA.wAxis, MatrixB.wAxis, fLambda);

	return result;
}
#pragma endregion
#endif // SSE_MATH_AVAILABLE