#ifndef MAT44F_H
#define MAT44F_H

#include "vec3f.h"

class Mat44f;

typedef Mat44f& Mat44f_Ref;
typedef const Mat44f& Mat44f_ConstRef;

typedef Mat44f_ConstRef Mat44f_In;
typedef Mat44f_Ref Mat44f_InOut;

typedef Mat44f Mat44f_Out;

// Other Mat44f Aliases
typedef Mat44f float4x4;

class Mat44f
{
#define Mat44_ACCESSOR(retType, name, retVal) \
	__forceinline retType name() { return retVal; }

#define Mat44_ACCESSOR_CONST(retType, name, retVal) \
	__forceinline retType name() const { return retVal; }

#define MAT44_MUTATOR(name, inType, modifiedVal) \
	__forceinline void name(inType rhs) { modifiedVal = rhs; }


private:
	union
	{
		float m[16];
		float ma[4][4];

		struct
		{
			Vec4f xAxis, yAxis, zAxis, wAxis;
		};
	};

public:
	Mat44f(){}
	explicit Mat44f(const float& fXx, const float& fXy, const float& fXz, const float& fXw,
					const float& fYx, const float& fYy, const float& fYz, const float& fYw,
					const float& fZx, const float& fZy, const float& fZz, const float& fZw,
					const float& fWx, const float& fWy, const float& fWz, const float& fWw);
	Mat44f(Mat44f_In mMatrix);
	Mat44f(Mat44f&& mMatrix);
	Mat44f(XMMATRIX&& mMatrix);
	explicit Mat44f(Vec4f_In vXAxis,
					Vec4f_In vYAxis,
					Vec4f_In vZAxis,
					Vec4f_In vWAxis);

	explicit Mat44f(eIdentityInitializer eIdentity);
	explicit Mat44f(eXRotationInitializer eXRotation, const float& fRotationInRadians);
	explicit Mat44f(eYRotationInitializer eYRotation, const float& fRotationInRadians);
	explicit Mat44f(eZRotationInitializer eZRotation, const float& fRotationInRadians);

#if defined(Mat44_ACCESSOR) && defined(Mat44_ACCESSOR_CONST)
	Mat44_ACCESSOR_CONST(Vec4f, GetXAxis, xAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetYAxis, yAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetZAxis, zAxis)
	Mat44_ACCESSOR_CONST(Vec4f, GetWAxis, wAxis)
	Mat44_ACCESSOR_CONST(Vec4f_ConstRef, GetXAxisRef, xAxis)
	Mat44_ACCESSOR_CONST(Vec4f_ConstRef, GetYAxisRef, yAxis)
	Mat44_ACCESSOR_CONST(Vec4f_ConstRef, GetZAxisRef, zAxis)
	Mat44_ACCESSOR_CONST(Vec4f_ConstRef, GetWAxisRef, wAxis)

	Mat44_ACCESSOR(Vec4f_Ref, GetXAxisRef, xAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetYAxisRef, yAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetZAxisRef, zAxis)
	Mat44_ACCESSOR(Vec4f_Ref, GetWAxisRef, wAxis)
#undef Mat44_ACCESSOR
#undef Mat44_ACCESSOR_CONST
#else
#error Mat44 ACCESSORS NOT DEFINED
#endif

#ifdef MAT44_MUTATOR
	MAT44_MUTATOR(SetXAxis, Vec4f_In, xAxis)
	MAT44_MUTATOR(SetYAxis, Vec4f_In, yAxis)
	MAT44_MUTATOR(SetZAxis, Vec4f_In, zAxis)
	MAT44_MUTATOR(SetWAxis, Vec4f_In, wAxis)
#undef MAT44_MUTATOR
#else
#error MAT44 MUTATORS NOT DEFINED!
#endif


	void MakeIdentity3x3();

	Mat44f_Ref operator=(Mat44f_In mMatrix);
	Mat44f_Ref operator=(Mat44f&& mMatrix);

	Mat44f_Out operator*(Mat44f_In mMatrix) const;
	void operator*=(Mat44f_In mMatrix);
	friend Vec4f_Out operator*(Vec4f_In vVector, Mat44f_In mMatrix);
	friend Vec4f_ConstRef operator*=(Vec4f_Ref vVector, Mat44f_In mMatrix);

	Mat44f_Out operator+(Mat44f_In rhs) const;
	void operator+=(Mat44f_In rhs);

	Mat44f_Out operator-(Mat44f_In rhs) const;
	void operator-=(Mat44f_In rhs);

	// actually faster than DirectX Version :)
	void Rotate_GlobalX(const float& fRadians);
	void Rotate_GlobalY(const float& fRadians);
	void Rotate_GlobalZ(const float& fRadians);

	void Rotate_LocalX(const float& fRadians);
	void Rotate_LocalY(const float& fRadians);
	void Rotate_LocalZ(const float& fRadians);

	void Scale(const Vec3f& vScale);

	void SetScale(const Vec3f& vScale);
	Vec3f GetScale() const;

	void Translate(Vec3f vTranslation);

	void Transpose();
	void Transpose3x3();

	void Normalize();

	void Invert();

	void LookAt(const Vec3f& mPos, const Vec3f& vWorldUp = g_IdentityY3);

	void TurnTo(Vec3f_In vPos, const float& fDeltaTime, float fTurnModifier = 1.0f);

	void OrthoNormalInvert();
};

#include "Mat44f.inl"

#endif // MAT44F_H