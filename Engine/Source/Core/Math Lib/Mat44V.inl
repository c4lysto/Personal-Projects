#if SSE_AVAILABLE
#ifndef MAT44V_INL
#define MAT44V_INL


typedef Mat44V& Mat44V_Ref;
typedef const Mat44V& Mat44V_ConstRef;

typedef Mat44V_ConstRef Mat44V_In;

// Other Mat44V Aliases
typedef Mat44V float4x4V;

ALIGN(16) class Mat44V
{
#define Mat44V_ACCESSOR(retType, name, retVal) \
	__forceinline retType name() { return retVal; }

#define Mat44V_ACCESSOR_CONST(retType, name, retVal) \
	__forceinline retType name() const { return retVal; }

#define Mat44V_MUTATOR(name, inType, modifiedVal) \
	__forceinline void name(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		float m[16];
		float ma[4][4];

		struct
		{
			__m128 row1, row2, row3, row4;
		};

		struct
		{
			float Xx, Xy, Xz, Xw,
				  Yx, Yy, Yz, Yw,
				  Zx, Zy, Zz, Zw,
				  Wx, Wy, Wz, Ww;
		};

		struct
		{
			Vec4V xAxis;
			Vec4V yAxis;
			Vec4V zAxis;
			Vec4V wAxis;
		};
	};

public:
	Mat44V(){}
	explicit Mat44V(float fXx, float fXy, float fXz, float fXw,
					float fYx, float fYy, float fYz, float fYw,
					float fZx, float fZy, float fZz, float fZw,
					float fWx, float fWy, float fWz, float fWw);
	Mat44V(Mat44V_In mMatrix);
	Mat44V(Mat44V&& mMatrix);
	Mat44V(XMMATRIX&& mMatrix);
	explicit Mat44V(Vec4V_In vXAxis,
					Vec4V_In vYAxis,
					Vec4V_In vZAxis,
					Vec4V_In vWAxis);

	explicit Mat44V(eIdentityInitializer eIdentity);

#if defined(Mat44V_ACCESSOR) && defined(Mat44V_ACCESSOR_CONST)
	Mat44V_ACCESSOR_CONST(Vec4V, GetXAxis, xAxis)
	Mat44V_ACCESSOR_CONST(Vec4V, GetYAxis, yAxis)
	Mat44V_ACCESSOR_CONST(Vec4V, GetZAxis, zAxis)
	Mat44V_ACCESSOR_CONST(Vec4V, GetWAxis, wAxis)

	Mat44V_ACCESSOR(Vec4V_Ref, GetXAxisRef, xAxis)
	Mat44V_ACCESSOR(Vec4V_Ref, GetYAxisRef, yAxis)
	Mat44V_ACCESSOR(Vec4V_Ref, GetZAxisRef, zAxis)
	Mat44V_ACCESSOR(Vec4V_Ref, GetWAxisRef, wAxis)
#undef Mat44V_ACCESSOR
#undef Mat44V_ACCESSOR_CONST
#else
#error Mat44V ACCESSORS NOT DEFINED
#endif

#ifdef Mat44V_MUTATOR
	Mat44V_MUTATOR(SetXAxis, Vec4V, xAxis)
	Mat44V_MUTATOR(SetYAxis, Vec4V, yAxis)
	Mat44V_MUTATOR(SetZAxis, Vec4V, zAxis)
	Mat44V_MUTATOR(SetWAxis, Vec4V, wAxis)
#undef Mat44V_MUTATOR
#else
#error Mat44V MUTATORS NOT DEFINED!
#endif

	//explicit operator XMMATRIX() const;
	//explicit operator XMMATRIX*() const;

	Mat44V& operator=(Mat44V_In mMatrix);
	Mat44V& operator=(Mat44V&& mMatrix);
	Mat44V& operator=(XMMATRIX&& mMatrix);

	Mat44V operator*(Mat44V_In mMatrix) const;
	Mat44V& operator*=(Mat44V_In mMatrix);

	// actually faster than DirectX Version :)
	Mat44V& Rotate_GlobalX_Radians(float fRadians);
	Mat44V& Rotate_GlobalY_Radians(float fRadians);
	Mat44V& Rotate_GlobalZ_Radians(float fRadians);

	Mat44V& Rotate_GlobalX_Degrees(float fDegrees);
	Mat44V& Rotate_GlobalY_Degrees(float fDegrees);
	Mat44V& Rotate_GlobalZ_Degrees(float fDegrees);

	Mat44V& Rotate_LocalX_Radians(float fRadians);
	Mat44V& Rotate_LocalY_Radians(float fRadians);
	Mat44V& Rotate_LocalZ_Radians(float fRadians);

	Mat44V& Rotate_LocalX_Degrees(float fDegrees);
	Mat44V& Rotate_LocalY_Degrees(float fDegrees);
	Mat44V& Rotate_LocalZ_Degrees(float fDegrees);

	Mat44V& Scale(Vec3V_In vScale);

	Mat44V& SetScale(Vec3V_In vScale);
	Vec3V GetScale() const;

	Mat44V& Translate(Vec3V_In vTranslation);

	Mat44V& Transpose();
	Mat44V& Transpose3x3();

	Mat44V& NormalizeXYZ();

	Mat44V& Invert();

	Mat44V& LookAt(Vec3V_In mPos, Vec3V_In vWorldUp = g_IdentityY3V);

	Mat44V& TurnTo(Vec3V_In vPos, float fDeltaTime, float fTurnModifier = 1.0f);

	Mat44V& OrthoNormalInvert();
};

__forceinline Mat44V::Mat44V(float fXx, float fXy, float fXz, float fXw,
							 float fYx, float fYy, float fYz, float fYw,
							 float fZx, float fZy, float fZz, float fZw,
							 float fWx, float fWy, float fWz, float fWw)
{
	xAxis = Vec4V(fXx, fXy, fXz, fXw);
	yAxis = Vec4V(fYx, fYy, fYz, fYw);
	zAxis = Vec4V(fZx, fZy, fZz, fZw);
	wAxis = Vec4V(fWx, fWy, fWz, fWw);
}

__forceinline Mat44V::Mat44V(Mat44V_In mMatrix)
{
	xAxis = mMatrix.xAxis;
	yAxis = mMatrix.yAxis;
	zAxis = mMatrix.zAxis;
	wAxis = mMatrix.wAxis;
}

__forceinline Mat44V::Mat44V(Mat44V&& mMatrix)
{
	xAxis = move(mMatrix.xAxis);
	yAxis = move(mMatrix.yAxis);
	zAxis = move(mMatrix.zAxis);
	wAxis = move(mMatrix.wAxis);
}

__forceinline Mat44V::Mat44V(XMMATRIX&& mMatrix)
{
	row1 = move(mMatrix.r[0]);
	row2 = move(mMatrix.r[1]);
	row3 = move(mMatrix.r[2]);
	row4 = move(mMatrix.r[3]);
}

__forceinline Mat44V::Mat44V(Vec4V_In vXAxis,
							 Vec4V_In vYAxis,
							 Vec4V_In vZAxis,
							 Vec4V_In vWAxis)
{
	xAxis = vXAxis;
	yAxis = vYAxis;
	zAxis = vZAxis;
	wAxis = vWAxis;
}

__forceinline Mat44V::Mat44V(eIdentityInitializer eIdentity)
{
	xAxis = g_IdentityX4V;
	yAxis = g_IdentityY4V;
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

__forceinline Mat44V& Mat44V::operator=(Mat44V_In mMatrix)
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

__forceinline Mat44V& Mat44V::operator=(Mat44V&& mMatrix)
{
	xAxis = move(mMatrix.xAxis);
	yAxis = move(mMatrix.yAxis);
	zAxis = move(mMatrix.zAxis);
	wAxis = move(mMatrix.wAxis);

	return *this;
}

__forceinline Mat44V& Mat44V::operator=(XMMATRIX&& mMatrix)
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

inline Mat44V& Mat44V::operator*=(Mat44V_In mMatrix)
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

// actually faster than DirectX Version :)
__forceinline Mat44V& Mat44V::Rotate_GlobalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = g_IdentityX4V;
	tmp.yAxis = Vec4V(0.0f, fCosAngle, fSinAngle, 0.0f);
	tmp.zAxis = Vec4V(0.0f, -fSinAngle, fCosAngle, 0.0f);
	tmp.wAxis = g_IdentityW4V;

	return *this *= tmp;
}

__forceinline Mat44V& Mat44V::Rotate_GlobalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = Vec4V(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	tmp.yAxis = g_IdentityY4V;
	tmp.zAxis = Vec4V(fSinAngle, 0.0f, fCosAngle, 0.0f);
	tmp.wAxis = g_IdentityW4V;

	return *this *= tmp;
}

__forceinline Mat44V& Mat44V::Rotate_GlobalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = Vec4V(fCosAngle, fSinAngle, 0.0f, 0.0f);
	tmp.yAxis = Vec4V(-fSinAngle, fCosAngle, 0.0f, 0.0f);
	tmp.zAxis = g_IdentityZ4V;
	tmp.wAxis = g_IdentityW4V;

	return *this *= tmp;
}

__forceinline Mat44V& Mat44V::Rotate_GlobalX_Degrees(float fDegrees)
{
	return Rotate_GlobalX_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Rotate_GlobalY_Degrees(float fDegrees)
{
	return Rotate_GlobalY_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Rotate_GlobalZ_Degrees(float fDegrees)
{
	return Rotate_GlobalZ_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Rotate_LocalX_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = g_IdentityX4V;
	tmp.yAxis = Vec4V(0.0f, fCosAngle, fSinAngle, 0.0f);
	tmp.zAxis = Vec4V(0.0f, -fSinAngle, fCosAngle, 0.0f);
	tmp.wAxis = g_IdentityW4V;

	return *this = tmp * (*this);
}

__forceinline Mat44V& Mat44V::Rotate_LocalY_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = Vec4V(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	tmp.yAxis = g_IdentityY4V;
	tmp.zAxis = Vec4V(fSinAngle, 0.0f, fCosAngle, 0.0f);
	tmp.wAxis = g_IdentityW4V;

	return *this = tmp * (*this);
}

__forceinline Mat44V& Mat44V::Rotate_LocalZ_Radians(float fRadians)
{
	float fSinAngle = sin(fRadians);
	float fCosAngle = cos(fRadians);

	Mat44V tmp;
	tmp.xAxis = Vec4V(fCosAngle, fSinAngle, 0.0f, 0.0f);
	tmp.yAxis = Vec4V(-fSinAngle, fCosAngle, 0.0f, 0.0f);
	tmp.zAxis = g_IdentityZ4V;
	tmp.wAxis = g_IdentityW4V;

	return *this = tmp * (*this);
}

__forceinline Mat44V& Mat44V::Rotate_LocalX_Degrees(float fDegrees)
{
	return Rotate_LocalX_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Rotate_LocalY_Degrees(float fDegrees)
{
	return Rotate_LocalY_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Rotate_LocalZ_Degrees(float fDegrees)
{
	return Rotate_LocalZ_Radians(DEGREES_TO_RADIANS(fDegrees));
}

__forceinline Mat44V& Mat44V::Scale(Vec3V_In vScale)
{
	xAxis.SetXYZ(xAxis.GetXYZ() * vScale.GetX());
	yAxis.SetXYZ(yAxis.GetXYZ() * vScale.GetY());
	zAxis.SetXYZ(zAxis.GetXYZ() * vScale.GetZ());
	return *this;
}

__forceinline Mat44V& Mat44V::SetScale(Vec3V_In vScale)
{
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()) * vScale.GetX());
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()) * vScale.GetY());
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()) * vScale.GetZ());
	return *this;
}

__forceinline Vec3V Mat44V::GetScale() const
{
	Vec3V retVal(xAxis.Mag(), yAxis.Mag(), zAxis.Mag());
	return retVal;
}

__forceinline Mat44V& Mat44V::Translate(Vec3V_In vTranslation)
{
	wAxis.SetXYZ(wAxis.GetXYZ() + vTranslation);
	return *this;
}

__forceinline Mat44V& Mat44V::Transpose()
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

__forceinline Mat44V& Mat44V::Transpose3x3()
{
	std::swap(Xy, Yx);
	std::swap(Xz, Zx);
	std::swap(Yz, Zy);

	return *this;
}

__forceinline Mat44V& Mat44V::NormalizeXYZ()
{
	xAxis.Normalize();
	yAxis.Normalize();
	zAxis.Normalize();

	return *this;
}

__forceinline Mat44V& Mat44V::Invert()
{
	return *this = XMMatrixInverse(NULL, *(XMMATRIX*)this);
}

__forceinline Mat44V& Mat44V::LookAt(Vec3V_In mPos, Vec3V_In vWorldUp)
{
	Vec3V vecToPos = mPos - wAxis.GetXYZ();
	vecToPos.Normalize();

	// zAxis
	zAxis.SetXYZ(vecToPos);

	// xAxis;
	xAxis.SetXYZ(CrossProduct(vWorldUp, zAxis.GetXYZ()));
	xAxis.Normalize();

	// yAxis;
	yAxis.SetXYZ(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ()));
	yAxis.Normalize();

	return *this;
}

__forceinline Mat44V& Mat44V::TurnTo(Vec3V_In vPos, float fDeltaTime, float fTurnModifier)
{
	Vec3V vecToPos = vPos - wAxis.GetXYZ();
	vecToPos.Normalize();

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

	xAxis.SetXYZ(Normalize(CrossProduct(g_WorldUp3V, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));

	return *this;
}

__forceinline Mat44V& Mat44V::OrthoNormalInvert()
{
	Mat44V tmp(*this);

	Transpose3x3();

	Wx = -DotProduct(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	Wy = -DotProduct(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	Wz = -DotProduct(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());

	return *this;
}
#endif //MAT44V_INL
#endif //SSE_AVAILABLE