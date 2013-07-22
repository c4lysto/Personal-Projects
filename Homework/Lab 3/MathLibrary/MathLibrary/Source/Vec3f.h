#ifndef VEC3F_H
#define VEC3F_H

#include <math.h>
#include <limits>
#include <utility>

#include <DirectXMath.h>
using namespace DirectX;

struct Vec3f
{
	union
	{
		float vector[3];

		struct
		{
			float x, y, z;
		};
	};


	Vec3f()
	{
		ZeroMemory(this, sizeof(Vec3f));
	}

	Vec3f(const Vec3f& vec)
	{
		memcpy(this, &vec, sizeof(Vec3f));
	}

	Vec3f(const Vec3f&& vec)
	{
		*this = std::move(vec);
	}

	Vec3f(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ){}

	operator XMFLOAT3*() {return (XMFLOAT3*)this;}
	operator const XMFLOAT3*() const {return (const XMFLOAT3*)this;}

	inline Vec3f& operator=(const Vec3f& vVector)
	{
		if(this != &vVector)
			memcpy(this, &vVector, sizeof(Vec3f));

		return *this;
	}

	inline Vec3f& operator=(const Vec3f&& vVector)
	{
		if(this != &vVector)
		{
			x = std::move(vVector.x);
			y = std::move(vVector.y);
			z = std::move(vVector.z);
		}

		return *this;
	}

	inline Vec3f& operator=(const float fScalar)
	{
		x = y = z = fScalar;

		return *this;
	}

	inline Vec3f& operator*=(const float fScalar)
	{
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;

		return *this;
	}

	inline Vec3f& operator*=(const Vec3f vScale)
	{
		x *= vScale.x;
		y *= vScale.y;
		z *= vScale.z;

		return *this;
	}

	inline Vec3f operator/(const float fScalar) const
	{
		Vec3f tmp(*this);

		tmp.x /= fScalar;
		tmp.y /= fScalar;
		tmp.z /= fScalar;

		return tmp;
	}

	inline Vec3f operator/(const Vec3f vScale) const
	{
		Vec3f tmp(*this);

		tmp.x /= vScale.x;
		tmp.y /= vScale.y;
		tmp.z /= vScale.z;

		return tmp;
	}

	inline Vec3f& operator/=(const float fScalar)
	{
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;

		return *this;
	}

	inline Vec3f& operator/=(const Vec3f vScale)
	{
		x /= vScale.x;
		y /= vScale.y;
		z /= vScale.z;

		return *this;
	}

	inline Vec3f operator*(const float fScalar) const
	{
		Vec3f tmp(*this);

		tmp.x *= fScalar;
		tmp.y *= fScalar;
		tmp.z *= fScalar;

		return tmp;
	}

	inline Vec3f operator*(const Vec3f vScale) const
	{
		Vec3f tmp(*this);

		tmp.x *= vScale.x;
		tmp.y *= vScale.y;
		tmp.z *= vScale.z;

		return tmp;
	}

	inline friend Vec3f operator*(const float fScalar, const Vec3f& vVector)
	{
		Vec3f tmp(vVector);

		tmp.x *= fScalar;
		tmp.y *= fScalar;
		tmp.z *= fScalar;

		return tmp;
	}

	inline Vec3f& operator+=(const Vec3f& vVector)
	{
		x += vVector.x;
		y += vVector.y;
		z += vVector.z;

		return *this;
	}

	inline Vec3f& operator+=(float fScalar)
	{
		x -= fScalar;
		y -= fScalar;
		z -= fScalar;

		return *this;
	}

	inline Vec3f operator+(const Vec3f& vVector) const
	{
		Vec3f tmp(*this);

		tmp.x += vVector.x;
		tmp.y += vVector.y;
		tmp.z += vVector.z;

		return tmp;
	}

	inline Vec3f operator+(const float fScalar) const
	{
		Vec3f tmp(*this);

		tmp.x += fScalar;
		tmp.y += fScalar;
		tmp.z += fScalar;

		return tmp;
	}

	inline Vec3f& operator-=(const Vec3f& vVector)
	{
		x -= vVector.x;
		y -= vVector.y;
		z -= vVector.z;

		return *this;
	}

	inline Vec3f& operator-=(const float fScalar)
	{
		x -= fScalar;
		y -= fScalar;
		z -= fScalar;

		return *this;
	}

	inline Vec3f operator-(const Vec3f& vVector) const
	{
		Vec3f tmp(*this);

		tmp.x -= vVector.x;
		tmp.y -= vVector.y;
		tmp.z -= vVector.z;

		return tmp;
	}

	inline Vec3f operator-(const float fScalar) const
	{
		Vec3f tmp(*this);

		tmp.x -= fScalar;
		tmp.y -= fScalar;
		tmp.z -= fScalar;

		return tmp;
	}

	friend Vec3f operator-(const Vec3f& vVector)
	{
		return vVector * -1.0f;
	}

	inline bool operator==(const Vec3f& vVector) const
	{
		return !memcmp(this, &vVector, sizeof(Vec3f));
	}

	inline bool operator!=(const Vec3f& vVector) const
	{
		return !(*this == vVector);
	}

	inline bool operator>(const Vec3f& vVector) const
	{
		return (sq_magnitude() > vVector.sq_magnitude()) ? true : false;
	}

	inline bool operator>=(const Vec3f& vVector) const
	{
		return (sq_magnitude() >= vVector.sq_magnitude()) ? true : false;
	}

	inline bool operator<(const Vec3f& vVector) const
	{
		return (sq_magnitude() < vVector.sq_magnitude()) ? true : false;
	}

	inline bool operator<=(const Vec3f& vVector) const
	{
		return (sq_magnitude() <= vVector.sq_magnitude()) ? true : false;
	}


	inline bool operator>(const float fMagnitude) const
	{
		return (sq_magnitude() > (fMagnitude * fMagnitude)) ? true : false;
	}

	inline bool operator>=(const float fMagnitude) const
	{
		return (sq_magnitude() >= (fMagnitude * fMagnitude)) ? true : false;
	}

	inline bool operator<(const float fMagnitude) const
	{
		return (sq_magnitude() < (fMagnitude * fMagnitude)) ? true : false;
	}

	inline bool operator<=(const float fMagnitude) const
	{
		return (sq_magnitude() <= (fMagnitude * fMagnitude)) ? true : false;
	}


	inline friend bool operator>(float fMagnitude, const Vec3f& vVector)
	{
		return ((fMagnitude * fMagnitude) > vVector.sq_magnitude()) ? true : false;
	}

	inline friend bool operator>=(float fMagnitude, const Vec3f& vVector)
	{
		return ((fMagnitude * fMagnitude) >= vVector.sq_magnitude()) ? true : false;
	}

	inline friend bool operator<(float fMagnitude, const Vec3f& vVector)
	{
		return ((fMagnitude * fMagnitude) < vVector.sq_magnitude()) ? true : false;
	}

	inline friend bool operator<=(float fMagnitude, const Vec3f& vVector)
	{
		return ((fMagnitude * fMagnitude) <= vVector.sq_magnitude()) ? true : false;
	}

	inline Vec3f& zero_out()
	{
		x = y = z = 0;
		return *this;
	}

	inline Vec3f& normalize()
	{
		float mag = 1 / magnitude();

		// protection against divide by zero
		if(mag != std::numeric_limits<float>::infinity())
		{
			x *= mag;
			y *= mag;
			z *= mag;
		}

		return *this;
	}

	inline Vec3f& negate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	inline float magnitude() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	inline float sq_magnitude() const 
	{
		return x*x + y*y + z*z;
	}

	inline float dot_product(const Vec3f& vVectorB) const
	{
		return x * vVectorB.x + y * vVectorB.y + z * vVectorB.z;
	}

	inline friend float dot_product(const Vec3f& vVectorA, const Vec3f& vVectorB)
	{
		return vVectorA.x * vVectorB.x + vVectorA.y * vVectorB.y + vVectorA.z * vVectorB.z;
	}

	inline Vec3f& cross_product(const Vec3f& vVectorB)
	{
		Vec3f vec(*this);
		x = vec.y * vVectorB.z - vec.z * vVectorB.y;
		y = vec.z * vVectorB.x - vec.x * vVectorB.z;
		z = vec.x * vVectorB.y - vec.y * vVectorB.x;
		
		return *this;
	}

	inline friend Vec3f cross_product(const Vec3f& vVectorA, const Vec3f vVectorB)
	{
		Vec3f vec;
		vec.x = vVectorA.y * vVectorB.z - vVectorA.z * vVectorB.y;
		vec.y = vVectorA.z * vVectorB.x - vVectorA.x * vVectorB.z;
		vec.z = vVectorA.x * vVectorB.y - vVectorA.y * vVectorB.x;

		return vec;
	}

	friend Vec3f Lerp(const Vec3f& vVectorA, const Vec3f& vVectorB, const float fLambda)
	{
		return vVectorA + (vVectorB - vVectorA) * fLambda;
	}
};

typedef Vec3f float3;
typedef Vec3f vec3;
typedef Vec3f vec3f;

#endif