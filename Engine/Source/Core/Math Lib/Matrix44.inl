#ifndef Mat44_INL
#define Mat44_INL

typedef Mat44& Mat44_Ref;
typedef const Mat44& Mat44_ConstRef;

typedef Mat44_ConstRef Mat44_In;
typedef Mat44_Ref Mat44_InOut;

typedef Mat44 Mat44_Out;

// Other Vec3f Aliases
typedef Mat44 float4x4;

class Mat44
{
#define Mat44_ACCESSOR(retType, name, retVal) \
	__forceinline retType name() { return retVal; }

#define Mat44_ACCESSOR_CONST(retType, name, retVal) \
	__forceinline retType name() const { return retVal; }

#define Mat44_MUTATOR(name, inType, modifiedVal) \
	__forceinline void name(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		float m[16];
		float ma[4][4];

		struct
		{
			Vec4f xAxis, yAxis, zAxis, wAxis;
		};
	};

public:
	Mat44(){}
	explicit Mat44(float fXx, float fXy, float fXz, float fXw,
				   float fYx, float fYy, float fYz, float fYw,
				   float fZx, float fZy, float fZz, float fZw,
				   float fWx, float fWy, float fWz, float fWw);
	Mat44(Mat44_In mMatrix);
	Mat44(Mat44&& mMatrix);
	Mat44(XMMATRIX&& mMatrix);
	explicit Mat44(Vec4f_In vXAxis,
				   Vec4f_In vYAxis,
				   Vec4f_In vZAxis,
				   Vec4f_In vWAxis);

	explicit Mat44(eIdentityInitializer eIdentity);
	explicit Mat44(eXRotationInitializer eXRotation, float fRotationInRadians);
	explicit Mat44(eYRotationInitializer eYRotation, float fRotationInRadians);
	explicit Mat44(eZRotationInitializer eZRotation, float fRotationInRadians);

#if defined(Mat44_ACCESSOR) && defined(Mat44_ACCESSOR_CONST)
	Mat44_ACCESSOR_CONST(Vec4f, GetXAxis, xAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetYAxis, yAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetZAxis, zAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetWAxis, wAxis)

	Mat44_ACCESSOR(Vec4f_Ref, GetXAxisRef, xAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetYAxisRef, yAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetZAxisRef, zAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetWAxisRef, wAxis)
#undef Mat44_ACCESSOR
#undef Mat44_ACCESSOR_CONST
#else
#error Mat44 ACCESSORS NOT DEFINED
#endif

#ifdef Mat44_MUTATOR
	Mat44_MUTATOR(SetXAxis, Vec4f, xAxis)
	Mat44_MUTATOR(SetYAxis, Vec4f, yAxis)
	Mat44_MUTATOR(SetZAxis, Vec4f, zAxis)
	Mat44_MUTATOR(SetWAxis, Vec4f, wAxis)
#undef Mat44_MUTATOR
#else
#error Mat44 MUTATORS NOT DEFINED!
#endif


	void MakeIdentity3x3();

	Mat44_ConstRef operator=(Mat44_In mMatrix);
	Mat44_ConstRef operator=(Mat44&& mMatrix);

	Mat44 operator*(Mat44_In mMatrix) const;
	void operator*=(Mat44_In mMatrix);
	friend Vec4f operator*(Vec4f_In vVector, Mat44_In mMatrix);
	friend Vec4f_ConstRef operator*=(Vec4f_Ref vVector, Mat44_In mMatrix);

	// actually faster than DirectX Version :)
	void Rotate_GlobalX(float fRadians);
	void Rotate_GlobalY(float fRadians);
	void Rotate_GlobalZ(float fRadians);

	void Rotate_LocalX(float fRadians);
	void Rotate_LocalY(float fRadians);
	void Rotate_LocalZ(float fRadians);

	void Scale(const Vec3f& vScale);

	void SetScale(const Vec3f& vScale);
	Vec3f GetScale() const;

	void Translate(Vec3f vTranslation);

	void Transpose();
	void Transpose3x3();

	void NormalizeXYZ();

	friend float MatrixDeterminant(Mat44_In mMatrix);

	void Invert();

	void LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	void TurnTo(Vec3f_In vPos, float fDeltaTime, float fTurnModifier = 1.0f);

	void OrthoNormalInvert();
};


__forceinline Mat44::Mat44(float fXx, float fXy, float fXz, float fXw,
						   float fYx, float fYy, float fYz, float fYw,
						   float fZx, float fZy, float fZz, float fZw,
						   float fWx, float fWy, float fWz, float fWw)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorSet(fXx, fXy, fXz, fXw), xAxis.vector);
	VectorStoreU(VectorSet(fYx, fYy, fYz, fYw), yAxis.vector);
	VectorStoreU(VectorSet(fZx, fZy, fZz, fZw), zAxis.vector);
	VectorStoreU(VectorSet(fWx, fWy, fWz, fWw), wAxis.vector);
#else
	xAxis = Vec4f(fXx, fXy, fXz, fXw);
	yAxis = Vec4f(fYx, fYy, fYz, fYw);
	zAxis = Vec4f(fZx, fZy, fZz, fZw);
	wAxis = Vec4f(fWx, fWy, fWz, fWw);
#endif
}

__forceinline Mat44::Mat44(Mat44_In mMatrix)
{
	xAxis = mMatrix.xAxis;
	yAxis = mMatrix.yAxis;
	zAxis = mMatrix.zAxis;
	wAxis = mMatrix.wAxis;
}

__forceinline Mat44::Mat44(Mat44&& mMatrix)
{
	xAxis = move(mMatrix.xAxis);
	yAxis = move(mMatrix.yAxis);
	zAxis = move(mMatrix.zAxis);
	wAxis = move(mMatrix.wAxis);
}

__forceinline Mat44::Mat44(eIdentityInitializer eIdentity)
{
	xAxis = g_IdentityX4;
	yAxis = g_IdentityY4;
	zAxis = g_IdentityZ4;
	wAxis = g_IdentityW4;
}

inline Mat44::Mat44(eXRotationInitializer eXRotation, float fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = g_IdentityX4;
	yAxis = Vec4f(0.0f, fCosAngle, fSinAngle, 0.0f);
	zAxis = Vec4f(0.0f, -fSinAngle, fCosAngle, 0.0f);
	wAxis = g_IdentityW4;
}

inline Mat44::Mat44(eYRotationInitializer eYRotation, float fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec4f(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	yAxis = g_IdentityY4;
	zAxis = Vec4f(fSinAngle, 0.0f, fCosAngle, 0.0f);
	wAxis = g_IdentityW4;
}

inline Mat44::Mat44(eZRotationInitializer eZRotation, float fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec4f(fCosAngle, fSinAngle, 0.0f, 0.0f);
	yAxis = Vec4f(-fSinAngle, fCosAngle, 0.0f, 0.0f);
	zAxis = g_IdentityZ4;
	wAxis = g_IdentityW4;
}

inline Mat44::Mat44(XMMATRIX&& mMatrix)
{
#if SSE_AVAILABLE
	VectorStoreU(mMatrix.r[0], xAxis.vector);
	VectorStoreU(mMatrix.r[1], yAxis.vector);
	VectorStoreU(mMatrix.r[2], zAxis.vector);
	VectorStoreU(mMatrix.r[3], wAxis.vector);
#else
	xAxis.x = mMatrix._11;
	xAxis.y = mMatrix._12;
	xAxis.z = mMatrix._13;
	xAxis.w = mMatrix._14;

	yAxis.x = mMatrix._21;
	yAxis.y = mMatrix._22;
	yAxis.z = mMatrix._23;
	yAxis.w = mMatrix._24;

	zAxis.x = mMatrix._31;
	zAxis.y = mMatrix._32;
	zAxis.z = mMatrix._33;
	zAxis.w = mMatrix._34;

	wAxis.x = mMatrix._41;
	wAxis.y = mMatrix._42;
	wAxis.z = mMatrix._43;
	wAxis.w = mMatrix._44;
#endif // SSE_AVAILABLE
}

__forceinline Mat44::Mat44(Vec4f_In vXAxis,
						   Vec4f_In vYAxis,
						   Vec4f_In vZAxis,
						   Vec4f_In vWAxis)
{
	xAxis = vXAxis;
	yAxis = vYAxis;
	zAxis = vZAxis;
	wAxis = vWAxis;
}

__forceinline Mat44_ConstRef Mat44::operator=(Mat44_In mMatrix)
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

__forceinline Mat44_ConstRef Mat44::operator=(Mat44&& mMatrix)
{
	xAxis = move(mMatrix.xAxis);
	yAxis = move(mMatrix.yAxis);
	zAxis = move(mMatrix.zAxis);
	wAxis = move(mMatrix.wAxis);
	return *this;
}

inline Mat44 Mat44::operator*(Mat44_In mMatrix) const
{
	Mat44 result;

#if SSE_AVAILABLE
	const Vector otherX = VectorLoadU(xAxis.vector);
	const Vector otherY = VectorLoadU(yAxis.vector);
	const Vector otherZ = VectorLoadU(zAxis.vector);
	const Vector otherW = VectorLoadU(wAxis.vector);

	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(xAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(xAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, result.xAxis.vector);

	// get 2nd row
	tmp1 = VectorSet(yAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(yAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, result.yAxis.vector);

	// get 3rd row
	tmp1 = VectorSet(zAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(zAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, result.zAxis.vector);

	// get bottom row
	tmp1 = VectorSet(wAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(wAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, result.wAxis.vector);
#else
	const Vec4f& otherX = mMatrix.xAxis;
	const Vec4f& otherY = mMatrix.yAxis;
	const Vec4f& otherZ = mMatrix.zAxis;
	const Vec4f& otherW = mMatrix.wAxis;

	result.Xx = xAxis.x * otherX.x + xAxis.y * mMatrix.Yx + xAxis.z * mMatrix.Zx + xAxis.w * mMatrix.Wx;
	result.Xy = xAxis.x * otherX.y + xAxis.y * mMatrix.Yy + xAxis.z * mMatrix.Zy + xAxis.w * mMatrix.Wy;
	result.Xz = xAxis.x * otherX.z + xAxis.y * mMatrix.Yz + xAxis.z * mMatrix.Zz + xAxis.w * mMatrix.Wz;
	result.Xw = xAxis.x * otherX.w + xAxis.y * mMatrix.Yw + xAxis.z * mMatrix.Zw + xAxis.w * mMatrix.Ww;
	
	result.Yx = yAxis.x * otherX.x + yAxis.y * mMatrix.Yx + yAxis.z * mMatrix.Zx + yAxis.w * mMatrix.Wx;
	result.Yy = yAxis.x * otherX.y + yAxis.y * mMatrix.Yy + yAxis.z * mMatrix.Zy + yAxis.w * mMatrix.Wy;
	result.Yz = yAxis.x * otherX.z + yAxis.y * mMatrix.Yz + yAxis.z * mMatrix.Zz + yAxis.w * mMatrix.Wz;
	result.Yw = yAxis.x * otherX.w + yAxis.y * mMatrix.Yw + yAxis.z * mMatrix.Zw + yAxis.w * mMatrix.Ww;
	
	result.Zx = zAxis.x * otherX.x + zAxis.y * mMatrix.Yx + zAxis.z * mMatrix.Zx + zAxis.w * mMatrix.Wx;
	result.Zy = zAxis.x * otherX.y + zAxis.y * mMatrix.Yy + zAxis.z * mMatrix.Zy + zAxis.w * mMatrix.Wy;
	result.Zz = zAxis.x * otherX.z + zAxis.y * mMatrix.Yz + zAxis.z * mMatrix.Zz + zAxis.w * mMatrix.Wz;
	result.Zw = zAxis.x * otherX.w + zAxis.y * mMatrix.Yw + zAxis.z * mMatrix.Zw + zAxis.w * mMatrix.Ww;
	
	result.Wx = wAxis.x * otherX.x + wAxis.y * mMatrix.Yx + wAxis.z * mMatrix.Zx + wAxis.w * mMatrix.Wx;
	result.Wy = wAxis.x * otherX.y + wAxis.y * mMatrix.Yy + wAxis.z * mMatrix.Zy + wAxis.w * mMatrix.Wy;
	result.Wz = wAxis.x * otherX.z + wAxis.y * mMatrix.Yz + wAxis.z * mMatrix.Zz + wAxis.w * mMatrix.Wz;
	result.Ww = wAxis.x * otherX.w + wAxis.y * mMatrix.Yw + wAxis.z * mMatrix.Zw + wAxis.w * mMatrix.Ww;
#endif

	return result;
}

inline void Mat44::operator*=(Mat44_In mMatrix)
{
#if SSE_AVAILABLE
	const Vector otherX = VectorLoadU(xAxis.vector);
	const Vector otherY = VectorLoadU(yAxis.vector);
	const Vector otherZ = VectorLoadU(zAxis.vector);
	const Vector otherW = VectorLoadU(wAxis.vector);

	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(xAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(xAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(xAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, xAxis.vector);

	// get 2nd row
	tmp1 = VectorSet(yAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(yAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(yAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, yAxis.vector);

	// get 3rd row
	tmp1 = VectorSet(zAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(zAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(zAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, zAxis.vector);

	// get bottom row
	tmp1 = VectorSet(wAxis.x);
	tmp2 = VectorMultiply(otherX, tmp1);
	tmp1 = VectorSet(wAxis.y);
	tmp2 = VectorAdd(VectorMultiply(otherY, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.z);
	tmp2 = VectorAdd(VectorMultiply(otherZ, tmp1), tmp2);
	tmp1 = VectorSet(wAxis.w);
	tmp2 = VectorAdd(VectorMultiply(otherW, tmp1), tmp2);

	VectorStoreU(tmp2, wAxis.vector);
#else
	const Vec4f& otherX = mMatrix.xAxis;
	const Vec4f& otherY = mMatrix.yAxis;
	const Vec4f& otherZ = mMatrix.zAxis;
	const Vec4f& otherW = mMatrix.wAxis;

	Vec4f currAxis = xAxis;

	xAxis.x = currAxis.x * otherX.x + currAxis.y * otherY.x + currAxis.z * otherZ.x + currAxis.w * otherW.x;
	xAxis.y = currAxis.x * otherX.y + currAxis.y * otherY.y + currAxis.z * otherZ.y + currAxis.w * otherW.y;
	xAxis.z = currAxis.x * otherX.z + currAxis.y * otherY.z + currAxis.z * otherZ.z + currAxis.w * otherW.z;
	xAxis.w = currAxis.x * otherX.w + currAxis.y * otherY.w + currAxis.z * otherZ.w + currAxis.w * otherW.w;

	currAxis = yAxis;
	yAxis.x = currAxis.x * otherX.x + currAxis.y * otherY.x + currAxis.z * otherZ.x + currAxis.w * otherW.x;
	yAxis.y = currAxis.x * otherX.y + currAxis.y * otherY.y + currAxis.z * otherZ.y + currAxis.w * otherW.y;
	yAxis.z = currAxis.x * otherX.z + currAxis.y * otherY.z + currAxis.z * otherZ.z + currAxis.w * otherW.z;
	yAxis.w = currAxis.x * otherX.w + currAxis.y * otherY.w + currAxis.z * otherZ.w + currAxis.w * otherW.w;

	currAxis = zAxis;
	zAxis.x = currAxis.x * otherX.x + currAxis.y * otherY.x + currAxis.z * otherZ.x + currAxis.w * otherW.x;
	zAxis.y = currAxis.x * otherX.y + currAxis.y * otherY.y + currAxis.z * otherZ.y + currAxis.w * otherW.y;
	zAxis.z = currAxis.x * otherX.z + currAxis.y * otherY.z + currAxis.z * otherZ.z + currAxis.w * otherW.z;
	zAxis.w = currAxis.x * otherX.w + currAxis.y * otherY.w + currAxis.z * otherZ.w + currAxis.w * otherW.w;

	currAxis = wAxis;
	wAxis.x = currAxis.x * otherX.x + currAxis.y * otherY.x + currAxis.z * otherZ.x + currAxis.w * otherW.x;
	wAxis.y = currAxis.x * otherX.y + currAxis.y * otherY.y + currAxis.z * otherZ.y + currAxis.w * otherW.y;
	wAxis.z = currAxis.x * otherX.z + currAxis.y * otherY.z + currAxis.z * otherZ.z + currAxis.w * otherW.z;
	wAxis.w = currAxis.x * otherX.w + currAxis.y * otherY.w + currAxis.z * otherZ.w + currAxis.w * otherW.w;
#endif
}

inline Vec4f operator*(Vec4f_In vVector, Mat44_In mMatrix)
{
	float fX = vVector.GetX();
	float fY = vVector.GetY();
	float fZ = vVector.GetZ();
	float fW = vVector.GetW();

#if SSE_AVAILABLE
	Vector tmp1, tmp2;

	// get the top row
	tmp1 = VectorSet(fX);
	tmp2 = VectorMultiply(VectorLoadU((float*)&mMatrix.xAxis), tmp1);
	tmp1 = VectorSet(fY);
	tmp2 = VectorAdd(VectorMultiply(VectorLoadU((float*)&mMatrix.yAxis), tmp1), tmp2);
	tmp1 = VectorSet(fZ);
	tmp2 = VectorAdd(VectorMultiply(VectorLoadU((float*)&mMatrix.zAxis), tmp1), tmp2);
	tmp1 = VectorSet(fW);
	tmp2 = VectorAdd(VectorMultiply(VectorLoadU((float*)&mMatrix.wAxis), tmp1), tmp2);

	return Vec4f(tmp2);
#else
	return Vec4f(fX * mMatrix.xAxis.x + fY * mMatrix.yAxis.x + fZ * mMatrix.zAxis.x + fW * mMatrix.wAxis.x,
				 fX * mMatrix.xAxis.y + fY * mMatrix.yAxis.y + fZ * mMatrix.zAxis.y + fW * mMatrix.wAxis.y,
				 fX * mMatrix.xAxis.z + fY * mMatrix.yAxis.z + fZ * mMatrix.zAxis.z + fW * mMatrix.wAxis.z,
				 fX * mMatrix.xAxis.w + fY * mMatrix.yAxis.w + fZ * mMatrix.zAxis.w + fW * mMatrix.wAxis.w);
#endif
}

__forceinline Vec4f_ConstRef operator*=(Vec4f_Ref vVector, Mat44_In mMatrix)
{
	return vVector = vVector * mMatrix;
}

__forceinline void Mat44::MakeIdentity3x3()
{
	xAxis.SetXYZ(g_IdentityX3);
	yAxis.SetXYZ(g_IdentityY3);
	zAxis.SetXYZ(g_IdentityZ3);
}

// actually faster than DirectX Version :)
__forceinline void Mat44::Rotate_GlobalX(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_X, fRadians);
	*this *= tmp;
}

__forceinline void Mat44::Rotate_GlobalY(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_Y, fRadians);
	*this *= tmp;
}

__forceinline void Mat44::Rotate_GlobalZ(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_Z, fRadians);
	*this *= tmp;
}

__forceinline void Mat44::Rotate_LocalX(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_X, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44::Rotate_LocalY(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_Y, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44::Rotate_LocalZ(float fRadians)
{
	Mat44 tmp(INIT_ROTATION_Z, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44::Scale(const Vec3f& vScale)
{
	xAxis.SetXYZ(xAxis.GetXYZ() * vScale.GetX());
	yAxis.SetXYZ(yAxis.GetXYZ() * vScale.GetY());
	zAxis.SetXYZ(zAxis.GetXYZ() * vScale.GetZ());
}

__forceinline void Mat44::SetScale(const Vec3f& vScale)
{
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()) * vScale.GetX());
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()) * vScale.GetY());
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()) * vScale.GetZ());
}

__forceinline Vec3f Mat44::GetScale() const
{
	Vec3f retVal(Mag(xAxis.GetXYZ()), Mag(yAxis.GetXYZ()), Mag(zAxis.GetXYZ()));
	return retVal;
}

__forceinline void Mat44::Translate(Vec3f vTranslation)
{
	wAxis.SetXYZ(wAxis.GetXYZ() + vTranslation);
}

inline void Mat44::Transpose()
{
#if SSE_AVAILABLE
	Vector row1 = VectorLoadU(xAxis.vector);
	Vector row2 = VectorLoadU(yAxis.vector);
	Vector row3 = VectorLoadU(zAxis.vector);
	Vector row4 = VectorLoadU(wAxis.vector);

	Vector tmp1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 1, 0));
	Vector tmp2 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(1, 0, 1, 0));
	Vector tmp3 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(3, 2, 3, 2));
	Vector tmp4 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(3, 2, 3, 2));

	VectorStoreU(_mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0)), xAxis.vector);
	VectorStoreU(_mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1)), yAxis.vector);
	VectorStoreU(_mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0)), zAxis.vector);
	VectorStoreU(_mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1)), wAxis.vector);
#else
	swap(xAxis.y, yAxis.x);
	swap(xAxis.z, zAxis.x);
	swap(yAxis.z, zAxis.y);
	swap(wAxis.x, xAxis.w);
	swap(wAxis.y, yAxis.w);
	swap(wAxis.z, zAxis.w);
#endif
}

__forceinline void Mat44::Transpose3x3()
{
	std::swap(xAxis.y, yAxis.x);
	std::swap(xAxis.z, zAxis.x);
	std::swap(yAxis.z, zAxis.y);
}

__forceinline void Mat44::NormalizeXYZ()
{
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()));
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()));
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()));
}

float MatrixDeterminant(Mat44_In mMatrix)
{
	const Vec4f& row1 = mMatrix.xAxis;
	const Vec4f& row2 = mMatrix.yAxis;
	const Vec4f& row3 = mMatrix.zAxis;
	const Vec4f& row4 = mMatrix.wAxis;

	float det = row1.x*row2.y*row3.z*row4.w +
				row1.x*row2.z*row3.w*row4.y + 
				row1.x*row2.w*row3.y*row4.z + 

				row1.y*row2.x*row3.w*row4.z + 
				row1.y*row2.z*row3.x*row4.w + 
				row1.y*row2.w*row3.z*row4.x + 

				row1.z*row2.x*row3.y*row4.w + 
				row1.z*row2.y*row3.w*row4.x + 
				row1.z*row2.w*row3.x*row4.y + 

				row1.w*row2.x*row3.z*row4.y + 
				row1.w*row2.y*row3.x*row4.z + 
				row1.w*row2.z*row3.y*row4.x 

				-

				row1.x*row2.y*row3.w*row4.z - 
				row1.x*row2.z*row3.y*row4.w - 
				row1.x*row2.w*row3.z*row4.y - 

				row1.y*row2.x*row3.z*row4.w - 
				row1.y*row2.z*row3.w*row4.x - 
				row1.y*row2.w*row3.x*row4.z - 

				row1.z*row2.x*row3.w*row4.y - 
				row1.z*row2.y*row3.x*row4.w - 
				row1.z*row2.w*row3.y*row4.x - 

				row1.w*row2.x*row3.y*row4.z - 
				row1.w*row2.y*row3.z*row4.x - 
				row1.w*row2.z*row3.x*row4.y;

	return det;
}

__forceinline void Mat44::Invert()
{
#if 1
	*this = DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4((DirectX::XMFLOAT4X4*)this));
#else
	float determinant = MatrixDeterminant(*this);

	if(determinant == 0)
		return;

	float invDet = 1.0f / determinant;
#endif;
}

__forceinline void Mat44::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	zAxis.SetXYZ(Normalize(mPos - wAxis.GetXYZ()));
	xAxis.SetXYZ(Normalize(CrossProduct(vWorldUp, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));
}

inline void Mat44::TurnTo(Vec3f_In vPos, float fDeltaTime, float fTurnModifier)
{
	Vec3f vecToPos = Normalize(vPos - wAxis.GetXYZ());

	float protection = DotProduct(vecToPos, zAxis.GetXYZ());

	if (protection != protection)
	{
		// You are already facing that exact direction
		return;
	}

	float fRotation = DotProduct(vecToPos, xAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
	{
		// protection to keep the matrix from turning slowly
		// if the position is behind the matrix
		if(DotProduct(vecToPos, zAxis.GetXYZ()) < 0.0f)
			fRotation = (fRotation < 0.0f) ? -1.0f : 1.0f;

		Rotate_LocalY(fRotation * fTurnModifier * fDeltaTime);
	}

	fRotation = DotProduct(vecToPos, yAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
		Rotate_LocalX(-fRotation * fTurnModifier * fDeltaTime);

	xAxis.SetXYZ(Normalize(CrossProduct(g_WorldUp, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));
}

__forceinline void Mat44::OrthoNormalInvert()
{
	Mat44 tmp(*this);

	Transpose3x3();

	wAxis.x = -DotProduct(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	wAxis.y = -DotProduct(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	wAxis.z = -DotProduct(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());
}

#endif //MAT44_INL