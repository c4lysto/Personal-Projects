//#ifndef VEC4F_INL
//#define VEC4F_INL

__forceinline Vec4f::Vec4f(Vec4f_In vVector)
#if !SSE_AVAILABLE || 1
: x(vVector.x), y(vVector.y), z(vVector.z), w(vVector.w)
#endif
{
#if SSE_AVAILABLE && 0
	VectorStoreU(VectorLoadU(vVector.vector), vector);
#endif
}

__forceinline Vec4f::Vec4f(Vec4f&& vVector)
#if !SSE_AVAILABLE || 1
: x(vVector.x), y(vVector.y), z(vVector.z), w(vVector.w)
#endif
{
#if SSE_AVAILABLE && 0
	VectorStoreU(VectorLoadU(vVector.vector), vector);
#endif
}

#if SSE_AVAILABLE
__forceinline Vec4f::Vec4f(Vector_In vVector)
{
	VectorStoreU(vVector, vector);
}

#if !_WIN64
__forceinline Vec4f::Vec4f(Vector&& vVector)
{
	VectorStoreU(vVector, vector);
}
#endif // !_WIN64
#endif //SSE_AVAILABLE

__forceinline Vec4f::Vec4f(const float& fVal) : x(fVal), y(fVal), z(fVal), w(fVal)
{

}

__forceinline Vec4f::Vec4f(const float& fX, const float& fY, const float& fZ, const float& fW) : x(fX), y(fY), z(fZ), w(fW)
{

}

__forceinline Vec4f::Vec4f(Vec2f_In vXY, Vec2f_In vZW) :
	x(vXY.GetXRef()),
	y(vXY.GetYRef()),
	z(vZW.GetXRef()),
	w(vZW.GetYRef())
{

}

__forceinline Vec4f::Vec4f(const float& fX, const float& fY, Vec2f_In vZW) : 
	x(fX),
	y(fY),
	z(vZW.GetXRef()),
	w(vZW.GetYRef())
{

}

__forceinline Vec4f::Vec4f(Vec2f_In vXY, const float& fZ, const float& fW) :
	x(vXY.GetXRef()),
	y(vXY.GetYRef()),
	z(fZ),
	w(fW)
{

}

__forceinline Vec4f::Vec4f(const float& fX, Vec3f_In vYZW) : 
	x(fX),
	y(vYZW.GetXRef()),
	z(vYZW.GetYRef()),
	w(vYZW.GetZRef())
{

}

__forceinline Vec4f::Vec4f(Vec3f_In vVector, const float& fW) : position(vVector), w(fW)
{

}

__forceinline Vec4f Vec4f::operator-()
{
	return Vec4f(-x, -y, -z, -w);
}

__forceinline Vec4f_Ref Vec4f::operator=(Vec4f_In vVector)
{
#if SSE_AVAILABLE && 0
	VectorStoreU(VectorLoadU(vVector.vector), vector);
#else
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;
	w = vVector.w;
#endif

	return *this;
}

__forceinline Vec4f_Ref Vec4f::operator=(Vec4f&& vVector)
{
#if SSE_AVAILABLE && 0
	VectorStoreU(VectorLoadU(vVector.vector), vector);
#else
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;
	w = vVector.w;
#endif
	return *this;
}

__forceinline Vec4f Vec4f::operator-(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorSubtract(VectorLoadU(vector), VectorLoadU(vVector.vector)), result.vector);
	return result;
#else
	return Vec4f(x - vVector.x, y - vVector.y, z - vVector.z, w - vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator-=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorSubtract(VectorLoadU(vector), VectorLoadU(vVector.vector)), vector);
#else
	x -= vVector.x;
	y -= vVector.y;
	z -= vVector.z;
	w -= vVector.w;
#endif
	return *this;
}

__forceinline Vec4f Vec4f::operator+(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorAdd(VectorLoadU(vector), VectorLoadU(vVector.vector)), result.vector);
	return result;
#else
	return Vec4f(x + vVector.x, y + vVector.y, z + vVector.z, w + vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator+=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorAdd(VectorLoadU(vector), VectorLoadU(vVector.vector)), vector);
#else
	x += vVector.x;
	y += vVector.y;
	z += vVector.z;
	w += vVector.w;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator/(const float& fScalar) const
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorDivide(VectorLoadU(vector), VectorSet(fScalar)), result.vector);
	return result;
#else
	fScalar = 1 / fScalar;
	return Vec4f(x * fScalar, y * fScalar, z * fScalar, w * fScalar);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator/=(const float& fScalar)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorDivide(VectorLoadU(vector), VectorSet(fScalar)), vector);
#else
	fScalar = 1 / fScalar;

	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator*(Vec4f_In vVector) const
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorMultiply(VectorLoadU(vector), VectorLoadU(vVector.vector)), result.vector);
	return result;
#else
	return Vec4f(x * vVector.x, y * vVector.y, z * vVector.z, w * vVector.w);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator*=(Vec4f_In vVector)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorMultiply(VectorLoadU(vector), VectorLoadU(vVector.vector)), vector);
#else
	x *= vVector.x;
	y *= vVector.y;
	z *= vVector.z;
	w *= vVector.w;
#endif

	return *this;
}

__forceinline Vec4f Vec4f::operator*(const float& fScalar) const
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorMultiply(VectorLoadU(vector), VectorSet(fScalar)), result.vector);
	return result;
#else
	return Vec4f(x * fScalar, y * fScalar, z * fScalar, w * fScalar);
#endif
}

__forceinline Vec4f_Ref Vec4f::operator*=(const float& fScalar)
{
#if SSE_AVAILABLE
	VectorStoreU(VectorMultiply(VectorLoadU(vector), VectorSet(fScalar)), vector);
#else
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
#endif

	return *this;
}

__forceinline Vec4f operator*(const float& fScalar, Vec4f_In vVector)
{
#if SSE_AVAILABLE
	Vec4f result;
	VectorStoreU(VectorMultiply(VectorLoadU(vVector.vector), VectorSet(fScalar)), result.vector);
	return result;
#else
	return Vec4f(vVector.x * fScalar, vVector.y * fScalar, vVector.z * fScalar, vVector.w * fScalar);
#endif
}

__forceinline Vec4f_Out Vec4f::operator&(Vec4f_In vVector) const
{
	Vec4f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) & *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) & *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) & *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&retVal.w) = *reinterpret_cast<const s32*>(&w) & *reinterpret_cast<const s32*>(&vVector.w);
	return retVal;
}

__forceinline Vec4f_Ref Vec4f::operator&=(Vec4f_In vVector)
{
	*reinterpret_cast<s32*>(&x) &= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) &= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) &= *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&w) &= *reinterpret_cast<const s32*>(&vVector.w);
	return *this;
}

__forceinline Vec4f_Out Vec4f::operator|(Vec4f_In vVector) const
{
	Vec4f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) | *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) | *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) | *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&retVal.w) = *reinterpret_cast<const s32*>(&w) | *reinterpret_cast<const s32*>(&vVector.w);
	return retVal;
}

__forceinline Vec4f_Ref Vec4f::operator|=(Vec4f_In vVector)
{
	*reinterpret_cast<s32*>(&x) |= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) |= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) |= *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&w) |= *reinterpret_cast<const s32*>(&vVector.w);
	return *this;
}

__forceinline Vec4f_Out Vec4f::operator^(Vec4f_In vVector) const
{
	Vec4f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = *reinterpret_cast<const s32*>(&x) ^ *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&retVal.y) = *reinterpret_cast<const s32*>(&y) ^ *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&retVal.z) = *reinterpret_cast<const s32*>(&z) ^ *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&retVal.w) = *reinterpret_cast<const s32*>(&w) ^ *reinterpret_cast<const s32*>(&vVector.w);
	return retVal;
}

__forceinline Vec4f_Ref Vec4f::operator^=(Vec4f_In vVector)
{
	*reinterpret_cast<s32*>(&x) ^= *reinterpret_cast<const s32*>(&vVector.x);
	*reinterpret_cast<s32*>(&y) ^= *reinterpret_cast<const s32*>(&vVector.y);
	*reinterpret_cast<s32*>(&z) ^= *reinterpret_cast<const s32*>(&vVector.z);
	*reinterpret_cast<s32*>(&w) ^= *reinterpret_cast<const s32*>(&vVector.w);
	return *this;
}

__forceinline Vec4f_Out Vec4f::operator~() const
{
	Vec4f retVal;
	*reinterpret_cast<s32*>(&retVal.x) = ~*reinterpret_cast<const s32*>(&x);
	*reinterpret_cast<s32*>(&retVal.y) = ~*reinterpret_cast<const s32*>(&y);
	*reinterpret_cast<s32*>(&retVal.z) = ~*reinterpret_cast<const s32*>(&z);
	*reinterpret_cast<s32*>(&retVal.w) = ~*reinterpret_cast<const s32*>(&w);
	return retVal;
}

__forceinline const float& Vec4f::operator[](int index) const
{
	return vector[index];
}

__forceinline float& Vec4f::operator[](int index)
{
	return vector[index];
}

//#endif //VEC4_INL