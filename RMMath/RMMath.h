#include "MathDeclarations.h"

#ifndef RMMATH_H
#define RMMATH_H

namespace RMMath
{
#pragma region Unaligned Math Structures
#pragma region Vec2 Definition
template<typename Type>
struct Vec2
{
	union
	{
		float pos[2];

		struct
		{
			float x, y;
		};
	};

	Vec2();
	Vec2(const Vec2<Type>& vVector);
	Vec2(Vec2<Type>&& vVector);
	Vec2(Type fX, Type fY);

	inline Vec2<Type> operator-();

	inline Vec2<Type>& operator=(const Vec2<Type>& vVector);
	inline Vec2<Type>& operator=(Vec2<Type>&& vVector);
	inline Vec2<Type>& operator=(const POINT vVector);
	inline Vec2<Type>& operator=(XMVECTOR&& vVector);
	inline Vec2<Type>& operator=(const XMVECTOR& vVector);

	inline Vec2<Type> operator+(const Vec2<Type>& vVector) const;
	inline Vec2<Type>& operator+=(const Vec2<Type> & vVector);

	inline Vec2<Type> operator-(const Vec2<Type>& vVector) const;
	inline Vec2<Type>& operator-=(const Vec2<Type>& vVector);

	inline Vec2<Type> operator*(float fScalar) const;
	inline Vec2<Type>& operator*=(float fScalar);

	inline Vec2<Type> operator/(float fScalar) const;
	inline Vec2<Type>& operator/=(float fScalar);

	inline float dot_product(const Vec2& vVector) const;

	inline float magnitude() const;
	inline float length() const;

	inline float sq_magnitude() const;
	inline float sq_length() const;

	inline Vec2& normalize();

	inline float angle_between(const Vec2& vVector) const;
};

template<typename Type>
inline Vec2<Type> operator*(float fScalar, const Vec2<Type>& vVector);

template<typename Type>
inline Vec2<Type> Normalize(const Vec2<Type>& vVector);

template<typename Type>
inline float DotProduct(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB);

template<typename Type>
inline Vec2<Type> Lerp(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB, const float fLambda);

template<typename Type>
inline float AngleBetween(const Vec2<Type>& vVectorA, const Vec2<Type>& vVectorB);

typedef Vec2<float> Vec2f;
typedef Vec2f float2;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2ui;
typedef Vec2<short> Vec2s;
typedef Vec2<unsigned short> Vec2us;
#pragma endregion

#pragma region Vec3f Definition

struct Matrix3f;

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

	Vec3f(const Vec3f& vVector);
	Vec3f(Vec3f&& vVector);

	Vec3f(XMVECTOR&& vVector);

	inline Vec3f operator-();

	inline Vec3f& operator=(const Vec3f& vVector);
	inline Vec3f& operator=(Vec3f&& vVector);
	inline Vec3f& operator=(XMVECTOR&& vVector);
	inline Vec3f& operator=(const XMVECTOR& vVector);

	inline Vec3f& operator*=(float fScalar);
	inline Vec3f& operator*=(const Vec3f& vScale);

	inline Vec3f operator*(const Matrix3f& mMatrix);
	inline Vec3f& operator*=(const Matrix3f& mMatrix);

	inline Vec3f operator/(float fScalar) const;
	inline Vec3f operator/(const Vec3f& vScale) const;

	inline Vec3f& operator/=(float fScalar);
	inline Vec3f& operator/=(const Vec3f& vScale);

	inline Vec3f operator*(float fScalar) const;
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

inline Vec3f Normalize(const Vec3f& vVector);
inline float DotProduct(const Vec3f& vVectorA, const Vec3f& vVectorB);
inline Vec3f CrossProduct(const Vec3f& vVectorA, const Vec3f& vVectorB);
inline Vec3f Lerp(const Vec3f& vVectorA, const Vec3f& vVectorB, const float fLambda);

typedef Vec3f float3;
typedef Vec3f vec3f;
typedef Vec3f vec3;

extern const __declspec(selectany) Vec3f g_IdentityX3 = Vec3f(1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec3f g_IdentityY3 = Vec3f(0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec3f g_IdentityZ3 = Vec3f(0.0f, 0.0f, 1.0f);
extern const __declspec(selectany) Vec3f g_WorldUp = g_IdentityY3;
extern const __declspec(selectany) Vec3f g_ZeroVec3 = Vec3f(0.0f, 0.0f, 0.0f);
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
	Vec4f(const Vec4f& vVector);
	Vec4f(Vec4f&& vVector);
	Vec4f(XMVECTOR&& vVector);
	Vec4f(float fR, float fG, float fB, float fA);
	Vec4f(const Vec3f& vVector, float fA);

	inline Vec4f operator-();

	inline Vec4f& operator=(Vec4f&& vVector);
	inline Vec4f& operator=(XMVECTOR&& vVector);

	inline Vec4f operator-(const Vec4f& vVector) const;
	inline Vec4f& operator-=(const Vec4f& vVector);

	inline Vec4f operator+(const Vec4f& vVector) const;
	inline Vec4f& operator+=(const Vec4f& vVector);

	inline Vec4f operator/(float fScalar) const;
	inline Vec4f& operator/=(float fScalar);

	inline Vec4f operator*(float fScalar) const;
	inline Vec4f& operator*=(float fScalar);
	inline Vec4f operator*(const Vec4f& vVector) const;
	inline Vec4f& operator*=(const Vec4f& vVector);
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

inline Vec4f Normalize(const Vec4f& vVector);
inline float DotProduct(const Vec4f& vVectorA, const Vec4f& vVectorB);
inline Vec4f Lerp(const Vec4f& vVectorA, const Vec4f& vVectorB, const float fLambda);

typedef Vec4f vec4f;
typedef Vec4f vec4;
typedef Vec4f float4;

#ifdef SSE_MATH_AVAILABLE
extern const __declspec(selectany) __m128 g_IdentityX4 = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
extern const __declspec(selectany) __m128 g_IdentityY4 = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) __m128 g_IdentityZ4 = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) __m128 g_IdentityW4 = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
extern const __declspec(selectany) __m128 g_ZeroVec4 = _mm_setzero_ps();
#else
extern const __declspec(selectany) Vec4f g_IdentityX4 = Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityY4 = Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityZ4 = Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
extern const __declspec(selectany) Vec4f g_IdentityW4 = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
extern const __declspec(selectany) Vec4f g_ZeroVec4 = Vec4f();
#endif
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
	Matrix3f(const Matrix3f& mMatrix);
	Matrix3f(Matrix3f&& mMatrix);
	Matrix3f(XMMATRIX&& mMatrix);
	Matrix3f(const Vec3f& vXAxis,
			 const Vec3f& vYAxis,
			 const Vec3f& vZAxis);

	inline Matrix3f& make_identity();

	inline Matrix3f& operator=(const Matrix3f& mMatrix);
	inline Matrix3f& operator=(Matrix3f&& mMatrix);
	inline Matrix3f& operator=(XMMATRIX&& mMatrix);

	inline Matrix3f operator*(const Matrix3f& mMatrix) const;
	inline Matrix3f& operator*=(const Matrix3f& mMatrix);

	inline Matrix3f& Transpose();
};

typedef Matrix3f Matrix33;
typedef Matrix3f Mat3f;
typedef Matrix3f float3x3;

#pragma endregion

#pragma region Matrix34f
struct Matrix34f
{
	union
	{
		float m[12];
		float ma[3][4];

		struct
		{
			float Xx, Xy, Xz,
				  Yx, Yy, Yz,
				  Zx, Zy, Zz,
				  Wx, Wy, Wz;
		};

		struct
		{
			Vec3f xAxis, 
				  yAxis, 
				  zAxis, 
				  wAxis;
		};
	};

	Matrix34f();
	Matrix34f(float fXx, float fXy, float fXz, 
			  float fYx, float fYy, float fYz, 
			  float fZx, float fZy, float fZz,
			  float fWx, float fWy, float fWz);
	Matrix34f(const Matrix34f& mMatrix);
	Matrix34f(Matrix34f&& mMatrix);
	Matrix34f(XMMATRIX&& mMatrix);
	Matrix34f(const Vec3f& vXAxis,
			  const Vec3f& vYAxis,
			  const Vec3f& vZAxis,
			  const Vec3f& vWAxis);

	inline Matrix34f& make_identity();
};

typedef Matrix34f Matrix34;
typedef Matrix34f float3x4;
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
	Matrix4f(const Matrix4f& mMatrix);
	Matrix4f(Matrix4f&& mMatrix);
	Matrix4f(XMMATRIX&& mMatrix);
	Matrix4f(const Vec4f& vXAxis,
			 const Vec4f& vYAxis,
			 const Vec4f& vZAxis,
			 const Vec4f& vWAxis);

	inline XMMATRIX toXMMatrix();
	inline Matrix3f Get3x3();

	inline Matrix4f& make_identity();
	inline Matrix4f& make_identity_3x3();

	inline Matrix4f& operator=(const Matrix4f& mMatrix);
	inline Matrix4f& operator=(Matrix4f&& mMatrix);
	inline Matrix4f& operator=(const XMMATRIX& mMatrix);
	inline Matrix4f& operator=(XMMATRIX&& mMatrix);

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

	inline Matrix4f& LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	inline Matrix4f& TurnTo(const Vec3f& vPos, float fTurnModifier = 1.0f);

	inline Matrix4f& MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);

	inline Matrix4f& MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar);

	inline Matrix4f& OrthoNormalInvert();

	inline Matrix4f& MakeTextureMatrixOffsetLH(unsigned int unWidth, unsigned int unHeight);
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
#pragma endregion

#ifdef SSE_MATH_AVAILABLE
#pragma region Aligned Math Structures

#pragma region Vec4fA
struct Matrix4fA;

__declspec(align(16)) struct Vec4fA
{
	union
	{
		float color[4];
		float vector[4];

		struct
		{
			__m128 row;
		};

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

	Vec4fA();
	Vec4fA(const Vec4fA& vVector);
	Vec4fA(Vec4fA&& vVector);
	Vec4fA(__m128&& vVector);
	Vec4fA(float fR, float fG, float fB, float fA);
	Vec4fA(const Vec4f& vVector);
	Vec4fA(Vec3f vVector, float fA);

	inline Vec4fA operator-();

	inline Vec4fA operator=(const Vec4fA& vVector);
	inline Vec4fA& operator=(Vec4fA&& vVector);
	inline Vec4fA& operator=(__m128&& vVector);

	inline Vec4fA operator-(const Vec4fA& vVector) const;
	inline Vec4fA& operator-=(const Vec4fA& vVector);

	inline Vec4fA operator+(const Vec4fA& vVector) const;
	inline Vec4fA& operator+=(const Vec4fA& vVector);

	inline Vec4fA operator/(float fScalar) const;
	inline Vec4fA& operator/=(float fScalar);

	inline Vec4fA operator*(float fScalar) const;
	inline Vec4fA& operator*=(float fScalar);
	inline Vec4fA operator*(const Vec4fA& vVector) const;
	inline Vec4fA& operator*=(const Vec4fA& vVector);
	inline Vec4fA operator*(const Matrix4fA& mMatrix) const;
	inline Vec4fA& operator*=(const Matrix4fA& mMatrix);

	inline float dot_product(const Vec4fA& vVector) const;

	inline float magnitude() const;
	inline float length() const;

	inline float sq_magnitude() const;
	inline float sq_length() const;

	inline Vec4fA& normalize();

	inline Vec4fA& zero_out();

	inline Vec4fA& full_color();
};

inline Vec4fA operator*(const float fScalar, const Vec4fA& vVector);

inline Vec4fA Normalize(const Vec4fA& vVector);
inline float DotProduct(const Vec4fA& vVectorA, const Vec4fA& vVectorB);
inline Vec4fA Lerp(const Vec4fA& vVectorA, const Vec4fA& vVectorB, const float fLambda);

typedef Vec4fA vec4fA;
typedef Vec4fA vec4A;
typedef Vec4fA float4A;
#pragma endregion

#pragma region Matrix4fA Definition
__declspec(align(16)) struct Matrix4fA
{
	union
	{
		float m[16];
		float ma[4][4];

		struct
		{
			__m128 row1, row2, row3, row4;
		};

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
	Matrix4fA();
	Matrix4fA(float fXx, float fXy, float fXz, float fXw,
		float fYx, float fYy, float fYz, float fYw,
		float fZx, float fZy, float fZz, float fZw,
		float fWx, float fWy, float fWz, float fWw);
	Matrix4fA(const Matrix4f& mMatrix);
	Matrix4fA(const Matrix4fA& mMatrix);
	Matrix4fA(Matrix4fA&& mMatrix);
	Matrix4fA(XMMATRIX&& mMatrix);
	Matrix4fA(const Vec4fA& vXAxis,
			  const Vec4fA& vYAxis,
			  const Vec4fA& vZAxis,
			  const Vec4fA& vWAxis);

	inline XMMATRIX toXMMatrix() const;
	inline Matrix3f Get3x3() const;

	inline Matrix4fA& make_identity();
	inline Matrix4fA& make_identity_3x3();

	inline Matrix4fA& operator=(Matrix4fA&& mMatrix);
	inline Matrix4fA& operator=(XMMATRIX&& mMatrix);

	inline Matrix4fA operator*(const Matrix4fA& mMatrix) const;
	inline Matrix4fA& operator*=(const Matrix4fA& mMatrix);

	// actually faster than DirectX Version :)
	inline Matrix4fA& Rotate_GlobalX_Radians(float fRadians);
	inline Matrix4fA& Rotate_GlobalY_Radians(float fRadians);
	inline Matrix4fA& Rotate_GlobalZ_Radians(float fRadians);

	inline Matrix4fA& Rotate_GlobalX_Degrees(float fDegrees);
	inline Matrix4fA& Rotate_GlobalY_Degrees(float fDegrees);
	inline Matrix4fA& Rotate_GlobalZ_Degrees(float fDegrees);

	inline Matrix4fA& Rotate_LocalX_Radians(float fRadians);
	inline Matrix4fA& Rotate_LocalY_Radians(float fRadians);
	inline Matrix4fA& Rotate_LocalZ_Radians(float fRadians);

	inline Matrix4fA& Rotate_LocalX_Degrees(float fDegrees);
	inline Matrix4fA& Rotate_LocalY_Degrees(float fDegrees);
	inline Matrix4fA& Rotate_LocalZ_Degrees(float fDegrees);

	inline Matrix4fA& Scale(float fXScale, float fYScale, float fZScale);
	inline Matrix4fA& Scale(const Vec3f& vScale);

	inline Matrix4fA& SetScale(float fXScale, float fYScale, float fZScale);
	inline Matrix4fA& SetScale(const Vec3f& vScale);
	inline Matrix4fA& SetScale(float fScale);
	inline Vec3f GetScale() const;

	inline Matrix4fA& Translate(float fX, float fY, float fZ);
	inline Matrix4fA& Translate(Vec3f vTranslation);

	inline Matrix4fA& MoveForward(float fMovement);
	inline Matrix4fA& MoveBackward(float fMovement);
	inline Matrix4fA& MoveLeft(float fMovement);
	inline Matrix4fA& MoveRight(float fMovement);
	inline Matrix4fA& MoveUp(float fMovement);
	inline Matrix4fA& MoveDown(float fMovement);

	inline Matrix4fA& Transpose();
	inline Matrix4fA& Transpose3x3();

	inline Matrix4fA& NormalizeXYZ();

	inline Matrix4fA& Invert();

	inline Matrix4fA& LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	inline Matrix4fA& TurnTo(const Vec3f& vPos, float fTurnModifier = 1.0f);

	inline Matrix4fA& MakePerspective(float fFOV, float fAspectRatio, float fNearClip, float fFarClip);

	inline Matrix4fA& MakeOrthographic(float fWidth, float fHeight, float fNear, float fFar);

	inline Matrix4fA& OrthoNormalInvert();

	inline Matrix4fA& MakeTextureMatrixOffsetLH(unsigned int unWidth, unsigned int unHeight);
};

typedef Matrix4fA Matrix44A;
typedef Matrix4fA Mat4fA;
typedef Matrix4fA float4x4A;
#pragma endregion

#pragma endregion
#endif //SSE_MATH_AVAILABLE

#ifndef SSE_MATH_AVAILABLE
extern const __declspec(selectany) Matrix4f g_IdentityMatrix(1.0f, 0.0f, 0.0f, 0.0f,
															0.0f, 1.0f, 0.0f, 0.0f,
															0.0f, 0.0f, 1.0f, 0.0f,
															0.0f, 0.0f, 0.0f, 1.0f);
#else
extern const __declspec(selectany) Matrix4fA g_IdentityMatrix(1.0f, 0.0f, 0.0f, 0.0f,
															  0.0f, 1.0f, 0.0f, 0.0f,
															  0.0f, 0.0f, 1.0f, 0.0f,
															  0.0f, 0.0f, 0.0f, 1.0f);
#endif

#pragma region Collision Declarations
struct Sphere
{
	Vec3f vCenter;
	float fRadius;

	inline void Build(const Vec3f* pVerts, unsigned int unNumVerts);
};

struct AABB
{
	Vec3f vMinPoint, vMaxPoint;
};

struct Plane
{
	Vec3f vNormal;
	float fOffset;

	inline void Build(const Vec3f& vTopLeft, const Vec3f& vBottomLeft, const Vec3f& vBottomRight);
};

enum FrustumCorners {FTL, FTR, FBR, FBL, NTL, NTR, NBR, NBL, NUM_FRUSTUM_CORNERS};
enum FrustumPlanes {LEFT_PLANE, RIGHT_PLANE, NEAR_PLANE, FAR_PLANE, TOP_PLANE, BOTTOM_PLANE, NUM_FRUSTUM_PLANES};
struct Frustum
{
	Plane Planes[NUM_FRUSTUM_PLANES];
	Vec3f vCorners[NUM_FRUSTUM_CORNERS];

	inline void Build(float fFov, float fNear, float fFar, float fAspectRatio, const Matrix4f& mCamera);
};

#pragma endregion

#include "Vec2f.inl"
#include "Vec3f.inl"
#include "Vec4f.inl"
#include "Matrix3f.inl"
#include "Matrix34f.inl"
#include "Matrix4f.inl"
#include "RMCollisions.inl"

#ifdef SSE_MATH_AVAILABLE
#include "Vec4fA.inl"
#include "Matrix4fA.inl"
#endif
};

#endif // RMMATH_H