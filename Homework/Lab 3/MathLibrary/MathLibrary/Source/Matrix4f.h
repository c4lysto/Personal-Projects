#ifndef	MATRIX4F_H
#define	MATRIX4F_H

#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "MathDeclarations.h"
#include "Vec3f.h"

struct Matrix4f
{	
	union
	{
#ifndef _XM_NO_INTRINSICS_
		float Xx, Xy, Xz, Xw,
				Yx, Yy, Yz, Yw,
				Zx, Zy, Zz, Zw,
				Wx, Wy, Wz, Ww;
#endif

		struct
		{
			Vec3f xAxis; float padXW;
			Vec3f yAxis; float padYW;
			Vec3f zAxis; float padZW;

			union
			{
				struct
				{
					Vec3f wAxis; float padWW;
				};

				struct
				{
					Vec3f position; float padWW;
				};
			};
		};

		struct
		{
			XMMATRIX matrix;
		};
	};

	Matrix4f() 
	{
		make_identity();
	}

	Matrix4f(const Matrix4f& mMatrix)
	{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
		matrix.r[0] = mMatrix.matrix.r[0];
		matrix.r[1] = mMatrix.matrix.r[1];
		matrix.r[2] = mMatrix.matrix.r[2];
		matrix.r[3] = mMatrix.matrix.r[3];
#else
		memcpy(this, &mMatrix, sizeof(Matrix4f));
#endif	
	}

	Matrix4f(const Matrix4f&& mMatrix) : matrix(std::move(mMatrix.matrix))
	{
	}

	Matrix4f(const XMMATRIX& mMatrix) : matrix(mMatrix){}
	
	~Matrix4f() 
	{
	}

	operator XMMATRIX*() {return &matrix;}
	operator const XMMATRIX*() const {return &matrix;}

	operator XMMATRIX&() {return matrix;}
	operator const XMMATRIX&() {return matrix;}

	inline Matrix4f& operator=(const Matrix4f& mMatrix) {matrix = mMatrix.matrix; return *this;}
	inline Matrix4f& operator=(const XMMATRIX& mMatrix) {matrix = mMatrix; return *this;}

	inline Matrix4f& operator=(const Matrix4f&& mMatrix) {matrix = std::move(mMatrix.matrix); return *this;}
	inline Matrix4f& operator=(const XMMATRIX&& mMatrix) {matrix = std::move(mMatrix); return *this;}

	inline Matrix4f operator*(const Matrix4f& mMatrix) const
	{
		Matrix4f tmp = XMMatrixMultiply(matrix, mMatrix.matrix);
		return tmp;
	}

	inline Matrix4f& operator*=(const Matrix4f& mMatrix)
	{
		matrix = XMMatrixMultiply(matrix, mMatrix.matrix);
		return *this;
	}

	Matrix4f& make_identity()
	{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
		matrix.r[0] = g_XMIdentityR0.v;
		matrix.r[1] = g_XMIdentityR1.v;
		matrix.r[2] = g_XMIdentityR2.v;
		matrix.r[3] = g_XMIdentityR3.v;
#else
		ZeroMemory(this, sizeof(Matrix4f));
		Xx = Yy = Zz = Ww = 1.0f;
#endif
		return *this;
	}

	Matrix4f& make_identity3x3()
	{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
		matrix.r[0] = _mm_set_ss(1.0f);
		matrix.r[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, Yw);
		matrix.r[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, Zw);
#else
		ZeroMemory(xAxis, sizeof(Vec3f));
		ZeroMemory(yAxis, sizeof(Vec3f));
		ZeroMemory(zAxis, sizeof(Vec3f));

		Xx = Yy = Zz = 1.0f;
#endif
		return *this;
	}

	inline Matrix4f& Rotate_GlobalX_Radians(float fRadians) {matrix *= XMMatrixRotationX(fRadians);	return *this;}
	inline Matrix4f& Rotate_GlobalY_Radians(float fRadians) {matrix *= XMMatrixRotationY(fRadians);	return *this;}
	inline Matrix4f& Rotate_GlobalZ_Radians(float fRadians) {matrix *= XMMatrixRotationZ(fRadians);	return *this;}

	inline Matrix4f& Rotate_GlobalX_Degrees(float fDegrees)
	{return Rotate_GlobalX_Radians(DEG_TO_RAD(fDegrees));}
	inline Matrix4f& Rotate_GlobalY_Degrees(float fDegrees)
	{return Rotate_GlobalY_Radians(DEG_TO_RAD(fDegrees));}
	inline Matrix4f& Rotate_GlobalZ_Degrees(float fDegrees)
	{return Rotate_GlobalZ_Radians(DEG_TO_RAD(fDegrees));}

	inline Matrix4f& Rotate_LocalX_Radians(float fRadians) {matrix = XMMatrixMultiply(XMMatrixRotationX(fRadians), matrix); return *this;}
	inline Matrix4f& Rotate_LocalY_Radians(float fRadians) {matrix = XMMatrixMultiply(XMMatrixRotationY(fRadians), matrix); return *this;}
	inline Matrix4f& Rotate_LocalZ_Radians(float fRadians) {matrix = XMMatrixMultiply(XMMatrixRotationZ(fRadians), matrix); return *this;}

	inline Matrix4f& Rotate_LocalX_Degrees(float fDegrees)
	{return Rotate_LocalX_Radians(DEG_TO_RAD(fDegrees));}
	inline Matrix4f& Rotate_LocalY_Degrees(float fDegrees)
	{return Rotate_LocalY_Radians(DEG_TO_RAD(fDegrees));}
	inline Matrix4f& Rotate_LocalZ_Degrees(float fDegrees)
	{return Rotate_LocalZ_Radians(DEG_TO_RAD(fDegrees));}

	Matrix4f& Scale(float fXScale, float fYScale, float fZScale);
	Matrix4f& Scale(const Vec3f& vScale);
	Matrix4f& SetScale(float fXScale, float fYScale, float fZScale);
	Matrix4f& SetScale(const Vec3f& vScale);
	inline Matrix4f& SetScale(float fScale) {return SetScale(fScale, fScale, fScale);}
	Vec3f GetScale() const;

	Matrix4f& Translate(float fX, float fY, float fZ);
	Matrix4f& Translate(Vec3f vTranslation);

	Matrix4f& MoveForward(float fMovement);
	Matrix4f& MoveBackward(float fMovement);
	Matrix4f& MoveLeft(float fMovement);
	Matrix4f& MoveRight(float fMovement);
	Matrix4f& MoveUp(float fMovement);
	Matrix4f& MoveDown(float fMovement);

	Matrix4f& Transpose();
	Matrix4f& Transpose3x3();
	Matrix4f& Negate();
	Matrix4f& Invert();

	Matrix4f& NormalizeXYZ();

	Matrix4f& LookAt(const Vec3f& mPos);

	Matrix4f& MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);

	friend void OrthoNormalInverse(Matrix4f& matrixA, const Matrix4f& matrixB);

	friend Matrix4f MatrixTranspose(const Matrix4f& mMatrix);
	friend Matrix4f MatrixTranspos3x3(const Matrix4f& mMatrix);

	friend Matrix4f MatrixScale(const Matrix4f& mMatrix, float fXScale, float fYScale, float fZScale);

	friend Matrix4f MatrixInverse(const Matrix4f& mMatrix);
	friend Matrix4f MatrixNegate(const Matrix4f& mMatrix);

	friend Matrix4f Interpolate(const Matrix4f& MatrixA, const Matrix4f& MatrixB, double dLamda);
};

typedef Matrix4f Matrix4f;
typedef Matrix4f float4x4;
typedef Matrix4f Matrix;

#endif