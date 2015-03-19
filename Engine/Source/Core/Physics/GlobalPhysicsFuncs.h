#ifndef GLOBALPHYSICSFUNCS_H
#define GLOBALPHYSICSFUNCS_H


// Sphere Intersections:

bool TestSphereToSphere(Sphere_In lhs, Sphere_In rhs);
// bool TestphereToSphere(Sphere_In lhs, Sphere_In rhs, ScalarV_Ref vOverlap);

// Returns True For Any Sort Of Intersection
bool TestSphereToPlane(Sphere_In vSphere, Plane_In vPlane);

bool TestSphereToRay(Sphere_In vSphere, Ray_In vRay);

// Returns True For Any Sort Of Intersection
// vOverlap - Will Return Negative If The Sphere Is Behind The Plane
//bool TestSphereToPlane(Sphere_In vSphere, Plane_In vPlane, ScalarV_Ref vOverlap);


// Plane Intersections:

// Returns True For Any Sort Of Intersection
bool TestPlaneToSphere(Plane_In vPlane, Sphere_In vSphere);

// Returns True For Any Sort Of Intersection
// vOverlap - Will Return Negative If The Sphere Is Behind The Plane
// bool TestPlaneToSphere(Plane_In vPlane, Sphere_In vSphere, ScalarV_Ref vOverlap);


// Ray Intersections:

bool TestRayToSphere(Ray_In vRay, Sphere_In vSphere);


// AABB Intersections:

bool TestAABBToAABB(AABB_In lhs, AABB_In rhs);


#include "GlobalPhysicsFuncs.inl"

#endif // GLOBALPHYSICSFUNCS_H