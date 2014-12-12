#if SSE_AVAILABLE
#ifndef MAT44V_INL
#define MAT44V_INL


typedef Mat44V& Mat44V_Ref;
typedef const Mat44V& Mat44V_ConstRef;

typedef Mat44V_Ref Mat44V_InOut;
typedef Mat44V_ConstRef Mat44V_In;

typedef Mat44V Mat44V_Out;

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
			Vector row1, row2, row3, row4;
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
	explicit Mat44V(eXRotationInitializer eXRotation, float fRotationInRadians);
	explicit Mat44V(eYRotationInitializer eXRotation, float fRotationInRadians);
	explicit Mat44V(eZRotationInitializer eXRotation, float fRotationInRadians);

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
	Mat44V_MUTATOR(SetXAxis, Vec4V_In, xAxis)
	Mat44V_MUTATOR(SetYAxis, Vec4V_In, yAxis)
	Mat44V_MUTATOR(SetZAxis, Vec4V_In, zAxis)
	Mat44V_MUTATOR(SetWAxis, Vec4V_In, wAxis)
#undef Mat44V_MUTATOR
#else
#error Mat44V MUTATORS NOT DEFINED!
#endif

	//explicit operator XMMATRIX() const;
	//explicit operator XMMATRIX*() const;

	Mat44V_ConstRef operator=(Mat44V_In mMatrix);
	Mat44V_ConstRef operator=(Mat44V&& mMatrix);
	Mat44V_ConstRef operator=(XMMATRIX&& mMatrix);

	Mat44V operator*(Mat44V_In mMatrix) const;
	void operator*=(Mat44V_In mMatrix);

	// actually faster than DirectX Version :)
	void Rotate_GlobalX(float fRadians);
	void Rotate_GlobalY(float fRadians);
	void Rotate_GlobalZ(float fRadians);

	void Rotate_LocalX(float fRadians);
	void Rotate_LocalY(float fRadians);
	void Rotate_LocalZ(float fRadians);

	void Scale(Vec3V_In vScale);

	void SetScale(Vec3V_In vScale);
	Vec3V GetScale() const;

	void Translate(Vec3V_In vTranslation);

	void Transpose();
	void Transpose3x3();

	void NormalizeXYZ();

	void Invert();

	void LookAt(Vec3V_In mPos, Vec3V_In vWorldUp = g_IdentityY3V);

	void TurnTo(Vec3V_In vPos, float fDeltaTime, float fTurnModifier = 1.0f);

	void OrthoNormalInvert();
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

inline Mat44V::Mat44V(eIdentityInitializer eIdentity)
{
	xAxis = g_IdentityX4V;
	yAxis = g_IdentityY4V;
	zAxis = g_IdentityZ4V;
	wAxis = g_IdentityW4V;
}

inline Mat44V::Mat44V(eXRotationInitializer eXRotation, float fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = g_IdentityX4V;
	yAxis = Vec4V(0.0f, fCosAngle, fSinAngle, 0.0f);
	zAxis = Vec4V(0.0f, -fSinAngle, fCosAngle, 0.0f);
	wAxis = g_IdentityW4V;
}

inline Mat44V::Mat44V(eYRotationInitializer eYRotation, float fRotationInRadians)
{
	float fSinAngle = sin(fRotationInRadians);
	float fCosAngle = cos(fRotationInRadians);
	xAxis = Vec4V(fCosAngle, 0.0f, -fSinAngle, 0.0f);
	yAxis = g_IdentityY4V;
	zAxis = Vec4V(fSinAngle, 0.0f, fCosAngle, 0.0f);
	wAxis = g_IdentityW4V;
}

inline Mat44V::Mat44V(eZRotationInitializer eZRotation, float fRotationInRadians)
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

// actually faster than DirectX Version :)
__forceinline void Mat44V::Rotate_GlobalX(float fRadians)
{
	Mat44V tmp(INIT_ROTATION_X, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_GlobalY(float fRadians)
{
	Mat44V tmp(INIT_ROTATION_Y, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_GlobalZ(float fRadians)
{
	Mat44V tmp(INIT_ROTATION_Z, fRadians);
	*this *= tmp;
}

__forceinline void Mat44V::Rotate_LocalX(float fRadians)
{
	Mat44V tmp(INIT_ROTATION_X, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44V::Rotate_LocalY(float fRadians)
{
	Mat44V tmp(INIT_ROTATION_Y, fRadians);
	*this = tmp * (*this);
}

__forceinline void Mat44V::Rotate_LocalZ(float fRadians)
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
	xAxis.SetXYZ(Normalize(xAxis.GetXYZ()) * vScale.GetX());
	yAxis.SetXYZ(Normalize(yAxis.GetXYZ()) * vScale.GetY());
	zAxis.SetXYZ(Normalize(zAxis.GetXYZ()) * vScale.GetZ());
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
	Vector tmp1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 1, 0));
	Vector tmp2 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(1, 0, 1, 0));
	Vector tmp3 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(3, 2, 3, 2));
	Vector tmp4 = _mm_shuffle_ps(row3, row4, _MM_SHUFFLE(3, 2, 3, 2));

	row1 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0));
	row2 = _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1));
	row3 = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0));
	row4 = _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1));
}

__forceinline void Mat44V::Transpose3x3()
{
	std::swap(xAxis.y, yAxis.x);
	std::swap(xAxis.z, zAxis.x);
	std::swap(yAxis.z, zAxis.y);
}

__forceinline void Mat44V::NormalizeXYZ()
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
	xAxis.SetXYZ(CrossProduct(vWorldUp, zAxis.GetXYZ()));
	xAxis.Normalize();

	// yAxis;
	yAxis.SetXYZ(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ()));
	yAxis.Normalize();
}

inline void Mat44V::TurnTo(Vec3V_In vPos, float fDeltaTime, float fTurnModifier)
{
	Vec3V vecToPos = vPos - wAxis.GetXYZ();
	vecToPos.Normalize();

	float protection = DotProduct(vecToPos, zAxis.GetXYZ());

	if (protection + 1 <= protection || protection != protection)
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

	xAxis.SetXYZ(Normalize(CrossProduct(g_WorldUp3V, zAxis.GetXYZ())));
	yAxis.SetXYZ(Normalize(CrossProduct(zAxis.GetXYZ(), xAxis.GetXYZ())));
}

inline void Mat44V::OrthoNormalInvert()
{
	Mat44V tmp(*this);

	Transpose3x3();

	wAxis.x = -DotProduct(tmp.wAxis.GetXYZ(), tmp.xAxis.GetXYZ());
	wAxis.y = -DotProduct(tmp.wAxis.GetXYZ(), tmp.yAxis.GetXYZ());
	wAxis.z = -DotProduct(tmp.wAxis.GetXYZ(), tmp.zAxis.GetXYZ());
}
#endif //MAT44V_INL
#endif //SSE_AVAILABLE