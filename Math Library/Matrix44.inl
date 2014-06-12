#ifndef Mat44_INL
#define Mat44_INL

typedef Mat44& Mat44_Ref;
typedef const Mat44& Mat44_ConstRef;
typedef Mat44_ConstRef Mat44_In;

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
			float Xx, Xy, Xz, Xw,
				  Yx, Yy, Yz, Yw,
				  Zx, Zy, Zz, Zw,
				  Wx, Wy, Wz, Ww;
		};

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


	Mat44_Ref MakeIdentity3x3();

	Mat44_Ref operator=(Mat44_In mMatrix);
	Mat44_Ref operator=(Mat44&& mMatrix);

	Mat44 operator*(Mat44_In mMatrix) const;
	Mat44_Ref operator*=(Mat44_In mMatrix);
	friend Vec4f operator*(Vec4f_In vVector, Mat44_In mMatrix);
	friend Vec4f_Ref operator*=(Vec4f_Ref vVector, Mat44_In mMatrix);

	// actually faster than DirectX Version :)
	Mat44_Ref Rotate_GlobalX_Radians(float fRadians);
	Mat44_Ref Rotate_GlobalY_Radians(float fRadians);
	Mat44_Ref Rotate_GlobalZ_Radians(float fRadians);

	Mat44_Ref Rotate_GlobalX_Degrees(float fDegrees);
	Mat44_Ref Rotate_GlobalY_Degrees(float fDegrees);
	Mat44_Ref Rotate_GlobalZ_Degrees(float fDegrees);

	Mat44_Ref Rotate_LocalX_Radians(float fRadians);
	Mat44_Ref Rotate_LocalY_Radians(float fRadians);
	Mat44_Ref Rotate_LocalZ_Radians(float fRadians);

	Mat44_Ref Rotate_LocalX_Degrees(float fDegrees);
	Mat44_Ref Rotate_LocalY_Degrees(float fDegrees);
	Mat44_Ref Rotate_LocalZ_Degrees(float fDegrees);

	Mat44_Ref Scale(const Vec3f& vScale);

	Mat44_Ref SetScale(const Vec3f& vScale);
	Vec3f GetScale() const;

	Mat44_Ref Translate(Vec3f vTranslation);

	Mat44_Ref Transpose();
	Mat44_Ref Transpose3x3();

	Mat44_Ref NormalizeXYZ();

	Mat44_Ref Invert();

	Mat44_Ref LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	Mat44_Ref TurnTo(Vec3f_In vPos, float fDeltaTime, float fTurnModifier = 1.0f);

	Mat44_Ref OrthoNormalInvert();
};


__forceinline Mat44::Mat44(float fXx, float fXy, float fXz, float fXw,
						   float fYx, float fYy, float fYz, float fYw,
						   float fZx, float fZy, float fZz, float fZw,
						   float fWx, float fWy, float fWz, float fWw)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(fXx, fXy, fXz, fXw));
	_mm_storeu_ps(m + 4, _mm_setr_ps(fYx, fYy, fYz, fYw));
	_mm_storeu_ps(m + 8, _mm_setr_ps(fZx, fZy, fZz, fZw));
	_mm_storeu_ps(m + 12, _mm_setr_ps(fWx, fWy, fWz, fWw));
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

__forceinline Mat44::Mat44(XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
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

__forceinline Mat44_Ref Mat44::operator=(Mat44_In mMatrix)
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

__forceinline Mat44_Ref Mat44::operator=(Mat44&& mMatrix)
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

inline Mat44_Ref Mat44::operator*=(Mat44_In mMatrix)
{
#if SSE_AVAILABLE
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

inline Vec4f operator*(Vec4f_In vVector, Mat44_In mMatrix)
{
	float fX = vVector.GetX();
	float fY = vVector.GetY();
	float fZ = vVector.GetZ();
	float fW = vVector.GetW();

#if SSE_AVAILABLE
	__m128 tmp1, tmp2;

	// get the top row
	tmp1 = _mm_set1_ps(fX);
	tmp2 = _mm_mul_ps(_mm_loadu_ps(mMatrix.m), tmp1);
	tmp1 = _mm_set1_ps(fY);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 4), tmp1), tmp2);
	tmp1 = _mm_set1_ps(fZ);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 8), tmp1), tmp2);
	tmp1 = _mm_set1_ps(fW);
	tmp2 = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(mMatrix.m + 12), tmp1), tmp2);

	return Vec4f(tmp2);
#else
	return Vec4f(fX * mMatrix.Xx + fY * mMatrix.Yx + fZ * mMatrix.Zx + fW * mMatrix.Wx,
				 fX * mMatrix.Xy + fY * mMatrix.Yy + fZ * mMatrix.Zy + fW * mMatrix.Wy,
				 fX * mMatrix.Xz + fY * mMatrix.Yz + fZ * mMatrix.Zz + fW * mMatrix.Wz,
				 fX * mMatrix.Xw + fY * mMatrix.Yw + fZ * mMatrix.Zw + fW * mMatrix.Ww);
#endif
}

__forceinline Vec4f_Ref operator*=(Vec4f_Ref vVector, Mat44_In mMatrix)
{
	return vVector = vVector * mMatrix;
}

__forceinline Mat44_Ref Mat44::MakeIdentity3x3()
{
	xAxis.SetXYZ(g_IdentityX3);
	yAxis.SetXYZ(g_IdentityY3);
	zAxis.SetXYZ(g_IdentityZ3);

	return *this;
}

// actually faster than DirectX Version :)
__forceinline Mat44_Ref Mat44::Rotate_GlobalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Yy = fCosAngle; tmp.Yz = fSinAngle;
	tmp.Zy = -fSinAngle; tmp.Zz = fCosAngle;

	return *this *= tmp;
}

__forceinline Mat44_Ref Mat44::Rotate_GlobalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Xx = fCosAngle; tmp.Xz = -fSinAngle;
	tmp.Zx = fSinAngle; tmp.Zz = fCosAngle;

	return *this *= tmp;
}

__forceinline Mat44_Ref Mat44::Rotate_GlobalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Xx = fCosAngle; tmp.Xy = fSinAngle;
	tmp.Yx = -fSinAngle; tmp.Yy = fCosAngle;

	return *this *= tmp;
}

__forceinline Mat44_Ref Mat44::Rotate_GlobalX_Degrees(float fDegrees)
{
	return Rotate_GlobalX_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Rotate_GlobalY_Degrees(float fDegrees)
{
	return Rotate_GlobalY_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Rotate_GlobalZ_Degrees(float fDegrees)
{
	return Rotate_GlobalZ_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Rotate_LocalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Yy = fCosAngle; tmp.Yz = fSinAngle;
	tmp.Zy = -fSinAngle; tmp.Zz = fCosAngle;

	return *this = tmp * (*this);
}

__forceinline Mat44_Ref Mat44::Rotate_LocalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Xx = fCosAngle; tmp.Xz = -fSinAngle;
	tmp.Zx = fSinAngle; tmp.Zz = fCosAngle;

	return *this = tmp * (*this);
}

__forceinline Mat44_Ref Mat44::Rotate_LocalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44 tmp;
	tmp.Xx = fCosAngle; tmp.Xy = fSinAngle;
	tmp.Yx = -fSinAngle; tmp.Yy = fCosAngle;

	return *this = tmp * (*this);
}

__forceinline Mat44_Ref Mat44::Rotate_LocalX_Degrees(float fDegrees)
{
	return Rotate_LocalX_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Rotate_LocalY_Degrees(float fDegrees)
{
	return Rotate_LocalY_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Rotate_LocalZ_Degrees(float fDegrees)
{
	return Rotate_LocalZ_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44_Ref Mat44::Scale(const Vec3f& vScale)
{
	xAxis.SetXYZ(xAxis.GetXYZ() * vScale.GetX());
	yAxis.SetXYZ(yAxis.GetXYZ() * vScale.GetY());
	zAxis.SetXYZ(zAxis.GetXYZ() * vScale.GetZ());
	return *this;
}

__forceinline Mat44_Ref Mat44::SetScale(const Vec3f& vScale)
{
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()) * vScale.GetX());
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()) * vScale.GetY());
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()) * vScale.GetZ());
	return *this;
}

__forceinline Vec3f Mat44::GetScale() const
{
	Vec3f retVal(xAxis.GetXYZ().Mag(), yAxis.GetXYZ().Mag(), zAxis.GetXYZ().Mag());
	return retVal;
}

__forceinline Mat44_Ref Mat44::Translate(Vec3f vTranslation)
{
	wAxis.SetXYZ(wAxis.GetXYZ() + vTranslation);
	return *this;
}

__forceinline Mat44_Ref Mat44::Transpose()
{
#if SSE_AVAILABLE
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

__forceinline Mat44_Ref Mat44::Transpose3x3()
{
	std::swap(Xy, Yx);
	std::swap(Xz, Zx);
	std::swap(Yz, Zy);

	return *this;
}

__forceinline Mat44_Ref Mat44::NormalizeXYZ()
{
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()));
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()));
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()));

	return *this;
}

__forceinline Mat44_Ref Mat44::Invert()
{
	return *this = XMMatrixInverse(NULL, XMLoadFloat4x4((XMFLOAT4X4*)this));
}

__forceinline Mat44_Ref Mat44::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	zAxis.SetXYZ(Normalize(mPos - wAxis.GetXYZ()));
	xAxis.SetXYZ(Normalize(CrossProduct(vWorldUp, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));

	return *this;
}

inline Mat44_Ref Mat44::TurnTo(Vec3f_In vPos, float fDeltaTime, float fTurnModifier)
{
	Vec3f vecToPos = Normalize(vPos - wAxis.GetXYZ());

	float protection = DotProduct(vecToPos, zAxis.GetXYZ());

	if (protection + 1 <= protection || protection != protection)
	{
		// You are already facing that exact direction
		return *this;
	}

	float fRotation = DotProduct(vecToPos, xAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
	{
		// protection to keep the matrix from turning slowly
		// if the position is behind the matrix
		if(DotProduct(vecToPos, zAxis.GetXYZ()) < 0.0f)
			fRotation = (fRotation < 0.0f) ? -1.0f : 1.0f;

		Rotate_LocalY_Radians(fRotation * fTurnModifier * fDeltaTime);
	}

	fRotation = DotProduct(vecToPos, yAxis.GetXYZ());

	if(fRotation > FLT_EPSILON || fRotation < -FLT_EPSILON)
		Rotate_LocalX_Radians(-fRotation * fTurnModifier * fDeltaTime);

	xAxis.SetXYZ(Normalize(CrossProduct(g_WorldUp, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));

	return *this;
}

__forceinline Mat44_Ref Mat44::OrthoNormalInvert()
{
	Mat44 tmp(*this);

	Transpose3x3();

	Wx = -DotProduct(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	Wy = -DotProduct(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	Wz = -DotProduct(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());

	return *this;
}

#endif //MAT44_INL