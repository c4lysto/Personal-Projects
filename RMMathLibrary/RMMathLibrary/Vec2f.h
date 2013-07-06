#ifndef VEC2F_H
#define VEC2F_H

#include "MathDeclarations.h"

struct Vec2f
{
	union
	{
		float pos[2];

		struct
		{
			float x, y;
		};
	};

	Vec2f() : x(0), y(0){}

	Vec2f(const Vec2f&& vVector)
	{
		*this = move(vVector);
	}

	Vec2f(const XMVECTOR&& vVector)
	{
		*this = move(vVector);
	}

	Vec2f(float fX, float fY) : x(fX), y(fY) {}

	inline Vec2f& operator=(const Vec2f&& vVector)
	{
		if(this != &vVector)
		{
			x = vVector.x;
			y = vVector.y;
		}
		
		return *this;
	}

	inline Vec2f& operator=(const POINT vVector)
	{
		x = (float)vVector.x;
		y = (float)vVector.y;

		return *this;
	}

	inline Vec2f& operator=(const XMVECTOR&& vVector)
	{
		if(this != (Vec2f*)&vVector)
			XMStoreFloat2((XMFLOAT2*)this, vVector);

		return *this;
	}

	inline Vec2f& operator=(const XMVECTOR& vVector)
	{
		if(this != (Vec2f*)&vVector)
			XMStoreFloat2((XMFLOAT2*)this, vVector);

		return *this;
	}

	inline Vec2f operator+(const Vec2f& vVector) const
	{
		Vec2f vec(*this);

		vec.x += vVector.x;
		vec.y += vVector.y;

		return vec;
	}

	inline Vec2f& operator+=(const Vec2f& vVector)
	{
		x += vVector.x;
		y += vVector.y;

		return *this;
	}

	inline Vec2f operator-(const Vec2f& vVector) const
	{
		Vec2f vec(*this);

		vec.x -= vVector.x;
		vec.y -= vVector.y;

		return vec;
	}

	inline Vec2f& operator-=(const Vec2f& vVector)
	{
		x -= vVector.x;
		y -= vVector.y;

		return *this;
	}

	inline friend Vec2f operator-(const Vec2f& vVector)
	{
		Vec2f vec(vVector);

		vec *= -1;

		return vec;
	}

	inline Vec2f operator*(const float fScalar) const
	{
		Vec2f vec(*this);

		vec.x *= fScalar;
		vec.y *= fScalar;

		return vec;
	}

	inline friend Vec2f operator*(float fScalar, const Vec2f& vVector)
	{
		return vVector * fScalar;
	}

	inline Vec2f& operator*=(const float fScalar)
	{
		x *= fScalar;
		y *= fScalar;

		return *this;
	}

	inline Vec2f operator/(const float fScalar) const
	{
		Vec2f vec(*this);

		vec.x /= fScalar;
		vec.y /= fScalar;

		return vec;
	}

	inline Vec2f& operator/=(const float fScalar)
	{
		x /= fScalar;
		y /= fScalar;

		return *this;
	}

	inline float dot_product(const Vec2f& vVector)
	{
		return (x * vVector.x) + (y * vVector.y);
	}

	inline Vec2f& Translate(float fX, float fY)
	{
		x += fX;
		y += fY;

		return *this;
	}

	inline float magnitude() const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	inline float sq_magnitude() const
	{
		return pow(x, 2) + pow(y, 2);
	}

	inline Vec2f& normalize()
	{
		float mag = magnitude();

		if(mag)
		{
			mag = 1 / mag;

			x *= mag;
			y *= mag;
		}

		return *this;
	}

	inline float AngleBetween(const Vec2f& vVector)
	{
		return XMVector2AngleBetweenVectors(XMLoadFloat2((XMFLOAT2*)this), XMLoadFloat2((XMFLOAT2*)&vVector)).m128_f32[0];
	}
};

typedef Vec2f vec2f;
typedef Vec2f vec2;
typedef Vec3f float2;

#endif