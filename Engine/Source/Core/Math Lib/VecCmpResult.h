#ifndef VEC_CMP_RESULT_H
#define VEC_CMP_RESULT_H

class VecCmpResult
{
private:
	Vec4V m_VectorMask;
	s32 m_Result;

public:
	explicit VecCmpResult(const s32& result) : m_Result(result) {}
	explicit VecCmpResult(const bool& bX)
	{
		m_Result = (bX << (int)VecElem::X);
	}
	VecCmpResult(const bool& bX, const bool& bY)
	{
		m_Result = (bX << (int)VecElem::X) | (bY << (int)VecElem::Y);
	}
	VecCmpResult(const bool& bX, const bool& bY, const bool& bZ)
	{
		m_Result = (bX << (int)VecElem::X) | (bY << (int)VecElem::Y) | (bZ << (int)VecElem::Z);
	}
	VecCmpResult(const bool& bX, const bool& bY, const bool& bZ, const bool bW)
	{
		m_Result = (bX << (int)VecElem::X) | (bY << (int)VecElem::Y) | (bZ << (int)VecElem::Z) | (bW << (int)VecElem::W);
	}
	VecCmpResult(s32&& result) : m_Result(move(result)) {}

	explicit operator Vec2f() const { return *reinterpret_cast<Vec2f*>(&m_VectorMask); }
	explicit operator Vec3f() const { return *reinterpret_cast<Vec3f*>(&m_VectorMask); }
	explicit operator Vec4f() const { return *reinterpret_cast<Vec4f*>(&m_VectorMask); }

#if SSE_AVAILABLE
	explicit operator Vec2V() const { return Vec2V(m_VectorMask.GetVector()); }
	explicit operator Vec3V() const { return Vec3V(m_VectorMask.GetVector()); }
	explicit operator Vec4V() const { return Vec4V(m_VectorMask.GetVector()); }
	explicit operator Vector() const { return m_VectorMask.GetVector(); }
#endif // SSE_AVAILABLE


	operator s32 () const {return m_Result;}

	bool IsTrueAny() const {return m_Result != 0;}
	bool IsTrueAll() const {return (m_Result & ((1 << (int)VecElem::X) | (1 << (int)VecElem::Y) | (1 << (int)VecElem::Z) | (1 << (int)VecElem::W))) != 0;}

	template<VecElem index>
	__forceinline bool IsTrue() const { CompileTimeAssert(index > = VecElem::X && index <= VecElem::W, "Invalid VecCmpResult Index!"); return (m_Result & (1 << index)) != 0; }

	template<VecElem index0, VecElem index1>
	__forceinline bool IsTrue() const 
	{ 
		CompileTimeAssert((index0 >= VecElem::X && index0 <= VecElem::W) &&
			(index1 >= VecElem::X && index1 <= VecElem::W), "Invalid VecCmpResult Index!"); 
		return (m_Result & ((1 << index0) | (1 << index1))) != 0; 
	}

	template<VecElem index0, VecElem index1, VecElem index2>
	bool IsTrue() const 
	{ 
		CompileTimeAssert((index0 >= VecElem::X && index0 <= VecElem::W) &&
			(index1 >= VecElem::X && index1 <= VecElem::W) &&
			(index2 >= VecElem::X && index2 <= VecElem::W), "Invalid VecCmpResult Index!"); 
		return (m_Result & ((1 << index0) | (1 << index1) | (1 << index2))) != 0; 
	}
};

#endif // VEC_CMP_RESULT_H