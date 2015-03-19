#ifndef AABB_H
#define AABB_H

class AABB;

typedef AABB& AABB_Ref;
typedef const AABB& AABB_ConstRef;

typedef AABB_ConstRef AABB_In;

typedef AABB AABB_Out;

typedef AABB_Ref AABB_InOut;

class AABB
{
private:
	Vec3V m_vMin;
	Vec3V m_vMax;

public:
	AABB();
	explicit AABB(Vec3V_In vMin, Vec3V_In vMax);
	explicit AABB(Sphere_In vSurroundingSphere);

	// Will Overwrite User Data 1
	void SetMin(Vec3V_In vMin);

	Vec3V_Out GetMin() const;
	Vec3V_Ref GetMinRef();
	Vec3V_ConstRef GetMinRef() const;

	// Will Overwrite User Data 2
	void SetMax(Vec3V_In vMax);

	Vec3V_Out GetMax() const;
	Vec3V_Ref GetMaxRef();
	Vec3V_ConstRef GetMaxRef() const;

	void SetUserData1(ScalarV_In vData);
	ScalarV_Out GetUserData1() const;

	void SetUserData2(ScalarV_In vData);
	ScalarV_Out GetUserData2() const;

	Vec3V_Out GetDimensions() const;

	void SetFromSurroundingSphere(Sphere_In vSurroundingSphere);
};

#include "AABB.inl"

#endif // AABB_H