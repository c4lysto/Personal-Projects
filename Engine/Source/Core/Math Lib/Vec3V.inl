
__forceinline Vec3V::Vec3V(const float& fVal)
{
	row = VectorSet(fVal);
}

__forceinline Vec3V::Vec3V(const float& fX, const float& fY, const float& fZ)
{
	row = VectorSet(fX, fY, fZ, VEC_FILL_VAL);
}

__forceinline Vec3V::Vec3V(ScalarV_In vVal)
{
	row = vVal.GetVector();
}

__forceinline Vec3V::Vec3V(ScalarV_In vX, ScalarV_In vY, ScalarV_In vZ)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(vX.GetVector(), vY.GetVector());
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(row, vZ.GetVector());
}

__forceinline Vec3V::Vec3V(Vec2V_In vXY, ScalarV_In vZ)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(vXY.GetVector(), vZ.GetVector());
}

__forceinline Vec3V::Vec3V(ScalarV_In vX, Vec2V_In vYZ)
{
	row = VectorPermute<VecElem::X2, VecElem::X1, VecElem::Y1, VecElem::W2>(vYZ.GetVector(), vX.GetVector());
}

//__forceinline Vec3V::Vec3V(Vec3V_In vVector)
//{
//	row = vVector.row;
//}

#if !_WIN64
__forceinline Vec3V::Vec3V(Vec3V&& vVector)
{
	row = move(vVector.row);
}
#endif // !_WIN64

__forceinline Vec3V::Vec3V(Vector_In vVector)
{
	row = vVector;
}

#if !_WIN64
__forceinline Vec3V::Vec3V(Vector&& vVector)
{
	row = move(vVector);
}
#endif // !_WIN64

__forceinline void Vec3V::SetX(ScalarV_In vX)
{
	row = VectorPermute<VecElem::X2, VecElem::Y1, VecElem::Z1, VecElem::W1>(row, vX.GetVector());
}

__forceinline void Vec3V::SetY(ScalarV_In vY)
{
	row = VectorPermute<VecElem::X1, VecElem::Y2, VecElem::Z1, VecElem::W1>(row, vY.GetVector());
}

__forceinline void Vec3V::SetZ(ScalarV_In vZ)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z2, VecElem::W1>(row, vZ.GetVector());
}

__forceinline void Vec3V::SetW(ScalarV_In vW)
{
	row = VectorPermute<VecElem::X1, VecElem::Y1, VecElem::Z1, VecElem::W2>(row, vW.GetVector());
}

__forceinline Vec3V Vec3V::operator-() const
{
	return Vec3V(VectorNegate(row));
}

__forceinline Vec3V_Out Vec3V::operator=(Vec3V_In vVector)
{
	if(this != &vVector) { row = vVector.row; }
	return *this;
}

#if !_WIN64
__forceinline Vec3V_Out Vec3V::operator=(Vec3V&& vVector)
{
	if(this != &vVector)
		row = move(vVector.row);
	return *this;
}
#endif // !_WIN_64

__forceinline Vec3V_Out Vec3V::operator*=(ScalarV_In vScalar)
{
	row = VectorDivide(row, vScalar.GetVector());
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator*=(Vec3V_In rhs)
{
	row = VectorMultiply(row, rhs.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator/(ScalarV_In vScalar) const
{
	return Vec3V(VectorDivide(row, vScalar.GetVector()));
}

__forceinline Vec3V Vec3V::operator/(Vec3V_In rhs) const
{
	return Vec3V(VectorDivide(row, rhs.row));
}

__forceinline Vec3V_Out Vec3V::operator/=(ScalarV_In vScalar)
{
	row = VectorDivide(row, vScalar.GetVector());
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator/=(Vec3V_In rhs)
{
	row = VectorDivide(row, rhs.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator*(ScalarV_In vScalar) const
{
	return Vec3V(VectorMultiply(row, vScalar.GetVector()));
}

__forceinline Vec3V Vec3V::operator*(Vec3V_In rhs) const
{
	return Vec3V(VectorMultiply(row, rhs.row));
}

__forceinline Vec3V operator*(ScalarV_Ref vScalar, Vec3V_In vVector)
{
	return Vec3V(VectorMultiply(vScalar.GetVector(), vVector.row));
}

__forceinline Vec3V_Out Vec3V::operator+=(Vec3V_In vVector)
{
	row = VectorAdd(row, vVector.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator+(Vec3V_In vVector) const
{
	return Vec3V(VectorAdd(row, vVector.row));
}

__forceinline Vec3V_Out Vec3V::operator-=(Vec3V_In vVector)
{
	row = VectorSubtract(row, vVector.row);
	return *this;
}

__forceinline Vec3V Vec3V::operator-(Vec3V_In vVector) const
{
	return Vec3V(VectorSubtract(row, vVector.row));
}

__forceinline bool Vec3V::operator==(Vec3V_In vVector) const
{
	return VectorIsEqualXYZ(row, vVector.row);
}

__forceinline bool Vec3V::operator!=(Vec3V_In vVector) const
{
	return VectorIsNotEqualXYZ(row, vVector.row);
}

__forceinline Vec3V_Out Vec3V::operator&(Vec3V_In vVector) const
{
	return Vec3V(row & vVector.row);
}

__forceinline Vec3V_Out Vec3V::operator&=(Vec3V_In vVector)
{
	row = row & vVector.row;
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator|(Vec3V_In vVector) const
{
	return Vec3V(row | vVector.row);
}

__forceinline Vec3V_Out Vec3V::operator|=(Vec3V_In vVector)
{
	row = row | vVector.row;
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator^(Vec3V_In vVector) const
{
	return Vec3V(row ^ vVector.row);
}

__forceinline Vec3V_Out Vec3V::operator^=(Vec3V_In vVector)
{
	row = row ^ vVector.row;
	return *this;
}

__forceinline Vec3V_Out Vec3V::operator~() const
{
	return Vec3V(~row);
}

__forceinline const float& Vec3V::operator[](int index) const
{
	return floatArr[index];
}

__forceinline float& Vec3V::operator[](int index)
{
	return floatArr[index];
}

__forceinline Vec3V_Out Vec3V::Normalize()
{
	ScalarV mag = Mag(*this);

	// protection against divide by zero
	if(mag)	{ row = VectorDivide(row, mag.GetVector()); }
	return *this;
}

__forceinline Vec3V_Out Vec3VInt(int intVal)
{
	return Vec3V(VectorSet(intVal));
}

__forceinline Vec3V_Out Vec3VInt(int intX, int intY, int intZ)
{
	return Vec3V(VectorSet(intX, intY, intZ, (int)VEC_FILL_VAL));
}

__forceinline Vec3V_Out Vec3VIntToFloat(Vec3V_In vec)
{
	return Vec3V(VectorIntToFloat(vec.GetVector()));
}

__forceinline Vec3V_Out Vec3VFloatToInt(Vec3V_In vec)
{
	return Vec3V(VectorFloatToInt(vec.GetVector()));
}
