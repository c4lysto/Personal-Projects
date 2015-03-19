#ifndef PLANE_H
#define PLANE_H

class Plane;

typedef Plane& Plane_Ref;
typedef const Plane& Plane_ConstRef;

#if _WIN64
typedef Plane Plane_In;
#else
typedef Plane_ConstRef Plane_In;
#endif

typedef Plane Plane_Out;

typedef Plane_Ref Plane_InOut;

class Plane
{
private:
	Vec4V m_vPlane;

public:
	Plane();
	explicit Plane(Vec3V_In vPosition)
	explicit Plane(Vec3V_In vNormal, ScalarV_In vOffset);
	explicit Plane(Vec4V_In vPlane);

	Vec3V_Out GetNormal() const;
	void SetNormal(Vec3V_In vNormal);

	ScalarV_Out GetOffset() const;
	void SetOffset(ScalarV_In vOffset);

	void Set(Vec4V_In vPlane);
	Vec4V_Out Get() const;

	Vec3V_Out GetPlanePos() const;
};

#include "Plane.h"

#endif // PLANE_H