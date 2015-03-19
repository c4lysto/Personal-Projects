
__forceinline Plane::Plane() {}

inline Plane::Plane(Vec3V_In vPosition) : m_vPlane(Normalize(vPosition), Mag(vPosition))
{

}

__forceinline Plane::Plane(Vec3V_In vNormal, ScalarV_In vOffset) : m_vPlane(Normalize(vNormal), vOffset)
{

}

__forceinline Plane::Plane(Vec4V_In vPlane) : m_vPlane(vPlane)
{

}

__forceinline Vec3V_Out Plane::GetNormal() const
{
	return m_vPlane.GetXYZ();
}

__forceinline void Plane::SetNormal(Vec3V_In vNormal)
{
	m_vPlane.SetXYZ(vNormal);
}

__forceinline ScalarV_Out Plane::GetOffset() const
{
	return m_vPlane.GetW();
}

__forceinline void Plane::SetOffset(ScalarV_In vOffset)
{
	m_vPlane.SetW(vOffset);
}

__forceinline void Plane::Set(Vec4V_In vPlane)
{
	m_vPlane = vPlane;
}

__forceinline Vec4V_Out Plane::Get() const
{
	return m_vPlane;
}

__forceinline Vec3V_Out Plane::GetPlanePos() const
{
	return GetNormal() * GetOffset();
}