
#pragma region Matrix3f Constructors
inline Matrix3f::Matrix3f()
{
	make_identity();
}

inline Matrix3f::Matrix3f(float fXx, float fXy, float fXz, 
						  float fYx, float fYy, float fYz, 
						  float fZx, float fZy, float fZz)
{
	Xx = fXx;	Xy = fXy;	Xz = fXz;
	Yx = fYx;	Yy = fYy;	Yz = fYz;
	Zx = fZx;	Zy = fZy;	Zz = fZz;
}

inline Matrix3f::Matrix3f(const Matrix3f& mMatrix) : xAxis(move(mMatrix.xAxis)), yAxis(move(mMatrix.yAxis)), zAxis(move(mMatrix.zAxis))
{

}

inline Matrix3f::Matrix3f(Matrix3f&& mMatrix) : xAxis(move(mMatrix.xAxis)), yAxis(move(mMatrix.yAxis)), zAxis(move(mMatrix.zAxis))
{

}

inline Matrix3f::Matrix3f(XMMATRIX&& mMatrix)
{
	XMStoreFloat3x3((XMFLOAT3X3*)this, mMatrix);
}

inline Matrix3f::Matrix3f(const Vec3f& vXAxis,
						  const Vec3f& vYAxis,
						  const Vec3f& vZAxis)
{
	xAxis = vXAxis;
	yAxis = vYAxis;
	zAxis = vZAxis;
}
#pragma endregion

#pragma region Matrix3f Operators
inline Matrix3f& Matrix3f::make_identity()
{
	xAxis = g_IdentityX3;
	xAxis = g_IdentityY3;
	xAxis = g_IdentityZ3;

	return *this;
}

inline Matrix3f& Matrix3f::operator=(const Matrix3f& mMatrix)
{
	xAxis = mMatrix.xAxis;
	yAxis = mMatrix.yAxis;
	zAxis = mMatrix.zAxis;

	return *this;
}

inline Matrix3f& Matrix3f::operator=(Matrix3f&& mMatrix)
{
	xAxis = move(mMatrix.xAxis);
	yAxis = move(mMatrix.yAxis);
	zAxis = move(mMatrix.zAxis);

	return *this;
}

inline Matrix3f& Matrix3f::operator=(XMMATRIX&& mMatrix)
{
	XMStoreFloat3x3((XMFLOAT3X3*)this, mMatrix);

	return *this;
}

inline Matrix3f Matrix3f::operator*(const Matrix3f& mMatrix) const
{
	return XMMatrixMultiply(XMLoadFloat3x3((XMFLOAT3X3*)this), XMLoadFloat3x3((XMFLOAT3X3*)&mMatrix));
}

inline Matrix3f& Matrix3f::operator*=(const Matrix3f& mMatrix)
{
	*this = *this * mMatrix;
}
#pragma endregion

#pragma region Matrix3f Math Funcs
inline Matrix3f& Matrix3f::Transpose()
{
	std::swap(Xy, Yx);
	std::swap(Xz, Zx);
	std::swap(Yz, Zy);
}
#pragma endregion