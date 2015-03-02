#ifndef SCALARV_H
#define SCALARV_H

#if SSE_AVAILABLE

class ScalarV;

typedef ScalarV& ScalarV_Ref;
typedef const ScalarV& ScalarV_ConstRef;

#if _WIN64
typedef ScalarV ScalarV_In;
#else
typedef ScalarV_ConstRef ScalarV_In;
#endif

typedef ScalarV ScalarV_Out;

typedef ScalarV_Ref ScalarV_InOut;


class ScalarV
{
#define DEFINE_SCALARV_ENUM_CONSTRUCTOR(enumeration, valueToInit) \
	explicit __forceinline ScalarV(enumeration) { row = VectorSet(valueToInit); }

private:

	Vector row;

public:
	ScalarV(){}
#if !_WIN64
	ScalarV(ScalarV&& vVector);
#endif // !_WIN64
	explicit ScalarV(Vector_In vVector);
	explicit ScalarV(const float& fVal);
	explicit ScalarV(const int& iVal);

#ifdef DEFINE_SCALARV_ENUM_CONSTRUCTOR
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eZeroInitializer, 0.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eOneInitializer, 1.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eTwoInitializer, 2.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eThreeInitializer, 3.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eFourInitializer, 4.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eFiveInitializer, 5.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eSixInitializer, 6.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eSevenInitializer, 7.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eEightInitializer, 8.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eNineInitializer, 9.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eTenInitializer, 10.0f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eQuarterInitializer, 0.25f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eHalfInitializer, 0.5f)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(ePIInitializer, PI)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eHalfPIInitializer, PI_OVER_2)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eTwoPIInitializer, _2PI)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eFLTMINInitializer, FLT_MIN)
	DEFINE_SCALARV_ENUM_CONSTRUCTOR(eFLTMAXInitializer, FLT_MAX)
#undef DEFINE_SCALARV_ENUM_CONSTRUCTOR
#endif //DEFINE_SCALARV_ENUM_CONSTRUCTOR

	Vector_Out GetVector() const;

	float GetFloat();
	float AsFloat();

	int GetInt();
	int AsInt();

	ScalarV_Out operator-() const;

	ScalarV_Ref operator=(ScalarV_In rhs);

	ScalarV_Out operator+(ScalarV_In rhs) const;
	ScalarV_Ref operator+=(ScalarV_In rhs);

	ScalarV_Out operator-(ScalarV_In rhs) const;
	ScalarV_Ref operator-=(ScalarV_In rhs);

	ScalarV_Out operator*(ScalarV_In rhs) const;
	ScalarV_Ref operator*=(ScalarV_In rhs);

	ScalarV_Out operator/(ScalarV_In rhs) const;
	ScalarV_Ref operator/=(ScalarV_In rhs);

	// Logical Operators
	ScalarV_Out operator<<(int nCount) const;
	ScalarV_Ref operator<<=(int nCount);

	ScalarV_Out operator>>(int nCount) const;
	ScalarV_Ref operator>>=(int nCount);

	ScalarV_Out operator&(ScalarV_In rhs) const;
	ScalarV_Ref operator&=(ScalarV_In rhs);

	ScalarV_Out operator|(ScalarV_In rhs) const;
	ScalarV_Ref operator|=(ScalarV_In rhs);

	ScalarV_Out operator~() const;

	ScalarV_Out operator^(ScalarV_In rhs) const;
	ScalarV_Ref operator^=(ScalarV_In rhs);

	// Bit comparison
	bool operator==(ScalarV_In rhs) const;	
	bool operator!=(ScalarV_In rhs) const;

	// Floating Point Comparison
	bool operator<(ScalarV_In rhs) const;
	bool operator<=(ScalarV_In rhs) const;
	bool operator>(ScalarV_In rhs) const;
	bool operator>=(ScalarV_In rhs) const;

	operator bool() const;
};

#include "ScalarV.inl"

#endif // SSE_AVAILABLE

#endif // SCALARV_H