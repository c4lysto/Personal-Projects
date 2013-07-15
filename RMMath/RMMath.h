#include "MathDeclarations.h"

#ifndef RMMATH_H
#define RMMATH_H

namespace RMMath
{
#pragma region Vec2f Definition
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

	Vec2f();
	Vec2f(const Vec2f&& vVector);
	Vec2f(const XMVECTOR&& vVector);
	Vec2f(float fX, float fY);

	inline Vec2f& operator=(const Vec2f&& vVector);
	inline Vec2f& operator=(const POINT vVector);
	inline Vec2f& operator=(const XMVECTOR&& vVector);
	inline Vec2f& operator=(const XMVECTOR& vVector);

	inline Vec2f operator+(const Vec2f& vVector) const;
	inline Vec2f& operator+=(const Vec2f& vVector);

	inline Vec2f operator-(const Vec2f& vVector) const;
	inline Vec2f& operator-=(const Vec2f& vVector);

	inline Vec2f operator*(const float fScalar) const;
	inline Vec2f& operator*=(const float fScalar);

	inline Vec2f operator/(const float fScalar) const;
	inline Vec2f& operator/=(const float fScalar);

	inline float dot_product(const Vec2f& vVector) const;

	inline float magnitude() const;
	inline float length() const;

	inline float sq_magnitude() const;
	inline float sq_length() const;

	inline Vec2f& normalize();

	inline float angle_between(const Vec2f& vVector) const;
};

inline Vec2f operator-(const Vec2f& vVector);
inline Vec2f operator*(float fScalar, const Vec2f& vVector);
inline Vec2f Normalize(const Vec2f& vVector);
inline float Dot_Product(const Vec2f& vVectorA, const Vec2f& vVectorB);
inline Vec2f Lerp(const Vec2f& vVectorA, const Vec2f& vVectorB, const float fLambda);
inline float AngleBetween(const Vec2f& vVectorA, const Vec2f& vVectorB);

typedef Vec2f vec2f;
typedef Vec2f vec2;
typedef Vec2f float2;
#pragma endregion

#pragma region Vec3f Definition
struct Vec3f
{
	union
	{
		float v[3];

		struct
		{
			float x, y, z;
		};
	};

	Vec3f();
	Vec3f(float fX, float fY, float fZ);
	Vec3f(float val);

	Vec3f(const Vec3f&& vVector);

	Vec3f(const XMVECTOR&& vVector);

	inline Vec3f& operator=(const Vec3f&& vVector);
	inline Vec3f& operator=(const XMVECTOR&& vVector);
	inline Vec3f& operator=(const XMVECTOR& vVector);
	inline Vec3f& operator=(const float fScalar);

	inline Vec3f& operator*=(const float fScalar);
	inline Vec3f& operator*=(const Vec3f& vScale);

	inline Vec3f operator/(const float fScalar) const;
	inline Vec3f operator/(const Vec3f& vScale) const;

	inline Vec3f& operator/=(const float fScalar);
	inline Vec3f& operator/=(const Vec3f& vScale);

	inline Vec3f operator*(const float fScalar) const;
	inline Vec3f operator*(const Vec3f& vScale) const;

	inline Vec3f& operator+=(const Vec3f& vVector);
	inline Vec3f operator+(const Vec3f& vVector) const;

	inline Vec3f& operator-=(const Vec3f& vVector);
	inline Vec3f operator-(const Vec3f& vVector) const;

	inline bool operator==(const Vec3f& vVector) const;

	inline bool operator!=(const Vec3f& vVector) const;

	inline Vec3f& zero_out();

	inline Vec3f& normalize();		

	inline Vec3f& negate();

	inline float magnitude() const;
	inline float length() const;

	inline float sq_magnitude() const;
	inline float sq_length() const;

	inline float dot_product(const Vec3f& vVectorB) const;

	inline Vec3f cross_product(const Vec3f& vVectorB) const;
};

inline Vec3f operator*(const float fScalar, const Vec3f& vVector);
inline Vec3f operator-(const Vec3f& vVector);

inline Vec3f Normalize(const Vec3f& vVector);
inline float Dot_Product(const Vec3f& vVectorA, const Vec3f& vVectorB);
inline Vec3f Cross_Product(const Vec3f& vVectorA, const Vec3f& vVectorB);
inline Vec3f Lerp(const Vec3f& vVectorA, const Vec3f& vVectorB, const float fLambda);

typedef Vec3f float3;
typedef Vec3f vec3f;
typedef Vec3f vec3;

extern const __declspec(selectany) Vec3f g_WorldUp = Vec3f(0.0f, 1.0f, 0.0f);
#pragma endregion

#pragma region Vec4f Definition
struct Matrix4f;

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

	Vec4f();
	Vec4f(const Vec4f&& vVector);
	Vec4f(const XMVECTOR&& vVector);
	Vec4f(float fR, float fG, float fB, float fA);
	Vec4f(Vec3f vVector, float fA);

	inline Vec4f& operator=(const Vec4f&& vVector);
	inline Vec4f& operator=(const XMVECTOR&& vVector);

	inline Vec4f operator-(const Vec4f& vVector) const;
	inline Vec4f& operator-=(const Vec4f& vVector);

	inline Vec4f operator+(const Vec4f& vVector) const;
	inline Vec4f& operator+=(const Vec4f& vVector);

	inline Vec4f operator/(const float fScalar) const;
	inline Vec4f& operator/=(const float fScalar);

	inline Vec4f operator*(const float fScalar) const;
	inline Vec4f& operator*=(const float fScalar);
	inline Vec4f operator*(const Matrix4f& mMatrix) const;
	inline Vec4f& operator*=(const Matrix4f& mMatrix);

	inline float dot_product(const Vec4f& vVector) const;

	inline float magnitude() const;
	inline float length() const;

	inline float sq_magnitude() const;
	inline float sq_length() const;

	inline Vec4f& normalize();

	inline Vec4f& zero_out();

	inline Vec4f& full_color();
};

inline Vec4f operator*(const float fScalar, const Vec4f& vVector);
inline Vec4f operator-(const Vec4f& vVector);

inline Vec4f Normalize(const Vec4f& vVector);
inline float Dot_Product(const Vec4f& vVectorA, const Vec4f& vVectorB);
inline Vec4f Lerp(const Vec4f& vVectorA, const Vec4f& vVectorB, const float fLambda);

typedef Vec4f vec4f;
typedef Vec4f vec4;
typedef Vec4f float4;
#pragma endregion

#pragma region Matrix3f
struct Matrix3f
{
	union
	{
		float m[9];
		float ma[3][3];

		struct
		{
			float Xx, Xy, Xz,
				  Yx, Yy, Yz,
				  Zx, Zy, Zz;
		};

		struct
		{
			Vec3f xAxis,
				  yAxis,
				  zAxis;
		};
	};

	Matrix3f();
	Matrix3f(float fXx, float fXy, float fXz, 
			 float fYx, float fYy, float fYz, 
			 float fZx, float fZy, float fZz);
	Matrix3f(const Matrix3f&& mMatrix);
	Matrix3f(const XMMATRIX&& mMatrix);
	Matrix3f(const Vec3f& vXAxis,
			 const Vec3f& vYAxis,
			 const Vec3f& vZAxis);

	inline Matrix3f& make_identity();

	inline Matrix3f& operator=(const Matrix3f&& mMatrix);
	inline Matrix3f& operator=(const XMMATRIX&& mMatrix);

	inline Matrix3f operator*(const Matrix3f& mMatrix) const;
	inline Matrix3f& operator*=(const Matrix3f& mMatrix);

	inline Matrix3f& Transpose();
};

typedef Matrix3f Matrix33;
typedef Matrix3f Mat3f;
typedef Matrix3f float3x3;

#pragma endregion

#pragma region Matrix4f Definition
struct Matrix4f
{
	union
	{
		float m[16];
		float ma[4][4];

		struct
		{
			float Xx, Xy, Xz, Xw,
				  Yx, Yy, Yz, Yw,
				  Zx, Zy, Zz, Zw,
				  Wx, Wy, Wz, Ww;
		};

		struct
		{
			Vec3f xAxis; float padXW;
			Vec3f yAxis; float padYW;
			Vec3f zAxis; float padZW;

			union
			{
				struct
				{
					Vec3f wAxis;
				};

				struct
				{
					Vec3f position;
				};
			};

			float padWW;
		};
	};

	// Matrix is always initialized to an identity matrix
	Matrix4f();
	Matrix4f(float fXx, float fXy, float fXz, float fXw,
		float fYx, float fYy, float fYz, float fYw,
		float fZx, float fZy, float fZz, float fZw,
		float fWx, float fWy, float fWz, float fWw);
	Matrix4f(const Matrix4f&& mMatrix);
	Matrix4f(const XMMATRIX&& mMatrix);
	Matrix4f(const Vec4f&& vXAxis,
			 const Vec4f&& vYAxis,
			 const Vec4f&& vZAxis,
			 const Vec4f&& vWAxis);

	inline XMMATRIX toXMMatrix();
	inline Matrix3f Get3x3();

	inline float operator[](size_t ucIndex) const;

	inline Matrix4f& make_identity();
	inline Matrix4f& make_identity_3x3();

	inline Matrix4f& operator=(const Matrix4f&& mMatrix);
	inline Matrix4f& operator=(const XMMATRIX&& mMatrix);

	inline Matrix4f operator*(const Matrix4f& mMatrix) const;
	inline Matrix4f& operator*=(const Matrix4f& mMatrix);

	// actually faster than DirectX Version :)
	inline Matrix4f& Rotate_GlobalX_Radians(float fRadians);
	inline Matrix4f& Rotate_GlobalY_Radians(float fRadians);
	inline Matrix4f& Rotate_GlobalZ_Radians(float fRadians);

	inline Matrix4f& Rotate_GlobalX_Degrees(float fDegrees);
	inline Matrix4f& Rotate_GlobalY_Degrees(float fDegrees);
	inline Matrix4f& Rotate_GlobalZ_Degrees(float fDegrees);

	inline Matrix4f& Rotate_LocalX_Radians(float fRadians);
	inline Matrix4f& Rotate_LocalY_Radians(float fRadians);
	inline Matrix4f& Rotate_LocalZ_Radians(float fRadians);

	inline Matrix4f& Rotate_LocalX_Degrees(float fDegrees);
	inline Matrix4f& Rotate_LocalY_Degrees(float fDegrees);
	inline Matrix4f& Rotate_LocalZ_Degrees(float fDegrees);

	inline Matrix4f& Scale(float fXScale, float fYScale, float fZScale);
	inline Matrix4f& Scale(const Vec3f& vScale);

	inline Matrix4f& SetScale(float fXScale, float fYScale, float fZScale);
	inline Matrix4f& SetScale(const Vec3f& vScale);
	inline Matrix4f& SetScale(float fScale);
	inline Vec3f GetScale() const;

	inline Matrix4f& Translate(float fX, float fY, float fZ);
	inline Matrix4f& Translate(Vec3f vTranslation);

	inline Matrix4f& MoveForward(float fMovement);
	inline Matrix4f& MoveBackward(float fMovement);
	inline Matrix4f& MoveLeft(float fMovement);
	inline Matrix4f& MoveRight(float fMovement);
	inline Matrix4f& MoveUp(float fMovement);
	inline Matrix4f& MoveDown(float fMovement);

	inline Matrix4f& Transpose();
	inline Matrix4f& Transpose3x3();

	inline Matrix4f& NormalizeXYZ();

	inline Matrix4f& Invert();

	inline Matrix4f& LookAt(const Vec3f& mPos);

	inline Matrix4f& TurnTo(const Vec3f& vPos, float fTurnModifier = 1.0f);

	inline Matrix4f& MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);

	inline Matrix4f& MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar);

	inline Matrix4f& OrthoNormalInvert();
};

inline Matrix4f MatrixTranspose(const Matrix4f& mMatrix);
inline Matrix4f MatrixTranspos3x3(const Matrix4f& mMatrix);
inline Matrix4f MatrixScale(const Matrix4f& mMatrix, float fXScale, float fYScale, float fZScale);
inline Matrix4f MatrixInverse(const Matrix4f& mMatrix);
inline Matrix4f Lerp(const Matrix4f& MatrixA, const Matrix4f& MatrixB, float fLambda);

typedef Matrix4f Matrix44;
typedef Matrix4f Mat4f;
typedef Matrix4f float4x4;
#pragma endregion

#include "Vec2f.inl"
#include "Vec3f.inl"
#include "Vec4f.inl"
#include "Matrix3f.inl"
#include "Matrix4f.inl"
};

#endif // RMMATH_H