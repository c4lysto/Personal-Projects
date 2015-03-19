
__forceinline Mat33f::Mat33f(const float& fXx, const float& fXy, const float& fXz,
							 const float& fYx, const float& fYy, const float& fYz,
							 const float& fZx, const float& fZy, const float& fZz) :
	xAxis(fXx, fXy, fXz),
	yAxis(fYx, fYy, fYz),
	zAxis(fZx, fZy, fZz)
{
}

__forceinline Mat33f::Mat33f(Mat33f_In mMatrix) : 
	xAxis(mMatrix.xAxis),
	yAxis(mMatrix.yAxis),
	zAxis(mMatrix.zAxis)
{
}

__forceinline Mat33f::Mat33f(Mat33f&& mMatrix) :
	xAxis(move(mMatrix.xAxis)),
	yAxis(move(mMatrix.yAxis)),
	zAxis(move(mMatrix.zAxis))
{
}

__forceinline Mat33f::Mat33f(eIdentityInitializer UNUSED_PARAM(eIdentity)) :
	xAxis(g_IdentityX3),
	yAxis(g_IdentityY3),
	zAxis(g_IdentityZ3)
{
}

inline Mat33f::Mat33f(eXRotationInitializer UNUSED_PARAM(eXRotation), const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = g_IdentityX3;
	yAxis = Vec3f(0.0f, fCosAngle, fSinAngle);
	zAxis = Vec3f(0.0f, -fSinAngle, fCosAngle);
}

inline Mat33f::Mat33f(eYRotationInitializer UNUSED_PARAM(eYRotation), const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec3f(fCosAngle, 0.0f, -fSinAngle);
	yAxis = g_IdentityY3;
	zAxis = Vec3f(fSinAngle, 0.0f, fCosAngle);
}

inline Mat33f::Mat33f(eZRotationInitializer UNUSED_PARAM(eZRotation), const float& fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec3f(fCosAngle, fSinAngle, 0.0f);
	yAxis = Vec3f(-fSinAngle, fCosAngle, 0.0f);
	zAxis = g_IdentityZ3;
}

inline Mat33f_Ref Mat33f::operator=(Mat33f_In mMatrix)
{
	if(this != &mMatrix)
	{
		xAxis = mMatrix.GetXAxisRef();
		yAxis = mMatrix.GetYAxisRef();
		zAxis = mMatrix.GetZAxisRef();
	}
	return *this;
}

inline Mat33f_Ref Mat33f::operator=(Mat33f&& mMatrix)
{
	if(this != &mMatrix)
	{
		xAxis = move(mMatrix.GetXAxisRef());
		yAxis = move(mMatrix.GetYAxisRef());
		zAxis = move(mMatrix.GetZAxisRef());
	}
	return *this;
}

__forceinline Mat33f_Out Mat33f::operator*(Mat33f_In mMatrix) const
{
	Mat33f result(*this);
	result *= mMatrix;
	return result;
}

inline void Mat33f::operator*=(Mat33f_In mMatrix)
{
#if SSE_AVAILABLE
	const Vector otherX = VectorSet(mMatrix.xAxis.GetXRef(), mMatrix.xAxis.GetYRef(), mMatrix.xAxis.GetZRef(), 0.0f);
	const Vector otherY = VectorSet(mMatrix.yAxis.GetXRef(), mMatrix.yAxis.GetYRef(), mMatrix.yAxis.GetZRef(), 0.0f);
	const Vector otherZ = VectorSet(mMatrix.zAxis.GetXRef(), mMatrix.zAxis.GetYRef(), mMatrix.zAxis.GetZRef(), 0.0f);

	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(xAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(xAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);

	xAxis = Vec3f(tmp2);

	// get 2nd row
	tmp1 = VectorSet(yAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(yAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);

	yAxis = Vec3f(tmp2);

	// get 3rd row
	tmp1 = VectorSet(zAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(zAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);

	zAxis = Vec3f(tmp2);
#else
	Vec3f_ConstRef otherX = mMatrix.xAxis;
	Vec3f_ConstRef otherY = mMatrix.yAxis;
	Vec3f_ConstRef otherZ = mMatrix.zAxis;

	Xx = xAxis.x * otherX.x + xAxis.y * mMatrix.Yx + xAxis.z * mMatrix.Zx;
	Xy = xAxis.x * otherX.y + xAxis.y * mMatrix.Yy + xAxis.z * mMatrix.Zy;
	Xz = xAxis.x * otherX.z + xAxis.y * mMatrix.Yz + xAxis.z * mMatrix.Zz;

	Yx = yAxis.x * otherX.x + yAxis.y * mMatrix.Yx + yAxis.z * mMatrix.Zx;
	Yy = yAxis.x * otherX.y + yAxis.y * mMatrix.Yy + yAxis.z * mMatrix.Zy;
	Yz = yAxis.x * otherX.z + yAxis.y * mMatrix.Yz + yAxis.z * mMatrix.Zz;

	Zx = zAxis.x * otherX.x + zAxis.y * mMatrix.Yx + zAxis.z * mMatrix.Zx;
	Zy = zAxis.x * otherX.y + zAxis.y * mMatrix.Yy + zAxis.z * mMatrix.Zy;
	Zz = zAxis.x * otherX.z + zAxis.y * mMatrix.Yz + zAxis.z * mMatrix.Zz;
#endif
}

inline Vec3f_Out operator*(Vec3f_ConstRef vVector, Mat33f_In mMatrix)
{
#if SSE_AVAILABLE
	const Vector otherX = VectorSet(mMatrix.xAxis.GetXRef(), mMatrix.xAxis.GetYRef(), mMatrix.xAxis.GetZRef(), 0.0f);
	const Vector otherY = VectorSet(mMatrix.yAxis.GetXRef(), mMatrix.yAxis.GetYRef(), mMatrix.yAxis.GetZRef(), 0.0f);
	const Vector otherZ = VectorSet(mMatrix.zAxis.GetXRef(), mMatrix.zAxis.GetYRef(), mMatrix.zAxis.GetZRef(), 0.0f);

	Vector tmp1, tmp2;
	tmp1 = VectorSet(vVector.GetXRef());
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(vVector.GetYRef());
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(vVector.GetZRef());
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);

	return Vec3f(tmp2);
#else
	Vec3f_ConstRef otherX = mMatrix.xAxis;
	Vec3f_ConstRef otherY = mMatrix.yAxis;
	Vec3f_ConstRef otherZ = mMatrix.zAxis;

	return Vec3f(	vVector.x * otherX.GetXRef() + vVector.y * otherY.GetXRef() + vVector.z * otherZ.GetXRef(),
					vVector.x * otherX.GetYRef() + vVector.y * otherY.GetYRef() + vVector.z * otherZ.GetYRef(),
					vVector.x * otherX.GetZRef() + vVector.y * otherY.GetZRef() + vVector.z * otherZ.GetZRef());
#endif
}

Vec3f_Ref operator*=(Vec3f_Ref vVector, Mat33f_In mMatrix)
{
	return vVector = vVector * mMatrix;
}

__forceinline Mat33f_Out Mat33f::operator+(Mat33f_In rhs) const
{
	return Mat33f(xAxis + rhs.xAxis, yAxis + rhs.yAxis, zAxis + rhs.zAxis);
}

__forceinline void Mat33f::operator+=(Mat33f_In rhs)
{
	xAxis += rhs.xAxis; yAxis += rhs.yAxis; zAxis += rhs.zAxis;
}

__forceinline Mat33f_Out Mat33f::operator-(Mat33f_In rhs) const
{
	return Mat33f(xAxis - rhs.xAxis, yAxis - rhs.yAxis, zAxis - rhs.zAxis);
}

__forceinline void Mat33f::operator-=(Mat33f_In rhs)
{
	xAxis -= rhs.xAxis; yAxis -= rhs.yAxis; zAxis -= rhs.zAxis;
}

inline void Mat33f::Rotate_LocalX(const float& fRadians)
{
	Mat33f tmp(I_ROTATION_X, fRadians);
	*this = tmp * (*this);
}

inline void Mat33f::Rotate_LocalY(const float& fRadians)
{
	Mat33f tmp(I_ROTATION_Y, fRadians);
	*this = tmp * (*this);
}

inline void Mat33f::Rotate_LocalZ(const float& fRadians)
{
	Mat33f tmp(I_ROTATION_Z, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat33f::Scale(Vec3f_In vScale)
{
	xAxis *= vScale.GetXRef();
	yAxis *= vScale.GetYRef();
	zAxis *= vScale.GetZRef();
}

__forceinline void Mat33f::SetScale(Vec3f_In vScale)
{
	xAxis = ::Normalize(xAxis) * vScale.GetXRef();
	yAxis = ::Normalize(yAxis) * vScale.GetYRef();
	zAxis = ::Normalize(zAxis) * vScale.GetZRef();
}

__forceinline Vec3f_Out Mat33f::GetScale() const
{
	return Vec3f(Mag(xAxis), Mag(yAxis), Mag(zAxis));
}

__forceinline void Mat33f::Transpose()
{
	std::swap(Yx, Xy);
	std::swap(Zx, Xz);
	std::swap(Zy, Yz);
}

__forceinline void Mat33f::Normalize()
{
	xAxis = ::Normalize(xAxis);
	yAxis = ::Normalize(yAxis);
	zAxis = ::Normalize(zAxis);
}