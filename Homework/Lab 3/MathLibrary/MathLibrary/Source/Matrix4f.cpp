#include "Matrix4f.h"

Matrix4f& Matrix4f::Scale(float fXScale, float fYScale, float fZScale)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 scale = _mm_setr_ps(fXScale, fXScale, fXScale, 1.0f);
	matrix.r[0] = _mm_mul_ps(matrix.r[0], scale);

	scale = _mm_setr_ps(fYScale, fYScale, fYScale, 1.0f);
	matrix.r[1] = _mm_mul_ps(matrix.r[1], scale);

	scale = _mm_setr_ps(fZScale, fZScale, fZScale, 1.0f);
	matrix.r[2] = _mm_mul_ps(matrix.r[2], scale);
#else
	xAxis *= fXScale;
	yAxis *= fYScale;
	zAxis *= fZScale;
#endif
	return *this;
}

Matrix4f& Matrix4f::Scale(const Vec3f& vScale)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 scale = _mm_setr_ps(vScale.x, vScale.x, vScale.x, 1.0f);
	matrix.r[0] = _mm_mul_ps(matrix.r[0], scale);

	scale = _mm_setr_ps(vScale.y, vScale.y, vScale.y, 1.0f);
	matrix.r[1] = _mm_mul_ps(matrix.r[1], scale);

	scale = _mm_setr_ps(vScale.z, vScale.z, vScale.z, 1.0f);
	matrix.r[2] = _mm_mul_ps(matrix.r[2], scale);
#else
	xAxis *= vScale.x;
	yAxis *= vScale.y;
	zAxis *= vScale.z;
#endif
	return *this;
}

Matrix4f& Matrix4f::SetScale(float fXScale, float fYScale, float fZScale)
{
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 scale = _mm_setr_ps(fXScale, fXScale, fXScale, 1.0f);
	matrix.r[0] = _mm_mul_ps(matrix.r[0], scale);

	scale = _mm_setr_ps(fYScale, fYScale, fYScale, 1.0f);
	matrix.r[1] = _mm_mul_ps(matrix.r[1], scale);

	scale = _mm_setr_ps(fZScale, fZScale, fZScale, 1.0f);
	matrix.r[2] = _mm_mul_ps(matrix.r[2], scale);
#else
	xAxis *= fXScale;
	yAxis *= fYScale;
	zAxis *= fZScale;
#endif

	return *this;
}

Matrix4f& Matrix4f::SetScale(const Vec3f& vScale)
{
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 scale = _mm_setr_ps(vScale.x, vScale.x, vScale.x, 1.0f);
	matrix.r[0] = _mm_mul_ps(matrix.r[0], scale);

	scale = _mm_setr_ps(vScale.y, vScale.y, vScale.y, 1.0f);
	matrix.r[1] = _mm_mul_ps(matrix.r[1], scale);

	scale = _mm_setr_ps(vScale.z, vScale.z, vScale.z, 1.0f);
	matrix.r[2] = _mm_mul_ps(matrix.r[2], scale);
#else
	xAxis *= vScale.x;
	yAxis *= vScale.y;
	zAxis *= vScale.z;
#endif

	return *this;
}

Vec3f Matrix4f::GetScale() const
{
	return Vec3f(xAxis.magnitude(), yAxis.magnitude(), zAxis.magnitude());
}

Matrix4f& Matrix4f::Translate(float fX, float fY, float fZ)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 translation = _mm_setr_ps(fX, fY, fZ, 0.0f);
	matrix.r[3] = _mm_add_ps(matrix.r[3], translation);
#else
	position.x += fX;
	position.y += fY;
	position.z += fZ;
#endif

	return *this;
}

Matrix4f& Matrix4f::Translate(Vec3f vTranslation)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 translation = _mm_setr_ps(vTranslation.x, vTranslation.y, vTranslation.z, 0.0f);
	matrix.r[3] = _mm_add_ps(matrix.r[3], translation);
#else
	position += vTranslation;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveForward(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(zAxis.x, zAxis.y, zAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveBackward(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(-zAxis.x, -zAxis.y, -zAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveLeft(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(-xAxis.x, -xAxis.y, -xAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveRight(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(xAxis.x, xAxis.y, xAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveUp(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(yAxis.x, yAxis.y, yAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::MoveDown(float fMovement)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 direction = _mm_setr_ps(-yAxis.x, -yAxis.y, -yAxis.z, 0.0f);
	__m128 scalar  = _mm_set1_ps(fMovement);

	matrix.r[3] = _mm_add_ps(matrix.r[3], _mm_mul_ps(direction, scalar));
#else
	position += zAxis * fMovement;
#endif

	return *this;
}

Matrix4f& Matrix4f::Transpose()
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	_MM_TRANSPOSE4_PS(matrix.r[0], matrix.r[1], matrix.r[2], matrix.r[3]);
#else
	std::swap(Xy, Yx);
    std::swap(Xz, Zx);
    std::swap(Yz, Zy);
    std::swap(Wx, Xw);
    std::swap(Wy, Yw);
    std::swap(Wz, Zw);
#endif

	return *this;
}

Matrix4f& Matrix4f::Transpose3x3()
{
	std::swap(Xy, Yx);
    std::swap(Xz, Zx);
    std::swap(Yz, Zy);

	return *this;
}

Matrix4f& Matrix4f::Negate()
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
	__m128 negation = _mm_set1_ps(-1.0f);

	matrix.r[0] = _mm_mul_ps(matrix.r[0], negation);
	matrix.r[1] = _mm_mul_ps(matrix.r[1], negation);
	matrix.r[2] = _mm_mul_ps(matrix.r[2], negation);
	matrix.r[3] = _mm_mul_ps(matrix.r[3], negation);
#else
	xAxis *= -1.0f;
	yAxis *= -1.0f;
	zAxis *= -1.0f;
	position *= -1.0f;

	Xw *= -1.0f;
	Yw *= -1.0f;
	Zw *= -1.0f;
	Ww *= -1.0f;
#endif

	return *this;
}

Matrix4f& Matrix4f::Invert()
{
	matrix = XMMatrixInverse(0, matrix);

	return *this;
}

Matrix4f& Matrix4f::NormalizeXYZ()
{
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

	return *this;
}

Matrix4f& Matrix4f::LookAt(const Vec3f& mPos)
{
	zAxis = mPos - wAxis;
	zAxis.normalize();

	Vec3f WorldUp(0.0f, 1.0f, 0.0f);
	xAxis = cross_product(WorldUp, zAxis);
	xAxis.normalize();

	yAxis = cross_product(zAxis, xAxis);
	yAxis.normalize();

	return *this;
}

Matrix4f& Matrix4f::MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip)
{
	matrix = XMMatrixPerspectiveFovLH(fFOV, fAspectRatio, fNearClip, fFarClip);
	return *this;
}

Matrix4f MatrixTranspose(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	return tmp.Transpose();
}

Matrix4f MatrixTranspos3x3(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	return tmp.Transpose3x3();
}

Matrix4f MatrixScale(const Matrix4f& mMatrix, float fXScale, float fYScale, float fZScale)
{
	Matrix4f tmp(mMatrix);

	return tmp.Scale(fXScale, fYScale, fZScale);
}
		
Matrix4f MatrixInverse(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	return tmp.Invert();
}

Matrix4f MatrixNegate(const Matrix4f& mMatrix)
{
	Matrix4f tmp(mMatrix);

	return tmp.Negate();
}

/*Matrix4f Interpolate(const Matrix4f& MatrixA, const Matrix4f& MatrixB, double dLamda)
{
	Matrix4f iMat(MatrixA);
	Matrix4f result(MatrixB);

	// Inverse of MatrixA
	iMat.Invert();

	// Remove MatrixA's transformation from MatrixB
	result *= iMat;

	// iMat is now the intermediary transformation from MatrixA to MatrixB
	iMat = result;

	// The trace of our matrix
	float fTrace = 1.0f + iMat.ma[0] + iMat.ma[5] + iMat.ma[10];

	float fQuatResult[4];

	// Calculate the quaternion of iMat
	// If trace is greater than 0, but consider small values that
	// might result in 0 when operated upon due to floating point error
	if (fTrace > 0.00000001f)
	{
		float fSq = sqrt(fTrace) * 2;
		fQuatResult[0] = (iMat.ma[9] - iMat.ma[6]) / fSq;
		fQuatResult[1] = (iMat.ma[2] - iMat.ma[8]) / fSq;
		fQuatResult[2] = (iMat.ma[4] - iMat.ma[1]) / fSq;
		fQuatResult[3] = 0.25f * fSq;
	}
	else
	{
		if( iMat.ma[0] > iMat.ma[5] && iMat.ma[0] > iMat.ma[10] )
		{
			float fSq = sqrt(1.0f + iMat.ma[0] - iMat.ma[5] - iMat.ma[10]) * 2;
			fQuatResult[0] = 0.25f * fSq;
			fQuatResult[1] = (iMat.ma[4] + iMat.ma[1]) / fSq;
			fQuatResult[2] = (iMat.ma[2] + iMat.ma[8]) / fSq;
			fQuatResult[3] = (iMat.ma[9] - iMat.ma[6]) / fSq;
		}
		else if( iMat.ma[5] > iMat.ma[10] )
		{
			float fSq = sqrt(1.0f + iMat.ma[5] - iMat.ma[0] - iMat.ma[10]) * 2;
			fQuatResult[0] = (iMat.ma[4] + iMat.ma[1]) / fSq;
			fQuatResult[1] = 0.25f * fSq;
			fQuatResult[2] = (iMat.ma[9] + iMat.ma[6]) / fSq;
			fQuatResult[3] = (iMat.ma[2] - iMat.ma[8]) / fSq;
		}
		else
		{
			float fSq = sqrt(1.0f + iMat.ma[10] - iMat.ma[0] - iMat.ma[5]) * 2;
			fQuatResult[0] = (iMat.ma[2] + iMat.ma[8]) / fSq;
			fQuatResult[1] = (iMat.ma[9] + iMat.ma[6]) / fSq;
			fQuatResult[2] = 0.25f * fSq;
			fQuatResult[3] = (iMat.ma[4] - iMat.ma[1]) / fSq;
		}
	}

	// Get the magnitude of our quaternion
	float fQuatMagnitude = sqrt(fQuatResult[0] * fQuatResult[0] + fQuatResult[1] * fQuatResult[1] + fQuatResult[2] * fQuatResult[2] + fQuatResult[3] * fQuatResult[3]);

	// Normalize our quaternion
	float fQuatNormalized[4] = {fQuatResult[0] / fQuatMagnitude, fQuatResult[1] / fQuatMagnitude, fQuatResult[2] / fQuatMagnitude, fQuatResult[3] / fQuatMagnitude}; 

	// Calculate the angles relevant to our quaternion
	float fCos_a = fQuatNormalized[3];
	float fAngle = acos(fCos_a) * 2;
	float fSin_a = sqrt(1.0f - fCos_a * fCos_a);
	
	// If there was no rotation between matrices, calculation
	// of the rotation matrix will end badly. So just do the linear
	// interpolation of the translation component and return
	if (fAngle == 0.0f)
	{
		result = MatrixA;

		result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0]) * (float)dLamda);
		result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1]) * (float)dLamda);
		result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2]) * (float)dLamda);

		return result;
	}


	// Our axis of abitrary rotation
	Vec3f axis;

	if (fabs(fSin_a) < 0.0005f)
	{
		fSin_a = 1;
	}

	axis.x = fQuatNormalized[0] / fSin_a;
	axis.y = fQuatNormalized[1] / fSin_a;
	axis.z = fQuatNormalized[2] / fSin_a;

	// Get the portion of the angle to rotate by
	fAngle *= (float)dLamda;

	axis.normalize();

	// Calculate the quaternion for our new (partial) angle of rotation
	fSin_a = sin(fAngle * 0.5f);
	fCos_a = cos(fAngle * 0.5f);
	fQuatNormalized[0] = axis.x * fSin_a;
	fQuatNormalized[1] = axis.y * fSin_a;
	fQuatNormalized[2] = axis.z * fSin_a;
	fQuatNormalized[3] = fCos_a;

	fQuatMagnitude = sqrt(fQuatNormalized[0] * fQuatNormalized[0] + fQuatNormalized[1] * fQuatNormalized[1] + fQuatNormalized[2] * fQuatNormalized[2] + fQuatNormalized[3] * fQuatNormalized[3]);		
	fQuatNormalized[0] /= fQuatMagnitude;
	fQuatNormalized[1] /= fQuatMagnitude;
	fQuatNormalized[2] /= fQuatMagnitude;
	fQuatNormalized[3] /= fQuatMagnitude;

	// Calculate our partial rotation matrix
	float fXx	= fQuatNormalized[0] * fQuatNormalized[0];
	float fXy   = fQuatNormalized[0] * fQuatNormalized[1];
	float fXz   = fQuatNormalized[0] * fQuatNormalized[2];
	float fXw   = fQuatNormalized[0] * fQuatNormalized[3];
	float fYy   = fQuatNormalized[1] * fQuatNormalized[1];
	float fYz   = fQuatNormalized[1] * fQuatNormalized[2];
	float fYw   = fQuatNormalized[1] * fQuatNormalized[3];
	float fZz   = fQuatNormalized[2] * fQuatNormalized[2];
	float fZw   = fQuatNormalized[2] * fQuatNormalized[3];

	result.ma[0]  = 1 - 2 * (fYy + fZz);
	result.ma[1]  =     2 * (fXy - fZw);
	result.ma[2]  =     2 * (fXz + fYw);
	result.ma[4]  =     2 * (fXy + fZw);
	result.ma[5]  = 1 - 2 * (fXx + fZz);
	result.ma[6]  =     2 * (fYz - fXw);
	result.ma[8]  =     2 * (fXz - fYw);
	result.ma[9]  =     2 * (fYz + fXw);
	result.ma[10] = 1 - 2 * (fXx + fYy);
	result.ma[3]  = result.ma[7] = result.ma[11] = result.ma[12] = result.ma[13] = result.ma[14] = 0;
	result.ma[15] = 1;

	// Combine our partial rotation with MatrixA
	result *= MatrixA;

	// Linear interpolation of the translation components of the matrices
	result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0] - MatrixA.m[3][0]) * (float)dLamda);
	result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1] - MatrixA.m[3][1]) * (float)dLamda);
	result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2] - MatrixA.m[3][2]) * (float)dLamda);

	return result;
}*/

void OrthoNormalInverse(Matrix4f& matrixA, const Matrix4f& matrixB)
{
	matrixA = matrixB;
	matrixA.Xy = matrixB.Yx;
	matrixA.Xz = matrixB.Zx;
	matrixA.Yx = matrixB.Xy;
	matrixA.Yz = matrixB.Zy;
	matrixA.Zx = matrixB.Xz;
	matrixA.Zy = matrixB.Yz;

	matrixA.Wx = -dot_product(matrixA.position, matrixA.xAxis);
	matrixA.Wy = -dot_product(matrixA.position, matrixA.yAxis);
	matrixA.Wz = -dot_product(matrixA.position, matrixA.zAxis);
}