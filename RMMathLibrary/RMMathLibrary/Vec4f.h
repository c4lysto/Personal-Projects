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

	inline float magnitude() const
	{
#ifdef SSE_MATH_AVAILABLE
		__m128 vec = _mm_set_ps(x, y, z, w);

		vec = _mm_mul_ps(vec, vec);

		__m128 tmp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 0, 3, 2));

		vec = _mm_add_ps(vec, tmp);

		return sqrt(_mm_add_ss(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 0, 0))).m128_f32[0]);
#else
		return sqrt(x*x + y*y + z*z + w*w);
#endif
	}

	inline Vec4f& normalize()
	{
		float mag = magnitude();

		if(mag)
		{
			mag = 1 / mag;

#ifdef SSE_MATH_AVAILABLE
			*this = _mm_mul_ps(_mm_set_ps(x, y, z, w), _mm_set1_ps(mag));
#else
			x *= mag;
			y *= mag;
			z *= mag;
			w *= mag;
#endif
		}

		return *this;
	}

	inline Vec4f& zero_out()
	{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(color, _mm_setzero_ps());
#else
		x = y = z = w = 0;
#endif
		return *this;
	}

	inline Vec4f& full_color()
	{
#ifdef SSE_MATH_AVAILABLE
		_mm_storeu_ps(color, _mm_set1_ps(1.0f));
#else
		x = y = z = w = 1;
#endif
		return *this;
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