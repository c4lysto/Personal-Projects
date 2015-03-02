//#ifndef Mat34fF_INL
//#define Mat34fF_INL

__forceinline Mat34f::Mat34f(const float& fXx, const float& fXy, const float& fXz,
							 const float& fYx, const float& fYy, const float& fYz,
							 const float& fZx, const float& fZy, const float& fZz,
							 const float& fWx, const float& fWy, const float& fWz) : 
	  xAxis(fXx, fXy, fXz),
	  yAxis(fYx, fYy, fYz),
	  zAxis(fZx, fZy, fZz),
	  wAxis(fWx, fWy, fWz)
{
}

__forceinline Mat34f::Mat34f(Mat34f_In mMatrix) : 
	xAxis(mMatrix.xAxis),
	yAxis(mMatrix.yAxis),
	zAxis(mMatrix.zAxis),
	wAxis(mMatrix.wAxis)
{
}

__forceinline Mat34f::Mat34f(Mat34f&& mMatrix) :
	xAxis(move(mMatrix.xAxis)),
	yAxis(move(mMatrix.yAxis)),
	zAxis(move(mMatrix.zAxis)),
	wAxis(move(mMatrix.wAxis))
{
}

__forceinline Mat34f::Mat34f(eIdentityInitializer eIdentity) :
	xAxis(g_IdentityX3),
	yAxis(g_IdentityY3),
	zAxis(g_IdentityZ3),
	wAxis(g_IdentityW3)
{
}

inline Mat34f::Mat34f(eXRotationInitializer eXRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = g_IdentityX4;
	yAxis = Vec3f(0.0f, fCosAngle, fSinAngle);
	zAxis = Vec3f(0.0f, -fSinAngle, fCosAngle);
	wAxis = g_IdentityW4;
}

inline Mat34f::Mat34f(eYRotationInitializer eYRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec3f(fCosAngle, 0.0f, -fSinAngle);
	yAxis = g_IdentityY3;
	zAxis = Vec3f(fSinAngle, 0.0f, fCosAngle);
	wAxis = g_IdentityW3;
}

inline Mat34f::Mat34f(eZRotationInitializer eZRotation, const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec3f(fCosAngle, fSinAngle, 0.0f);
	yAxis = Vec3f(-fSinAngle, fCosAngle, 0.0f);
	zAxis = g_IdentityZ3;
	wAxis = g_IdentityW3;
}

inline Mat34f::Mat34f(XMMATRIX&& mMatrix)
{
//#if SSE_AVAILABLE
//	DirectX::store
//
//	VectorStoreU(mMatrix.r[0], xAxis.GetVector());
//	VectorStoreU(mMatrix.r[1], yAxis.GetVector());
//	VectorStoreU(mMatrix.r[2], zAxis.GetVector());
//	VectorStoreU(mMatrix.r[3], wAxis.GetVector());
//#else
//	xAxis.x = mMatrix._11;
//	xAxis.y = mMatrix._12;
//	xAxis.z = mMatrix._13;
//	xAxis.w = mMatrix._14;
//
//	yAxis.x = mMatrix._21;
//	yAxis.y = mMatrix._22;
//	yAxis.z = mMatrix._23;
//	yAxis.w = mMatrix._24;
//
//	zAxis.x = mMatrix._31;
//	zAxis.y = mMatrix._32;
//	zAxis.z = mMatrix._33;
//	zAxis.w = mMatrix._34;
//
//	wAxis.x = mMatrix._41;
//	wAxis.y = mMatrix._42;
//	wAxis.z = mMatrix._43;
//	wAxis.w = mMatrix._44;
//#endif // SSE_AVAILABLE
}

__forceinline Mat34f::Mat34f(Vec3f_In vXAxis,
						     Vec3f_In vYAxis,
						     Vec3f_In vZAxis,
						     Vec3f_In vWAxis) :
	xAxis(vXAxis),
	yAxis(vYAxis),
	zAxis(vZAxis),
	wAxis(vWAxis)
{
}

__forceinline Mat34f_Ref Mat34f::operator=(Mat34f_In mMatrix)
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

__forceinline Mat34f_Ref Mat34f::operator=(Mat34f&& mMatrix)
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

inline Vec4f_Out operator*(Vec4f_In vVector, Mat34f_In mMatrix)
{
	const float& fX = vVector.GetXConstRef();
	const float& fY = vVector.GetYConstRef();
	const float& fZ = vVector.GetZConstRef();
	const float& fW = vVector.GetWConstRef();

#if SSE_AVAILABLE
	Vector tmp1, tmp2;

	Vec3f_ConstRef vXAxis = mMatrix.xAxis;
	Vec3f_ConstRef vYAxis = mMatrix.yAxis;
	Vec3f_ConstRef vZAxis = mMatrix.zAxis;
	Vec3f_ConstRef vWAxis = mMatrix.wAxis;

	// get the top row
	tmp1 = VectorSet(fX);
	tmp2 = VectorMultiply(VectorSet(vXAxis.GetXConstRef(), vXAxis.GetYConstRef(), vXAxis.GetZConstRef(), 0.0f), tmp1);
	tmp1 = VectorSet(fY);
	tmp2 = VectorAdd(VectorMultiply(VectorSet(vYAxis.GetXConstRef(), vYAxis.GetYConstRef(), vYAxis.GetZConstRef(), 0.0f), tmp1), tmp2);
	tmp1 = VectorSet(fZ);
	tmp2 = VectorAdd(VectorMultiply(VectorSet(vZAxis.GetXConstRef(), vZAxis.GetYConstRef(), vZAxis.GetZConstRef(), 0.0f), tmp1), tmp2);
	tmp1 = VectorSet(fW);
	tmp2 = VectorAdd(VectorMultiply(VectorSet(vWAxis.GetXConstRef(), vWAxis.GetYConstRef(), vWAxis.GetZConstRef(), 0.0f), tmp1), tmp2);

	return Vec3f(tmp2);
#else
	return Vec3f(fX * mMatrix.xAxis.x + fY * mMatrix.yAxis.x + fZ * mMatrix.zAxis.x + fW * mMatrix.wAxis.x,
				 fX * mMatrix.xAxis.y + fY * mMatrix.yAxis.y + fZ * mMatrix.zAxis.y + fW * mMatrix.wAxis.y,
				 fX * mMatrix.xAxis.z + fY * mMatrix.yAxis.z + fZ * mMatrix.zAxis.z + fW * mMatrix.wAxis.z);
#endif
}

__forceinline Vec4f_Ref operator*=(Vec4f_Ref vVector, Mat34f_In mMatrix)
{
	return vVector.SetXYZ(vVector * mMatrix);
}

__forceinline Mat34f_Out Mat34f::operator+(Mat34f_In rhs) const
{
	return Mat34f(xAxis + rhs.GetXAxisConstRef(), yAxis + rhs.GetYAxisConstRef(),  zAxis + rhs.GetZAxisConstRef(), wAxis + rhs.GetWAxisConstRef());
}

__forceinline void Mat34f::operator+=(Mat34f_In rhs)
{
	xAxis += rhs.GetXAxisConstRef(); yAxis += rhs.GetYAxisConstRef(); zAxis += rhs.GetZAxisConstRef(); wAxis += rhs.GetWAxisConstRef();
}

__forceinline void Mat34f::MakeIdentity3x3()
{
	xAxis = g_IdentityX3;
	yAxis = g_IdentityY3;
	zAxis = g_IdentityZ3;
}

__forceinline Mat34f_Out Mat34f::operator-(Mat34f_In rhs) const
{
	return Mat34f(xAxis - rhs.GetXAxisConstRef(), yAxis - rhs.GetYAxisConstRef(),  zAxis - rhs.GetZAxisConstRef(), wAxis - rhs.GetWAxisConstRef());
}

__forceinline void Mat34f::operator-=(Mat34f_In rhs)
{
	xAxis -= rhs.GetXAxisConstRef(); yAxis -= rhs.GetYAxisConstRef(); zAxis -= rhs.GetZAxisConstRef(); wAxis -= rhs.GetWAxisConstRef();
}

// actually faster than DirectX Version :)
__forceinline void Mat34f::Rotate_GlobalX(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_X, fRadians);
	orientation *= tmp;
}

__forceinline void Mat34f::Rotate_GlobalY(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_Y, fRadians);
	orientation *= tmp;
}

__forceinline void Mat34f::Rotate_GlobalZ(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_Z, fRadians);
	orientation *= tmp;
}

__forceinline void Mat34f::Rotate_LocalX(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_X, fRadians);
	orientation = tmp * orientation;
}

__forceinline void Mat34f::Rotate_LocalY(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_Y, fRadians);
	orientation = tmp * orientation;
}

__forceinline void Mat34f::Rotate_LocalZ(const float& fRadians)
{
	Mat33f tmp(INIT_ROTATION_Z, fRadians);
	orientation = tmp * orientation;
}

__forceinline void Mat34f::Scale(const Vec3f& vScale)
{
	xAxis = xAxis * vScale.GetX();
	yAxis = yAxis * vScale.GetY();
	zAxis = zAxis * vScale.GetZ();
}

__forceinline void Mat34f::SetScale(const Vec3f& vScale)
{
	xAxis = Normalize(xAxis) * vScale.GetX();
	yAxis = Normalize(yAxis) * vScale.GetY();
	zAxis = Normalize(zAxis) * vScale.GetZ();
}

__forceinline Vec3f Mat34f::GetScale() const
{
	Vec3f retVal(Mag(xAxis), Mag(yAxis), Mag(zAxis));
	return retVal;
}

__forceinline void Mat34f::Translate(Vec3f vTranslation)
{
	wAxis = wAxis + vTranslation;
}

inline void Mat34f::Transpose()
{
#if SSE_AVAILABLE
	Vector row1 = VectorLoadU(xAxis.GetVector());
	Vector row2 = VectorLoadU(yAxis.GetVector());
	Vector row3 = VectorLoadU(zAxis.GetVector());
	Vector row4 = VectorLoadU(wAxis.GetVector());

	Vector tmp1 = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::X2, VecElem::Y2>(row1, row2);
	Vector tmp2 = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::X2, VecElem::Y2>(row3, row4);
	Vector tmp3 = VectorPermute<VecElem::Z1, VecElem::W1, VecElem::Z2, VecElem::W2>(row1, row2);
	Vector tmp4 = VectorPermute<VecElem::Z1, VecElem::W1, VecElem::Z2, VecElem::W2>(row3, row4);

	VectorStoreU(VectorPermute<VecElem::X1, VecElem::Z1, VecElem::X2, VecElem::Z2>(tmp1, tmp2), xAxis.GetVector());
	VectorStoreU(VectorPermute<VecElem::Y1, VecElem::W1, VecElem::Y2, VecElem::W2>(tmp1, tmp2), yAxis.GetVector());
	VectorStoreU(VectorPermute<VecElem::X1, VecElem::Z1, VecElem::X2, VecElem::Z2>(tmp3, tmp4), zAxis.GetVector());
	VectorStoreU(VectorPermute<VecElem::Y1, VecElem::W1, VecElem::Y2, VecElem::W2>(tmp3, tmp4), wAxis.GetVector());
#else
	swap(xAxis.y, yAxis.x);
	swap(xAxis.z, zAxis.x);
	swap(yAxis.z, zAxis.y);
	swap(wAxis.x, xAxis.w);
	swap(wAxis.y, yAxis.w);
	swap(wAxis.z, zAxis.w);
#endif
}

__forceinline void Mat34f::Transpose3x3()
{
	std::swap(xAxis.y, yAxis.x);
	std::swap(xAxis.z, zAxis.x);
	std::swap(yAxis.z, zAxis.y);
}

__forceinline void Mat34f::Normalize()
{
	xAxis = Normalize(xAxis);
	yAxis = Normalize(yAxis);
	zAxis = Normalize(zAxis);
}

__forceinline void Mat34f::Invert()
{
#if 1
	*this = DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x3((DirectX::XMFLOAT4X3*)this));
#else
	float determinant = MatrixDeterminant(*this);

	if(determinant == 0)
		return;

	float invDet = 1.0f / determinant;
#endif;
}

__forceinline void Mat34f::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	zAxis = Normalize(mPos - wAxis);
	xAxis = Normalize(Cross(vWorldUp, zAxis));
	yAxis = Normalize(Cross(zAxis, xAxis));
}

inline void Mat34f::TurnTo(Vec3f_In vPos, const float& fDeltaTime, float fTurnModifier)
{
	Vec3f vecToPos = Normalize(vPos - wAxis);

	float protection = Dot(vecToPos, zAxis);

	if (protection != protection)
	{
		// You are already facing that exact direction
		return;
	}

	float fRotation = Dot(vecToPos, xAxis);

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
	{
		// protection to keep the matrix from turning slowly
		// if the position is behind the matrix
		if(Dot(vecToPos, zAxis) < 0.0f)
			fRotation = (fRotation < 0.0f) ? -1.0f : 1.0f;

		Rotate_LocalY(fRotation * fTurnModifier * fDeltaTime);
	}

	fRotation = Dot(vecToPos, yAxis);

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
		Rotate_LocalX(-fRotation * fTurnModifier * fDeltaTime);

	xAxis = Normalize(Cross(g_WorldUp, zAxis));
	yAxis = Normalize(Cross(zAxis, xAxis));
}

__forceinline void Mat34f::OrthoNormalInvert()
{
	Mat34f tmp(*this);

	Transpose3x3();

	wAxis.x = -Dot(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	wAxis.y = -Dot(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	wAxis.z = -Dot(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());
}

//#endif //Mat34fF_INL