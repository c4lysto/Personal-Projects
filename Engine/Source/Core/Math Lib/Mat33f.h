#ifndef MAT33F_H
#define MAT33F_H

#include "Vec3f.h"

class Mat33f;

typedef Mat33f& Mat33f_Ref;
typedef const Mat33f& Mat33f_ConstRef;

typedef Mat33f_ConstRef Mat33f_In;
typedef Mat33f_Ref Mat33f_InOut;

typedef Mat33f Mat33f_Out;

// Other Mat33f Aliases
typedef Mat33f float3x3;

class Mat33f
{
#define MAT33F_ACCESSOR(retType, name, retVal) \
	__forceinline retType name() { return retVal; }

#define MAT33F_ACCESSOR_CONST(retType, name, retVal) \
	__forceinline retType name() const { return retVal; }

#define MAT33F_MUTATOR(name, inType, modifiedVal) \
	__forceinline void name(inType rhs) { modifiedVal = rhs; }

private:
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
			Vec3f xAxis, yAxis, zAxis;
		};
	};

public:
	Mat33f(){}
	explicit Mat33f(const float& fXx, const float& fXy, const float& fXz,
					const float& fYx, const float& fYy, const float& fYz,
					const float& fZx, const float& fZy, const float& fZz);
	Mat33f(Mat33f_In rhs);
	Mat33f(Mat33f&& rhs);
	explicit Mat33f(Vec3f_In vXAxis,
					Vec3f_In vYAxis,
					Vec3f_In vZAxis);
	explicit Mat33f(eIdentityInitializer eIdentity);
	explicit Mat33f(eXRotationInitializer eXRotation, const float& fRotationInRadians);
	explicit Mat33f(eYRotationInitializer eYRotation, const float& fRotationInRadians);
	explicit Mat33f(eZRotationInitializer eZRotation, const float& fRotationInRadians);

#if defined(MAT33F_ACCESSOR) && defined(MAT33F_ACCESSOR_CONST)
	MAT33F_ACCESSOR_CONST(Vec3f_Out, GetXAxis, xAxis)
	MAT33F_ACCESSOR_CONST(Vec3f_Out, GetYAxis, yAxis)
	MAT33F_ACCESSOR_CONST(Vec3f_Out, GetZAxis, zAxis)
	MAT33F_ACCESSOR_CONST(Vec3f_ConstRef, GetXAxisRef, xAxis)
	MAT33F_ACCESSOR_CONST(Vec3f_ConstRef, GetYAxisRef, yAxis)
	MAT33F_ACCESSOR_CONST(Vec3f_ConstRef, GetZAxisRef, zAxis)

	MAT33F_ACCESSOR(Vec3f_Ref, GetXAxisRef, xAxis)
	MAT33F_ACCESSOR(Vec3f_Ref, GetYAxisRef, yAxis)
	MAT33F_ACCESSOR(Vec3f_Ref, GetZAxisRef, zAxis)

#undef MAT33F_ACCESSOR
#undef MAT33F_ACCESSOR_CONST
#else
#error MAT33F ACCESSORS NOT DEFINED
#endif

#ifdef MAT33F_MUTATOR
	MAT33F_MUTATOR(SetXAxis, Vec3f_In, xAxis)
	MAT33F_MUTATOR(SetYAxis, Vec3f_In, yAxis)
	MAT33F_MUTATOR(SetZAxis, Vec3f_In, zAxis)
#undef MAT33F_MUTATOR
#else
#error MAT33F MUTATORS NOT DEFINED!
#endif

	Mat33f_Ref operator=(Mat33f_In mMatrix);
	Mat33f_Ref operator=(Mat33f&& mMatrix);

	Mat33f_Out operator*(Mat33f_In mMatrix) const;
	void operator*=(Mat33f_In mMatrix);
	friend Vec3f_Out operator*(Vec3f_ConstRef vVector, Mat33f_In mMatrix);
	friend Vec3f_Ref operator*=(Vec3f_Ref vVector, Mat33f_In mMatrix);

	Mat33f_Out operator+(Mat33f_In rhs) const;
	void operator+=(Mat33f_In rhs);

	Mat33f_Out operator-(Mat33f_In rhs) const;
	void operator-=(Mat33f_In rhs);

	void Rotate_LocalX(const float& fRadians);
	void Rotate_LocalY(const float& fRadians);
	void Rotate_LocalZ(const float& fRadians);

	void Scale(Vec3f_In vScale);

	void SetScale(Vec3f_In vScale);
	Vec3f_Out GetScale() const;

	void Transpose();

	void Normalize();
};

#include "Mat33f.inl"

#endif // MAT33F_H