#ifndef RAY_H
#define RAY_H

class Ray;

typedef Ray& Ray_Ref;
typedef const Ray& Ray_ConstRef;

typedef Ray_ConstRef Ray_In;

typedef Ray Ray_Out;

typedef Ray_Ref Ray_InOut;

class Ray
{
private:
	Vec3V m_vOrigin;
	Vec3V m_vDir;

public:
	Ray();
	explicit Ray(Vec3V_In vOrigin, Vec3V_In vDir);

	void SetOrigin(Vec3V_In vOrigin);
	Vec3V_Out GetOrigin() const;
	Vec3V_Ref GetOriginRef();
	Vec3V_ConstRef GetOriginRef() const;

	void SetDir(Vec3V_In vDir);
	Vec3V_Out GetDir() const;
	Vec3V_Ref GetDirRef();
	Vec3V_ConstRef GetDirRef() const;

	void SetUserData1(ScalarV vData);
	ScalarV_Out GetUserData1() const;

	void SetUserData2(ScalarV vData);
	ScalarV_Out GetUserData2() const;
}

#include "Ray.inl"

#endif // RAY_H