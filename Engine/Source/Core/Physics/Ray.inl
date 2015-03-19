
__forceinline Ray::Ray() {}

__forceinline Ray::Ray(Vec3V_In vOrigin, Vec3V_In vDir) : m_vOrigin(vOrigin), m_vDir(Normalize(vDir)) {}

__forceinline void Ray::SetOrigin(Vec3V_In vOrigin)
{
	m_vOrigin = vOrigin;
}

__forceinline Vec3V_Out Ray::GetOrigin() const
{
	return m_vOrigin;
}

__forceinline Vec3V_Ref Ray::GetOriginRef()
{
	return m_vOrigin;
}

__forceinline Vec3V_ConstRef Ray::GetOriginRef() const
{
	return m_vOrigin;
}

__forceinline void Ray::SetDir(Vec3V_In vDir)
{
	Assert(Abs(Mag(vDir)-ScalarV(I_ONE)) > ScalarV(FLT_EPSILON), "Ray Dir Is Not Normlized!");
	m_vDir = vDir;
}

__forceinline Vec3V_Out Ray::GetDir() const
{
	Assert(Abs(Mag(m_vDir)-ScalarV(I_ONE)) > ScalarV(FLT_EPSILON), "Ray Dir Is Not Normlized!");
	return m_vDir;
}

__forceinline Vec3V_Ref Ray::GetDirRef()
{
	return m_vDir;
}

__forceinline Vec3V_ConstRef Ray::GetDirRef() const
{
	return m_vDir;
}

__forceinline void Ray::SetUserData1(ScalarV vData)
{
	m_vOrigin.SetW(vData);
}

__forceinline ScalarV_Out Ray::GetUserData1() const
{
	return m_vOrigin.GetW();
}

__forceinline void Ray::SetUserData2(ScalarV vData)
{
	m_vDir.SetW(vData);
}

__forceinline ScalarV_Out Ray::GetUserData2() const
{
	return m_vDir.GetW();
}