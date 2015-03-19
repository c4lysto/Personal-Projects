//#ifndef VEC2F_INL
//#define VEC2F_INL

__forceinline Vec2f::Vec2f(Vec2f_In vVector) : x(vVector.x), y(vVector.y)
{

}

__forceinline Vec2f::Vec2f(Vec2f&& vVector)
{
	x = std::move(vVector.x);
	y = std::move(vVector.y);
}

__forceinline Vec2f::Vec2f(const float& fVal) : x(fVal), y(fVal)
{

}

__forceinline Vec2f::Vec2f(const float& fX, const float& fY) : x(fX), y(fY)
{
	
}

#if SSE_AVAILABLE
__forceinline Vec2f::Vec2f(Vector_In vVector)
{
	x = VectorExtractFloat<VecElem::X>(vVector);
	y = VectorExtractFloat<VecElem::Y>(vVector);
}

#if !_WIN64
__forceinline Vec2f::Vec2f(Vector&& vVector)
{
	x = VectorExtractFloat<VecElem::X>(vVector);
	y = VectorExtractFloat<VecElem::Y>(vVector);
}
#endif
#endif

__forceinline Vec2f_Ref Vec2f::operator=(Vec2f_In vVector)
{
	if(this != &vVector)
	{
		x = vVector.x;
		y = vVector.y;
	}
	return *this;
}

__forceinline Vec2f_Ref Vec2f::operator=(Vec2f&& vVector)
{
	if(this != &vVector)
	{
		x = std::move(vVector.x);
		y = std::move(vVector.y);
	}
	return *this;
}

__forceinline Vec2f Vec2f::operator+(Vec2f_In vVector) const
{
	return Vec2f(x + vVector.x, y + vVector.y);
}

__forceinline Vec2f_Ref Vec2f::operator+=(Vec2f_In vVector)
{
	x += vVector.x;	y += vVector.y;
	return *this;
}

__forceinline Vec2f Vec2f::operator-(Vec2f_In vVector) const
{
	return Vec2f(x - vVector.x, y - vVector.y);
}

__forceinline Vec2f_Ref Vec2f::operator-=(Vec2f_In vVector)
{
	x -= vVector.x;	y -= vVector.y;
	return *this;
}

__forceinline Vec2f Vec2f::operator-() const
{
	return Vec2fInt(iX ^ 0x80000000, iY ^ 0x80000000);
}

__forceinline Vec2f Vec2f::operator*(const float& fScalar) const
{
	return Vec2f(x * fScalar, y * fScalar);
}

__forceinline Vec2f operator*(const float& fScalar, Vec2f_In vVector)
{
	return Vec2f(vVector.x * fScalar, vVector.y * fScalar);
}

__forceinline Vec2f_Ref Vec2f::operator*=(const float& fScalar)
{
	x *= fScalar; y *= fScalar;
	return *this;
}

__forceinline Vec2f Vec2f::operator/(const float& fScalar) const
{
	float fInvScalar = 1 / fScalar;
	return Vec2f(x * fInvScalar, y * fInvScalar);
}

__forceinline Vec2f_Ref Vec2f::operator/=(const float& fScalar)
{
	float fInvScalar = 1 / fScalar;
	x *= fInvScalar; y *= fInvScalar;
	return *this;
}

__forceinline bool Vec2f::operator==(Vec2f_In vVector)
{
	if(iX != vVector.iX)
		return false;
	if(iY != vVector.iY)
		return false;
	return true;
}

__forceinline bool Vec2f::operator!=(Vec2f_In vVector)
{
	if(iX != vVector.iX)
		return true;
	if(iY != vVector.iY)
		return true;
	return false;
}

__forceinline Vec2f_Out Vec2f::operator&(Vec2f_In vVector) const
{
	return Vec2fInt(iX & vVector.iX, iY & vVector.iY);
}

__forceinline Vec2f_Ref Vec2f::operator&=(Vec2f_In vVector)
{
	iX &= vVector.iX; iY &= vVector.iY;
	return *this;
}

__forceinline Vec2f_Out Vec2f::operator|(Vec2f_In vVector) const
{
	return Vec2fInt(iX | vVector.iX, iY | vVector.iY);
}

__forceinline Vec2f_Ref Vec2f::operator|=(Vec2f_In vVector)
{
	iX |= vVector.iX; iY |= vVector.iY;
	return *this;
}

__forceinline Vec2f_Out Vec2f::operator^(Vec2f_In vVector) const
{
	return Vec2fInt(iX ^ vVector.iX, iY ^ vVector.iY);
}

__forceinline Vec2f_Ref Vec2f::operator^=(Vec2f_In vVector)
{
	iX ^= vVector.iX; iY ^= vVector.iY;
	return *this;
}

__forceinline Vec2f_Out Vec2f::operator~() const
{
	return Vec2fInt(~iX, ~iY);
}

__forceinline const float& Vec2f::operator[](int index) const
{
	return vector[index];
}

__forceinline float& Vec2f::operator[](int index)
{
	return vector[index];
}

__forceinline Vec2f_Out Vec2fInt(const s32& intVal)
{
	return Vec2f(*reinterpret_cast<const float*>(&intVal));
}

__forceinline Vec2f_Out Vec2fInt(const s32& intX, const s32& intY)
{
	return Vec2f(*reinterpret_cast<const float*>(&intX), *reinterpret_cast<const float*>(&intY));
}

//#endif //VEC2_INL