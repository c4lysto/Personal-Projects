//#ifndef VEC3F_INL
//#define VEC3F_INL

__forceinline Vec3f::Vec3f(const float& fVal) : x(fVal), y(fVal), z(fVal)
{

}

__forceinline Vec3f::Vec3f(const float& fX, const float& fY, const float& fZ) : x(fX), y(fY), z(fZ)
{

}

__forceinline Vec3f::Vec3f(const float& fX, Vec2f_In vYZ) : x(fX), y(vYZ.GetXRef()), z(vYZ.GetYRef())
{

}

__forceinline Vec3f::Vec3f(Vec2f_In vXY, const float& fZ) : x(vXY.GetXRef()), y(vXY.GetYRef()), z(fZ)
{

}

__forceinline Vec3f::Vec3f(Vec3f_In vVector) : x(vVector.x), y(vVector.y), z(vVector.z)
{

}

__forceinline Vec3f::Vec3f(Vec3f&& vVector)
{
	x = move(vVector.x);
	y = move(vVector.y);
	z = move(vVector.z);
}

#if SSE_AVAILABLE
__forceinline Vec3f::Vec3f(Vector_In vVector)
{
	x = VectorExtractFloat<VecElem::X>(vVector);
	y = VectorExtractFloat<VecElem::Y>(vVector);
	z = VectorExtractFloat<VecElem::Z>(vVector);
}

#if !_WIN64
__forceinline Vec3f::Vec3f(Vector&& vVector)
{
	x = VectorExtractFloat<VecElem::X>(vVector);
	y = VectorExtractFloat<VecElem::Y>(vVector);
	z = VectorExtractFloat<VecElem::Z>(vVector);
}
#endif // !_WIN64
#endif // SSE_AVAILABLE

__forceinline Vec3f Vec3f::operator-() const
{
	return Vec3f(-x, -y, -z);
}

__forceinline Vec3f_Ref Vec3f::operator=(Vec3f_In vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
		z = vVector.z;
	}

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator=(Vec3f&& vVector)
{
	if(this != &vVector)
	{
		x = move(vVector.x);
		y = move(vVector.y);
		z = move(vVector.z);
	}

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator*=(const float& fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator*=(Vec3f_In vScale)
{
	x *= vScale.x;
	y *= vScale.y;
	z *= vScale.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator/(const float& fScalar) const
{
#if SSE_AVAILABLE
	Vector result = VectorDivide(VectorSet(x, y, z, 0.0f), VectorSet(fScalar));
	return Vec3f(VectorExtractFloat<VecElem::X>(result), VectorExtractFloat<VecElem::Y>(result), VectorExtractFloat<VecElem::Z>(result));
#else
	fScalar = 1 / fScalar;
	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
#endif
}

__forceinline Vec3f Vec3f::operator/(Vec3f_In vScale) const
{
	return Vec3f(x / vScale.x, y / vScale.y, z / vScale.z);
}

__forceinline Vec3f_Ref Vec3f::operator/=(const float& fScalar)
{
	float fInvScalar = 1 / fScalar;

	x *= fInvScalar;
	y *= fInvScalar;
	z *= fInvScalar;

	return *this;
}

__forceinline Vec3f_Ref Vec3f::operator/=(Vec3f_In vScale)
{
	x /= vScale.x;
	y /= vScale.y;
	z /= vScale.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator*(const float& fScalar) const
{
	return Vec3f(x * fScalar, y * fScalar, z * fScalar);
}

__forceinline Vec3f Vec3f::operator*(Vec3f_In vScale) const
{
	return Vec3f(x * vScale.x, y * vScale.y, z * vScale.z);
}

__forceinline Vec3f operator*(const float& fScalar, Vec3f_In vVector)
{
	return Vec3f(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar);
}

__forceinline Vec3f_Ref Vec3f::operator+=(Vec3f_In vVector)
{
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator+(Vec3f_In vVector) const
{
	return Vec3f(x + vVector.x, y + vVector.y, z + vVector.z);
}

__forceinline Vec3f_Ref Vec3f::operator-=(Vec3f_In vVector)
{
	x -= vVector.x;
	y -= vVector.y;
	z -= vVector.z;

	return *this;
}

__forceinline Vec3f Vec3f::operator-(Vec3f_In vVector) const
{
	return Vec3f(x - vVector.x, y - vVector.y, z - vVector.z);
}

__forceinline bool Vec3f::operator==(Vec3f_In vVector) const
{
	if(x != vVector.x)
		return false;

	if(y != vVector.y)
		return false;

	if(z != vVector.z)
		return false;

	return true;
}

__forceinline bool Vec3f::operator!=(Vec3f_In vVector) const
{
	if(x != vVector.x)
		return true;

	if(y != vVector.y)
		return true;

	if(z != vVector.z)
		return true;

	return false;
}

__forceinline Vec3f_Out Vec3f::operator&(Vec3f_In vVector) const
{
	Vec3f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) & *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) & *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) & *reinterpret_cast<const s32*>(&vVector.z);
	return retVal;
}

__forceinline Vec3f_Ref Vec3f::operator&=(Vec3f_In vVector)
{
	*reinterpret_cast<s32*>(&x) &= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) &= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) &= *reinterpret_cast<const s32*>(&vVector.z);
	return *this;
}

__forceinline Vec3f_Out Vec3f::operator|(Vec3f_In vVector) const
{
	Vec3f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) | *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) | *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) | *reinterpret_cast<const s32*>(&vVector.z);
	return retVal;
}

__forceinline Vec3f_Ref Vec3f::operator|=(Vec3f_In vVector)
{
	*reinterpret_cast<s32*>(&x) |= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) |= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) |= *reinterpret_cast<const s32*>(&vVector.z);
	return *this;
}

__forceinline Vec3f_Out Vec3f::operator^(Vec3f_In vVector) const
{
	Vec3f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) ^ *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) ^ *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) ^ *reinterpret_cast<const s32*>(&vVector.z);
	return retVal;
}

__forceinline Vec3f_Ref Vec3f::operator^=(Vec3f_In vVector)
{
	*reinterpret_cast<s32*>(&x) ^= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) ^= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) ^= *reinterpret_cast<const s32*>(&vVector.z);
	return *this;
}

__forceinline Vec3f_Out Vec3f::operator~() const
{
	Vec3f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = ~*reinterpret_cast<const s32*>(&x);
	*reinterpret_cast<s32*>(&retVal.y) = ~*reinterpret_cast<const s32*>(&y);
	*reinterpret_cast<s32*>(&retVal.z) = ~*reinterpret_cast<const s32*>(&z);
	return retVal;
}

__forceinline const float& Vec3f::operator[](int index) const
{
	return vector[index];
}

__forceinline float& Vec3f::operator[](int index)
{
	return vector[index];
}

//#endif VEC3F_INL