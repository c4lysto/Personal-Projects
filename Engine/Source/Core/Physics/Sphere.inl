
__forceinline Sphere::Sphere() {}

__forceinline Sphere::Sphere(Vec3V_In vCenter, ScalarV_In vRadius) : m_vSphere(vCenter, vRadius) {}

__forceinline Sphere::Sphere(Vec4V_In vSphere) : m_vSphere(vSphere) {}

__forceinline Vec3V_Out Sphere::GetCenter() const
{
	return m_vSphere.GetXYZ();
}

__forceinline void Sphere::SetCenter(Vec3V_In vCenter)
{
	m_vSphere.SetXYZ(vCenter);
}

__forceinline ScalarV_Out Sphere::GetRadius() const
{
	return m_vSphere.GetW();
}

__forceinline ScalarV_Out Sphere::GetRadiusSq() const
{
	ScalarV vRadius = GetRadius();
	return vRadius * vRadius;
}

__forceinline void Sphere::SetRadius(ScalarV_In vRadius)
{
	m_vSphere.SetW(vRadius);
}

__forceinline void Sphere::Set(Vec4V_In vSphere)
{
	m_vSphere = vSphere;
}

__forceinline Vec4V_Out Sphere::Get() const
{
	return m_vSphere;
}