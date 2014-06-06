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
			Vec3f xAxis; float padXW;
			Vec3f yAxis; float padYW;
			Vec3f zAxis; float padZW;

			union
			{
				struct
				{
					Vec3f wAxis;
				};

				struct
				{
					Vec3f position;
				};
			};

			float padWW;
		};

		struct
		{
			Vec4f row1, row2, row3, row4;
		};
	};

public:
	// Matrix is always initialized to an identity matrix
	Mat44(){}
	explicit Mat44(float fXx, float fXy, float fXz, float fXw,
					  float fYx, float fYy, float fYz, float fYw,
					  float fZx, float fZy, float fZz, float fZw,
					  float fWx, float fWy, float fWz, float fWw);
	Mat44(Mat44_In mMatrix);
	Mat44(Mat44&& mMatrix);
	Mat44(XMMATRIX&& mMatrix);
	explicit Mat44(const Vec4f& vXAxis,
					  const Vec4f& vYAxis,
					  const Vec4f& vZAxis,
					  const Vec4f& vWAxis);

	explicit Mat44(eIdentityInitializer eIdentity);

#if defined(Mat44_ACCESSOR) && defined(Mat44_ACCESSOR_CONST)
	Mat44_ACCESSOR_CONST(Vec4f, GetXAxis, row1)
	Mat44_ACCESSOR_CONST(Vec4f, GetYAxis, row2)
	Mat44_ACCESSOR_CONST(Vec4f, GetZAxis, row3)
	Mat44_ACCESSOR_CONST(Vec4f, GetWAxis, row4)

	Mat44_ACCESSOR(Vec4f_Ref, GetXAxisRef, row1)
	Mat44_ACCESSOR(Vec4f_Ref, GetYAxisRef, row2)
	Mat44_ACCESSOR(Vec4f_Ref, GetZAxisRef, row3)
	Mat44_ACCESSOR(Vec4f_Ref, GetWAxisRef, row4)
#undef Mat44_ACCESSOR
#undef Mat44_ACCESSOR_CONST
#else
#error Mat44 ACCESSORS NOT DEFINED
#endif

#ifdef Mat44_MUTATOR
	Mat44_MUTATOR(SetXAxis, Vec4f, row1)
	Mat44_MUTATOR(SetYAxis, Vec4f, row2)
	Mat44_MUTATOR(SetZAxis, Vec4f, row3)
	Mat44_MUTATOR(SetWAxis, Vec4f, row4)
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

	Mat44_Ref Scale(float fXScale, float fYScale, float fZScale);
	Mat44_Ref Scale(const Vec3f& vScale);

	Mat44_Ref SetScale(float fXScale, float fYScale, float fZScale);
	Mat44_Ref SetScale(const Vec3f& vScale);
	Mat44_Ref SetScale(float fScale);
	Vec3f GetScale() const;

	Mat44_Ref Translate(float fX, float fY, float fZ);
	Mat44_Ref Translate(Vec3f vTranslation);

	Mat44_Ref Transpose();
	Mat44_Ref Transpose3x3();

	Mat44_Ref NormalizeXYZ();

	Mat44_Ref Invert();

	Mat44_Ref LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	Mat44_Ref TurnTo(const Vec3f& vPos, float fTurnModifier = 1.0f);

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
	row1 = Vec4f(fXx, fXy, fXz, fXw);
	row2 = Vec4f(fYx, fYy, fYz, fYw);
	row3 = Vec4f(fZx, fZy, fZz, fZw);
	row4 = Vec4f(fWx, fWy, fWz, fWw);
#endif
}

__forceinline Mat44::Mat44(Mat44_In mMatrix)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
	_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
	_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
	_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
	row1 = mMatrix.row1;
	row2 = mMatrix.row2;
	row3 = mMatrix.row3;
	row4 = mMatrix.row4;
#endif
}

__forceinline Mat44::Mat44(Mat44&& mMatrix)
{
#if SSE_AVAILABLE
	_mm_storeu_ps(m, _mm_loadu_ps(mMatrix.m));
	_mm_storeu_ps(m + 4, _mm_loadu_ps(mMatrix.m + 4));
	_mm_storeu_ps(m + 8, _mm_loadu_ps(mMatrix.m + 8));
	_mm_storeu_ps(m + 12, _mm_loadu_ps(mMatrix.m + 12));
#else
	row1 = mMatrix.row1;
	row2 = mMatrix.row2;
	row3 = mMatrix.row3;
	row4 = mMatrix.row4;
#endif
}

__forceinline Mat44::Mat44(eIdentityInitializer eIdentity)
{
	row1 = g_IdentityX4;
	row2 = g_IdentityY4;
	row3 = g_IdentityZ4;
	row4 = g_IdentityW4;
}

__forceinline Mat44::Mat44(XMMATRIX&& mMatrix)
{
	XMStoreFloat4x4((XMFLOAT4X4*)this, mMatrix);
}

__forceinline Mat44::Mat44(const Vec4f& vXAxis,
						  const Vec4f& vYAxis,
						  const Vec4f& vZAxis,
						  const Vec4f& vWAxis)
{
#if SSE_AVAILABLE
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

__forceinline Mat44_Ref Mat44::operator=(Mat44_In mMatrix)
{
	if(this != &mMatrix)
	{
#if SSE_AVAILABLE
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

__forceinline Mat44_Ref Mat44::operator=(Mat44&& mMatrix)
{
#if SSE_AVAILABLE
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

__forceinline Vec4f operator*(Vec4f_In vVector, Mat44_In mMatrix)
{
	float fX = vVector.GetX();
	float fY = vVector.GetY();
	float fZ = vVector.GetZ();
	float fW = vVector.GetW();

#ifdef SSE_AVAILABLE
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
#if SSE_AVAILABLE
	_mm_storeu_ps(m, _mm_setr_ps(1.0f, 0.0f, 0.0f, Xw));
	_mm_storeu_ps(m + 4, _mm_setr_ps(0.0f, 1.0f, 0.0f, Yw));
	_mm_storeu_ps(m + 8, _mm_setr_ps(0.0f, 0.0f, 1.0f, Zw));
#else
	xAxis = g_IdentityX3;
	yAxis = g_IdentityY3;
	zAxis = g_IdentityZ3;
#endif

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

__forceinline Mat44_Ref Mat44::Scale(float fXScale, float fYScale, float fZScale)
{
	xAxis *= fXScale;
	yAxis *= fYScale;
	zAxis *= fZScale;

	return *this;
}

__forceinline Mat44_Ref Mat44::Scale(const Vec3f& vScale)
{
	return Scale(vScale.x, vScale.y, vScale.z);
}

__forceinline Mat44_Ref Mat44::SetScale(float fXScale, float fYScale, float fZScale)
{
	xAxis.Normalize();
	xAxis *= fXScale;

	yAxis.Normalize();
	yAxis *= fYScale;

	zAxis.Normalize();
	zAxis *= fZScale;

	return *this;
}

__forceinline Mat44_Ref Mat44::SetScale(const Vec3f& vScale)
{
	return SetScale(vScale.x, vScale.y, vScale.z);
}

__forceinline Mat44_Ref Mat44::SetScale(float fScale)
{
	return SetScale(fScale, fScale, fScale);
}

__forceinline Vec3f Mat44::GetScale() const
{
	Vec3f retVal(xAxis.Mag(), yAxis.Mag(), zAxis.Mag());
	return retVal;
}

__forceinline Mat44_Ref Mat44::Translate(float fX, float fY, float fZ)
{
	Wx += fX;
	Wy += fY;
	Wz += fZ;

	return *this;
}

__forceinline Mat44_Ref Mat44::Translate(Vec3f vTranslation)
{
	position += vTranslation;
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
	xAxis.Normalize();
	yAxis.Normalize();
	zAxis.Normalize();

	return *this;
}

__forceinline Mat44_Ref Mat44::Invert()
{
	return *this = XMMatrixInverse(NULL, XMLoadFloat4x4((XMFLOAT4X4*)this));
}

__forceinline Mat44_Ref Mat44::LookAt(const Vec3f& mPos, const Vec3f& vWorldUp)
{
	zAxis = mPos - position;
	zAxis.Normalize();

	xAxis = CrossProduct(vWorldUp, zAxis);
	xAxis.Normalize();

	yAxis = CrossProduct(zAxis, xAxis);
	yAxis.Normalize();

	return *this;
}

__forceinline Mat44_Ref Mat44::TurnTo(const Vec3f& vPos, float fTurnModifier)
{

}

__forceinline Mat44_Ref Mat44::OrthoNormalInvert()
{
	Mat44 tmp(*this);

	Transpose3x3();

	Wx = -DotProduct(tmp.position, tmp.xAxis);
	Wy = -DotProduct(tmp.position, tmp.yAxis);
	Wz = -DotProduct(tmp.position, tmp.zAxis);

	return *this;
}

#endif //MAT44_INL