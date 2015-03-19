

// Sphere Intersections:

__forceinline bool TestSphereToSphere(Sphere_In lhs, Sphere_In rhs)
{
	Vec3V vCenterToCenter = lhs.GetCenter() - rhs.GetCenter();
	ScalarV vRadiiSq = lhs.GetRadiusSq() + rhs.GetRadiusSq();
	return vRadiiSq >= MagSq(vCenterToCenter);
}

//inline bool TestSphereToSphere(Sphere_In lhs, Sphere_In rhs, ScalarV_Ref vOverlap)
//{
//	Vec3V vCenterToCenter = lhs.GetCenter() - rhs.GetCenter();
//	ScalarV vRadiiSq = lhs.GetRadiusSq() + rhs.GetRadiusSq();
//	ScalarV vDistSq = MagSq(vCenterToCenter);
//	vOverlap = Sqrt(vRadiiSq - vDistSq);
//	return vRadiiSq >= vDistSq;
//}

__forceinline bool TestSphereToPlane(Sphere_In vSphere, Plane_In vPlane)
{
	Vec3V vPlanePosToCenter = vSphere.GetCenter() - vPlane.GetPlanePos();
	return vSphere.GetRadiusSq() <= Abs(Dot(vPlanePosToCenter, vPlane.GetNormal()));
}

bool TestSphereToRay(Sphere_In vSphere, Ray_In vRay)
{
	Vec3V vSphereCenter = vSphere.GetCenter();
	Vec3V vRayOriginToSphere = vSphereCenter - vRay.GetOrigin();
	Vec3V vClosestPointToSphere = vRay.GetDirRef() * Dot(vRay.GetDirRef(), vRayOriginToSphere);
	return vSphere.GetRadiusSq() >= MagSq(vClosestPointToSphere-vSphereCenter);
}

// Returns True For Any Sort Of Intersection
// vOverlap - Will Return Negative If The Sphere Is Behind The Plane
//__forceinline bool TestSphereToPlane(Sphere_In vSphere, Plane_In vPlane, ScalarV_Ref vOverlap)
//{
//	Vec3V vPlanePosToCenter = vSphere.GetCenter() - vPlane.GetPlanePos();
//	vOverlap = Dot(vPlanePosToCenter, vPlane.GetNormal());
//	return vSphere.GetRadiusSq() <= Abs(vOverlap);
//}


// Plane Intersections:

__forceinline bool TestPlaneToSphere(Plane_In vPlane, Sphere_In vSphere)
{
	return TestSphereToPlane(vSphere, vPlane);
}

//__forceinline bool TestPlaneToSphere(Plane_In vPlane, Sphere_In vSphere, ScalarV_Ref vOverlap)
//{
//	return IntersectSphereToPlane(vSphere, vPlane, vOverlap);
//}


// Ray Intersections:

__forceinline bool TestRayToSphere(Ray_In vRay, Sphere_In vSphere)
{
	return TestSphereToRay(vSphere, vRay);
}


// AABB Intersections:

__forceinline bool TestAABBToAABB(AABB_In lhs, AABB_In rhs)
{
	return (IsLessThan(lhs.m_vMax, rhs.m_vMin)) != 0;
}