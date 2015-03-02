

#if !_WIN64
__forceinline ScalarV::ScalarV(ScalarV&& vVector)
{
	row = move(vVector.row);
}
#endif // !_WIN64

__forceinline ScalarV::ScalarV(Vector_In vVector)
{
	row = vVector;
}

__forceinline ScalarV::ScalarV(const float& fVal)
{
	row = VectorSet(fVal);
}

__forceinline ScalarV::ScalarV(const int& iVal)
{
	row = VectorSet(iVal);
}

__forceinline Vector_Out ScalarV::GetVector() const
{
	return row;
}

__forceinline float ScalarV::GetFloat()
{
	return VectorExtractFloat<VecElem::X>(row);
}

__forceinline float ScalarV::AsFloat()
{
	return GetFloat();
}

__forceinline int ScalarV::GetInt()
{
	return VectorExtractInt<VecElem::X>(row);
}

__forceinline int ScalarV::AsInt()
{
	return GetInt();
}

__forceinline ScalarV_Out ScalarV::operator-() const
{
	return ScalarV(VectorNegate(row));
}

__forceinline ScalarV_Ref ScalarV::operator=(ScalarV_In rhs)
{
	if(this != &rhs)
	{
		row = rhs.row;
	}
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator+(ScalarV_In rhs) const
{
	return ScalarV(VectorAdd(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator+=(ScalarV_In rhs)
{
	row = VectorAdd(row, rhs.row);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator-(ScalarV_In rhs) const
{
	return ScalarV(VectorSubtract(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator-=(ScalarV_In rhs)
{
	row = VectorSubtract(row, rhs.row);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator*(ScalarV_In rhs) const
{
	return ScalarV(VectorMultiply(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator*=(ScalarV_In rhs)
{
	row = VectorMultiply(row, rhs.row);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator/(ScalarV_In rhs) const
{
	return ScalarV(VectorDivide(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator/=(ScalarV_In rhs)
{
	row = VectorDivide(row, rhs.row);
	return *this;
}

// Logical Operators
__forceinline ScalarV_Out ScalarV::operator<<(int nCount) const
{
	return ScalarV(VectorLeftShift(row, nCount));
}

__forceinline ScalarV_Ref ScalarV::operator<<=(int nCount)
{
	row = VectorLeftShift(row, nCount);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator>>(int nCount) const
{
	return ScalarV(VectorRightShift(row, nCount));
}

__forceinline ScalarV_Ref ScalarV::operator>>=(int nCount)
{
	row = VectorRightShift(row, nCount);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator&(ScalarV_In rhs) const
{
	return ScalarV(VectorAnd(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator&=(ScalarV_In rhs)
{
	row = VectorAnd(row, rhs.row);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator|(ScalarV_In rhs) const
{
	return ScalarV(VectorOr(row, rhs.row));
}

__forceinline ScalarV_Ref ScalarV::operator|=(ScalarV_In rhs)
{
	row = VectorOr(row, rhs.row);
	return *this;
}

__forceinline ScalarV_Out ScalarV::operator~() const
{
	return ScalarV(VectorNot(row));
}

__forceinline bool ScalarV::operator==(ScalarV_In rhs) const
{
	return IsEqualIntX(row, rhs.row);
}

__forceinline bool ScalarV::operator!=(ScalarV_In rhs) const
{
	return IsNotEqualIntX(row, rhs.row);
}

__forceinline bool ScalarV::operator<(ScalarV_In rhs) const
{
	return IsLessThanX(row, rhs.row);
}

__forceinline bool ScalarV::operator<=(ScalarV_In rhs) const
{
	return IsLessThanOrEqualX(row, rhs.row);
}

__forceinline bool ScalarV::operator>(ScalarV_In rhs) const
{
	return IsGreaterThanX(row, rhs.row);
}

__forceinline bool ScalarV::operator>=(ScalarV_In rhs) const
{
	return IsGreaterThanOrEqualX(row, rhs.row);
}

__forceinline ScalarV::operator bool() const
{
	return *this != ScalarV(INIT_ZERO);
}