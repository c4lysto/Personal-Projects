#pragma region Matrix3f Constructors
inline Matrix34f::Matrix34f()
{
	make_identity();
}

inline Matrix34f::Matrix34f(float fXx, float fXy, float fXz, 
							float fYx, float fYy, float fYz, 
							float fZx, float fZy, float fZz,
							float fWx, float fWy, float fWz)
{
	Xx = fXx; Xy = fXy; Xz = fXz;
	Yx = fYx; Yy = fYy; Yz = fYz;
	Zx = fZx; Zy = fZy; Zz = fZz;
	Wx = fWx; Wy = fWy; Wz = fWz;
}

inline Matrix34f::Matrix34f(const Matrix34f&& mMatrix)
{
	*this = move(mMatrix);
}

inline Matrix34f::Matrix34f(const XMMATRIX&& mMatrix)
{
	XMStoreFloat4x3((XMFLOAT4X3*)this, mMatrix);
}

inline Matrix34f::Matrix34f(const Vec3f& vXAxis,
							const Vec3f& vYAxis,
							const Vec3f& vZAxis,
							const Vec3f& vWAxis)
{
	xAxis = vXAxis;
	yAxis = vYAxis;
	zAxis = vZAxis;
	wAxis = vWAxis;
}
#pragma endregion

#pragma region Matrix34f Operators
inline Matrix34f& Matrix34f::make_identity()
{
	xAxis = g_IdentityX3;
	yAxis = g_IdentityY3;
	zAxis = g_IdentityZ3;
	wAxis = g_IdentityW3;
}
#pragma endregion