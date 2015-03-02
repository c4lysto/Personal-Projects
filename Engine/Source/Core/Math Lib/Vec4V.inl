//#if SSE_AVAILABLE
//#ifndef VEC4V_INL
//#define VEC4V_INL

//__forceinline Vec4V::Vec4V(Vec4V_In vVector)
//{
//	row = vVector.row;
//}

#if !_WIN64
__forceinline Vec4V::Vec4V(Vec4V&& vVector)
{
	row = vVector.row;
}
#endif // !_WIN64

__forceinline Vec4V::Vec4V(Vector_In vVector)
{
	row = vVector;
}

#if !_WIN64
__forceinline Vec4V::Vec4V(Vector&& vVector)
{
	row = move(vVector);
}
#endif // !_WIN64

__forceinline Vec4V::Vec4V(const float& fVal)
{
	row = VectorSet(fVal);
}

__forceinline Vec4V::Vec4V(const float& fX, const float& fY, const float& fZ, const float& fW)
{
	row = VectorSet(fX, fY, fZ, fW);
}

__forceinline Vec4V::Vec4V(ScalarV_In vVal)
{
	row = vVal.GetVector();
}

__forceinline Vec4V::Vec4V(ScalarV_In vX, ScalarV_In vY, ScalarV_In vZ, ScalarV_In vW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(vX.GetVector(), vY.GetVector());
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(row, vZ.GetVector());
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(row, vW.GetVector());
}

__forceinline Vec4V::Vec4V(Vec2V_In vXY, Vec2V_In vZW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W2>(vXY.GetVector(), vZW.GetVector());
}

__forceinline Vec4V::Vec4V(ScalarV_In vX, ScalarV_In vY, Vec2V_In vZW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(vX.GetVector(), vY.GetVector());
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W2>(row, vZW.GetVector());
}

__forceinline Vec4V::Vec4V(Vec2V_In vXY, ScalarV_In vZ, ScalarV_In vW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(vZ.GetVector(), vW.GetVector());
	row = VectorPermute<VecElem::X2, VecElem::Y2, VecElem::Z1, VecElem::W1>(row, vXY.GetVector());
}

__forceinline Vec4V::Vec4V(Vec3V_In vXYZ, ScalarV_In vW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(vXYZ.GetVector(), vW.GetVector());
}

__forceinline Vec4V::Vec4V(ScalarV_In vX, Vec3V_In vYZW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z2, VecElem::W2>(vX.GetVector(), vYZW.GetVector());
}

__forceinline void Vec4V::SetX(ScalarV_In vX)
{
	row = VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z1, VecElem::W1>(row, vX.GetVector());
}

__forceinline void Vec4V::SetY(ScalarV_In vY)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(row, vY.GetVector());
}

__forceinline void Vec4V::SetZ(ScalarV_In vZ)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(row, vZ.GetVector());
}

__forceinline void Vec4V::SetW(ScalarV_In vW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(row, vW.GetVector());
}

__forceinline Vec4V Vec4V::operator-() const
{
	return Vec4V(VectorNegate(row));
}

__forceinline Vec4V_Out Vec4V::operator=(Vec4V_In vVector)
{
	if(this != &vVector) { row = vVector.row; }
	return *this;
}

#if !_WIN64
__forceinline Vec4V_Out Vec4V::operator=(Vec4V&& vVector)
{
	if(this != &vVector)
		row = move(vVector.row);
	return *this;
}
#endif // !_WIN64

__forceinline Vec4V Vec4V::operator-(Vec4V_In vVector) const
{
	return Vec4V(VectorSubtract(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator-=(Vec4V_In vVector)
{
	row = VectorSubtract(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator+(Vec4V_In vVector) const
{
	return Vec4V(VectorAdd(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator+=(Vec4V_In vVector)
{
	row = VectorAdd(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator/(ScalarV_In vScalar) const
{
	return Vec4V(VectorDivide(row, vScalar.GetVector()));
}

__forceinline Vec4V_Out Vec4V::operator/=(ScalarV_In vScalar)
{
	row = VectorDivide(row, vScalar.GetVector());
	return *this;
}

__forceinline Vec4V Vec4V::operator*(Vec4V_In vVector) const
{
	return Vec4V(VectorMultiply(row, vVector.row));
}

__forceinline Vec4V_Out Vec4V::operator*=(Vec4V_In vVector)
{
	row = VectorMultiply(row, vVector.row);
	return *this;
}

__forceinline Vec4V Vec4V::operator*(ScalarV_In vScalar) const
{
	return Vec4V(VectorMultiply(row, vScalar.GetVector()));
}

__forceinline Vec4V_Out Vec4V::operator*=(ScalarV_In vScalar)
{
	row = VectorMultiply(row, vScalar.GetVector());
	return *this;
}

__forceinline Vec4V operator*(ScalarV_Ref vScalar, Vec4V_In vVector)
{
	return Vec4V(VectorMultiply(vVector.GetVector(), vScalar.GetVector()));
}

__forceinline bool Vec4V::operator==(Vec4V_In vVector) const
{
	return IsEqualXYZW(row, vVector.row);
}

__forceinline bool Vec4V::operator!=(Vec4V_In vVector) const
{
	return IsNotEqualXYZW(row, vVector.row);
}

__forceinline Vec4V_Out Vec4V::operator&(Vec4V_In vVector) const
{
	return Vec4V(row & vVector.row);
}

__forceinline Vec4V_Out Vec4V::operator&=(Vec4V_In vVector)
{
	row = row & vVector.row;
	return *this;
}

__forceinline Vec4V_Out Vec4V::operator|(Vec4V_In vVector) const
{
	return Vec4V(row | vVector.row);
}

__forceinline Vec4V_Out Vec4V::operator|=(Vec4V_In vVector)
{
	row = row | vVector.row;
	return *this;
}

__forceinline Vec4V_Out Vec4V::operator^(Vec4V_In vVector) const
{
	return Vec4V(row ^ vVector.row);
}

__forceinline Vec4V_Out Vec4V::operator^=(Vec4V_In vVector)
{
	row = row ^ vVector.row;
	return *this;
}

__forceinline Vec4V_Out Vec4V::operator~() const
{
	return Vec4V(~row);
}

__forceinline const float& Vec4V::operator[](int index) const
{
	return floatArr[index];
}

__forceinline float& Vec4V::operator[](int index)
{
	return floatArr[index];
}

__forceinline Vec4V_Out Vec4V::Normalize()
{
	ScalarV mag = Mag(*this);
	if(mag) { row = VectorDivide(row, mag.GetVector()); }
	return *this;
}

__forceinline Vec4V_Out Vec4VInt(int intVal)
{
	return Vec4V(VectorSet(intVal));
}

__forceinline Vec4V_Out Vec4VInt(int intX, int intY, int intZ, int intW)
{
	return Vec4V(VectorSet(intX, intY, intZ, intW));
}

__forceinline Vec4V_Out Vec4VIntToFloat(Vec4V_In vec)
{
	return Vec4V(VectorIntToFloat(vec.GetVector()));
}

__forceinline Vec4V_Out Vec4VFloatToInt(Vec4V_In vec)
{
	return Vec4V(VectorFloatToInt(vec.GetVector())); 
}

//#endif //VEC4V_INL
//#endif //SSE_AVAILABLE