#ifndef VEC4F_H
#define VEC4F_H

#include "MathDeclarations.h"

#include "Matrix4f.h"

struct Vec4f
{
	union
	{
		float color[4];
		float vector[4];

		union
		{
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				Vec3f position; float padW;
			};
		};

		struct
		{
			float r, g, b, a;
		};
	};

	Vec4f() : r(0), g(0), b(0), a(0) {}

	Vec4f(const Vec4f&& vVector)
	{
		*this = move(vVector);
	}

	Vec4f(const XMVECTOR&& vVector)
	{
		*this = move(vVector);
	}

	Vec4f(float fR, float fG, float fB, float fA) : r(fR), g(fG), b(fB), a(fA) {}

	Vec4f(Vec3f vVector, float fA)
	{
		position = vVector;
		a = fA;
	}

	Vec4f& operator=(const Vec4f&& vVector)
	{
		if(this != &vVector)
		{
#ifdef SSE_MATH_AVAILABLE
			_mm_storeu_ps(color, _mm_loadu_ps(vVector.color));
#else
			x = vVector.x;
			y = vVector.y;
			z = vVector.z;
			w = vVector.w;
#endif
		}

		return *this;
	}

	Vec4f& operator=(const XMVECTOR&& vVector)
	{
		if(this != (Vec4f*)&vVector)
			XMStoreFloat4((XMFLOAT4*)this, vVector);

		return *this;
	}

	inline Vec4f operator-(const Vec4f& vVector) const
	{
		return XMVectorSubtract(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
	}

	inline Vec4f& operator-=(const Vec4f& vVector)
	{
		return *this = XMVectorSubtract(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
	}

	inline Vec4f operator+(const Vec4f& vVector) const
	{
		return XMVectorAdd(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
	}

	inline Vec4f& operator+=(const Vec4f& vVector)
	{
		return *this = XMVectorAdd(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4((XMFLOAT4*)&vVector));
	}

	inline Vec4f operator/(const float fScalar) const
	{
		return XMVectorDivide(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
	}

	inline Vec4f& operator/=(const float fScalar)
	{
		return *this = XMVectorDivide(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
	}

	inline Vec4f operator*(const float fScalar) const
	{
		return XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
	}

	inline Vec4f& operator*=(const float fScalar)
	{
		return *this = XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)this), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
	}

	inline Vec4f operator*(const Matrix4f& mMatrix) const
	{
		return XMVector4Transform(XMLoadFloat4((XMFLOAT4*)this), XMLoadFloat4x4((XMFLOAT4X4*)&mMatrix));
	}

	inline Vec4f& operator*=(const Matrix4f& mMatrix)
	{
		return (*this) = (*this) * mMatrix;
	}

	inline friend Vec4f operator*(const float fScalar, const Vec4f& vVector)
	{
		return XMVectorMultiply(XMLoadFloat4((XMFLOAT4*)&vVector), XMVectorSet(fScalar, fScalar, fScalar, fScalar));
	}

	inline Vec4f& zero_out()
	{
		return *this = XMVectorZero();
	}

	inline Vec4f& full_color()
	{
		return *this = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	}

	friend Vec4f Lerp(const Vec4f& vVectorA, const Vec4f& vVectorB, const float fLambda)
	{
		return vVectorA + (vVectorB - vVectorA) * fLambda;
	}
};

typedef Vec4f vec4f;
typedef Vec4f vec4;
typedef Vec4f float4;

#endif