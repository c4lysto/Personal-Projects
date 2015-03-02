
__forceinline Mat44V::Mat44V(const float& fXx, const float& fXy, const float& fXz, const float& fXw,
							 const float& fYx, const float& fYy, const float& fYz, const float& fYw,
							 const float& fZx, const float& fZy, const float& fZz, const float& fZw,
							 const float& fWx, const float& fWy, const float& fWz, const float& fWw) :
	xAxis(fXx, fXy, fXz, fXw),
	yAxis(fYx, fYy, fYz, fYw),
	zAxis(fZx, fZy, fZz, fZw),
	wAxis(fWx, fWy, fWz, fWw)
{
}

__forceinline Mat44V::Mat44V(Mat44V_In mMatrix) : 
	xAxis(mMatrix.xAxis),
	yAxis(mMatrix.yAxis),
	zAxis(mMatrix.zAxis),
	wAxis(mMatrix.wAxis)
{
}

__forceinline Mat44V::Mat44V(Mat44V&& mMatrix) : 
	xAxis(move(mMatrix.xAxis)),
	yAxis(move(mMatrix.yAxis)),
	zAxis(move(mMatrix.zAxis)),
	wAxis(move(mMatrix.wAxis))
{
}

__forceinline Mat44V::Mat44V(XMMATRIX&& mMatrix) : 
	xAxis(move(mMatrix.r[0])),
	yAxis(move(mMatrix.r[1])),
	zAxis(move(mMatrix.r[2])),
	wAxis(move(mMatrix.r[3]))
{
}

__forceinline Mat44V::Mat44V(Vec4V_In vXAxis,
							 Vec4V_In vYAxis,
							 Vec4V_In vZAxis,
							 Vec4V_In vWAxis) : 
	xAxis(vXAxis),
	yAxis(vYAxis),
	zAxis(vZAxis),
	wAxis(vWAxis)
{
}

inline Mat44V::Mat44V(eIdentityInitializer eIdentity) :
	xAxis(g_IdentityX4V),
	yAxis(g_IdentityY4V),
	zAxis(g_IdentityZ4V),
	wAxis(g_IdentityW4V)
{
}

inline Mat44V::Mat44V(eXRotationInitializer eXRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = g_IdentityX4V;
	yAxis = Vec4V(0.0f, fCosAngle, fSinAngle, 0.0f);
	zAxis = Vec4V(0.0f, -fSinAngle, fCosAngle, 0.0f);
	wAxis = g_IdentityW4V;
}

inline Mat44V::Mat44V(eYRotationInitializer eYRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec4V(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	yAxis = g_IdentityY4V;
	zAxis = Vec4V(fSinAngle, 0.0f, fCosAngle, 0.0f);
	wAxis = g_IdentityW4V;
}

inline Mat44V::Mat44V(eZRotationInitializer eZRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec4V(fCosAngle, fSinAngle, 0.0f, 0.0f);
	yAxis = Vec4V(-fSinAngle, fCosAngle, 0.0f, 0.0f);
	zAxis = g_IdentityZ4V;
	wAxis = g_IdentityW4V;
}

//__forceinline Mat44V::operator XMMATRIX() const
//{
//	return *(XMMATRIX*)this;
//}
//
//__forceinline Mat44V::operator XMMATRIX*() const
//{
//	return (XMMATRIX*)this;
//}

__forceinline Mat44V_ConstRef Mat44V::operator=(Mat44V_In mMatrix)
{
	if(this != &mMatrix)
	{
		xAxis = mMatrix.xAxis;
		yAxis = mMatrix.yAxis;
		zAxis = mMatrix.zAxis;
		wAxis = mMatrix.wAxis;
	}
	return *this;
}

__forceinline Mat44V_ConstRef Mat44V::operator=(Mat44V&& mMatrix)
{
	if(this != &mMatrix)
	{
		xAxis = move(mMatrix.xAxis);
		yAxis = move(mMatrix.yAxis);
		zAxis = move(mMatrix.zAxis);
		wAxis = move(mMatrix.wAxis);
	}
	return *this;
}

__forceinline Mat44V_ConstRef Mat44V::operator=(XMMATRIX&& mMatrix)
{
	row1 = move(mMatrix.r[0]);
	row2 = move(mMatrix.r[1]);
	row3 = move(mMatrix.r[2]);
	row4 = move(mMatrix.r[3]);

	return *this;
}

inline Mat44V Mat44V::operator*(Mat44V_In mMatrix) const
{
	Mat44V result;

	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(xAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(xAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	result.row1 = tmp2;

	// get 2nd row
	tmp1 = VectorSet(yAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(yAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	result.row2 = tmp2;

	// get 3rd row
	tmp1 = VectorSet(zAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(zAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	result.row3 = tmp2;

	// get 4th row
	tmp1 = VectorSet(wAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(wAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	result.row4 = tmp2;

	return result;
}

inline void Mat44V::operator*=(Mat44V_In mMatrix)
{
	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(xAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(xAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	row1 = tmp2;

	// get 2nd row
	tmp1 = VectorSet(yAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(yAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	row2 = tmp2;

	// get 3rd row
	tmp1 = VectorSet(zAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(zAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	row3 = tmp2;

	// get 4th row
	tmp1 = VectorSet(wAxis.x);
	tmp2 = VectorMultiply(mMatrix.row1, tmp1);
	tmp1 = VectorSet(wAxis.y);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row2, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.z);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row3, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.w);
	tmp2 = VectorAdd(VectorMultiply(mMatrix.row4, tmp1), tmp2);

	row4 = tmp2;
}

__forceinline Mat44V_Out Mat44V::operator+(Mat44V_In rhs) const
{
	return Mat44V(xAxis + rhs.GetXAxisRef(), yAxis + rhs.GetYAxisRef(), zAxis + rhs.GetZAxisRef(), wAxis + rhs.GetWAxisRef());
}

__forceinline void Mat44V::operator+=(Mat44V_In rhs)
{
	xAxis += rhs.GetXAxisRef(); yAxis += rhs.GetYAxisRef(); zAxis += rhs.GetZAxisRef(); wAxis += rhs.GetWAxisRef();
}

__forceinline Mat44V_Out Mat44V::operator-(Mat44V_In rhs) const
{
	return Mat44V(xAxis - rhs.GetXAxisRef(), yAxis - rhs.GetYAxisRef(), zAxis - rhs.GetZAxisRef(), wAxis - rhs.GetWAxisRef());
}

__forceinline void Mat44V::operator-=(Mat44V_In rhs)
{
	xAxis -= rhs.GetXAxisRef(); yAxis -= rhs.GetYAxisRef(); zAxis -= rhs.GetZAxisRef(); wAxis -= rhs.GetWAxisRef();
}

// actually faster than DirectX Version :)
__forceinline void Mat44V::Rotate_GlobalX(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_X, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_GlobalY(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_Y, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_GlobalZ(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_Z, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_LocalX(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_X, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44V::Rotate_LocalY(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_Y, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44V::Rotate_LocalZ(const float& fRadians)
{
	Mat44V tmp(INIT_ROTATION_Z, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44V::Scale(Vec3V_In vScale)
{
	xAxis.SetXYZ(xAxis.GetXYZ() * vScale.GetX());
	yAxis.SetXYZ(yAxis.GetXYZ() * vScale.GetY());
	zAxis.SetXYZ(zAxis.GetXYZ() * vScale.GetZ());
}

__forceinline void Mat44V::SetScale(Vec3V_In vScale)
{
	xAxis.SetXYZ(::Normalize(xAxis.GetXYZ()) * vScale.GetX());
	yAxis.SetXYZ(::Normalize(yAxis.GetXYZ()) * vScale.GetY());
	zAxis.SetXYZ(::Normalize(zAxis.GetXYZ()) * vScale.GetZ());
}

__forceinline Vec3V Mat44V::GetScale() const
{
	Vec3V retVal(Mag(xAxis), Mag(yAxis), Mag(zAxis));
	return retVal;
}

__forceinline void Mat44V::Translate(Vec3V_In vTranslation)
{
	wAxis.SetXYZ(wAxis.GetXYZ() + vTranslation);
}

inline void Mat44V::Transpose()
{
	Vector tmp1 = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::X2, VecElem::Y2>(row1, row2);
	Vector tmp2 = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::X2, VecElem::Y2>(row3, row4);
	Vector tmp3 = VectorPermute<VecElem::Z1, VecElem::W1, VecElem::Z2, VecElem::W2>(row1, row2);
	Vector tmp4 = VectorPermute<VecElem::Z1, VecElem::W1, VecElem::Z2, VecElem::W2>(row3, row4);
	row1 = VectorPermute<VecElem::X1, VecElem::Z1, VecElem::X2, VecElem::Z2>(tmp1, tmp2);
	row2 = VectorPermute<VecElem::Y1, VecElem::W1, VecElem::Y2, VecElem::W2>(tmp1, tmp2);
	row3 = VectorPermute<VecElem::X1, VecElem::Z1, VecElem::X2, VecElem::Z2>(tmp3, tmp4);
	row4 = VectorPermute<VecElem::Y1, VecElem::W1, VecElem::Y2, VecElem::W2>(tmp3, tmp4);
}

__forceinline void Mat44V::Transpose3x3()
{
	std::swap(xAxis.y, yAxis.x);
	std::swap(xAxis.z, zAxis.x);
	std::swap(yAxis.z, zAxis.y);
}

__forceinline void Mat44V::Normalize()
{
	xAxis.Normalize();
	yAxis.Normalize();
	zAxis.Normalize();
}

__forceinline void Mat44V::Invert()
{
	*this = XMMatrixInverse(NULL, *(XMMATRIX*)this);
}

inline void Mat44V::LookAt(Vec3V_In mPos, Vec3V_In vWorldUp)
{
	Vec3V vecToPos = mPos - wAxis.GetXYZ();
	vecToPos.Normalize();

	// zAxis
	zAxis.SetXYZ(vecToPos);

	// xAxis;
	xAxis.SetXYZ(Cross(vWorldUp, zAxis.GetXYZ()));
	xAxis.Normalize();

	// yAxis;
	yAxis.SetXYZ(Cross(zAxis.GetXYZ(), xAxis.GetXYZ()));
	yAxis.Normalize();
}

inline void Mat44V::TurnTo(Vec3V_In vPos, const float& fDeltaTime, float fTurnModifier)
{
	Vec3V vecToPos = vPos - wAxis.GetXYZ();
	vecToPos.Normalize();

	float protection = Dot(vecToPos, zAxis.GetXYZ());

	if (protection + 1 <= protection || protection != protection)
	{
		// You are already facing that exact direction
		return;
	}

	float fRotation = Dot(vecToPos, xAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
	{
		// protection to keep the matrix from turning slowly
		// if the position is behind the matrix
		if(Dot(vecToPos, zAxis.GetXYZ()) < ScalarV(INIT_ZERO))
			fRotation = (fRotation < 0.0f) ? -1.0f : 1.0f;

		Rotate_LocalY(fRotation * fTurnModifier * fDeltaTime);
	}

	fRotation = Dot(vecToPos, yAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
		Rotate_LocalX(-fRotation * fTurnModifier * fDeltaTime);

	xAxis.SetXYZ(::Normalize(Cross(g_WorldUp3V, zAxis.GetXYZ())));
	yAxis.SetXYZ(::Normalize(Cross(zAxis.GetXYZ(), xAxis.GetXYZ())));
}

inline void Mat44V::OrthoNormalInvert()
{
	Mat44V tmp(*this);

	Transpose3x3();

	wAxis.x = -Dot(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	wAxis.y = -Dot(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	wAxis.z = -Dot(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());
}