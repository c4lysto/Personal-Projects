#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Debugging/DebugHelp.h"

template<typename Type>
class SmartPointer
{
	typedef Type TypeVal;
	typedef Type& TypeRef;
	typedef const Type& TypeConstRef;
	typedef Type* TypePtr;
	typedef const Type* TypeConstPtr;

private:
	TypePtr m_pType;
	unsigned int m_unRef;

	inline void AddRef()
	{
		if(m_pType)
			++m_unRef;
	}

	inline void Release()
	{
		if(m_pType && --m_unRef < 1)
			delete m_pType;
	}

	// Be Smart When Calling This, as it is unsafe and should only be called
	// if it has been verfied that it is safe to do so.
	// ex: m_pType == nullptr
	__forceinline void Set(Type* pPtr)
	{
		m_pType = pPtr;
		m_unRef = 0;
	}

public:
	SmartPointer() : m_pType(nullptr), m_unRef(1) {}
	~SmartPointer() 
	{
		if(m_pType)
			delete m_pType;

		m_unRef = 0;
	}

	// Don't Allow Bad Things!!!
	SmartPointer(SmartPointer& smartPtr);
	TypeRef operator=(SmartPointer& smartPtr);

	TypeRef operator=(Type* pPtr)
	{
		Assert(m_pType, "Smart Pointer - Attempting To Set A Smart Pointer When It Already Has a Valid Value!!! Unexpected Results!");
		Set(pPtr);
	}

	TypePtr operator Type*()
	{
		return m_pType;
	}

	TypeRef operator*()
	{
		Assert(m_pType, "Smart Pointer - Dereferencing a NULL pointer!");
		return *m_pType;
	}

	TypeConstRef operator*() const
	{
		Assert(m_pType, "Smart Pointer - Dereferencing a NULL pointer!");
		return *m_pType;
	}

	TypeRef operator->()
	{
		Assert(m_pType, "Smart Pointer - Dereferencing a NULL pointer!");
		return *m_pType;
	}

	TypeConstRef operator->() const
	{
		Assert(m_pType, "Smart Pointer - Dereferencing a NULL pointer!");
		return *m_pType;
	}
};

#endif // SMARTPOINTER_H