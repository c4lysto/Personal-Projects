#ifndef FUNCTIONPOINTER_H
#define FUNCTIONPOINTER_H

template<typename retType, typename... tArgs>
class _FuncPtrBase
{
public:
	virtual inline retType operator()(tArgs... args) const = 0;
	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const = 0;
};

template<typename retType, typename... tArgs>
class StaticFunctionPointer : public _FuncPtrBase<retType, tArgs...>
{
	typedef retType(*_FuncPtr)(tArgs...);

protected:
	_FuncPtr m_pFunc;

public:
	StaticFunctionPointer() : m_pFunc(nullptr) {}
	StaticFunctionPointer(const StaticFunctionPointer& rhs) : m_pFunc(rhs.m_pFunc) {}
	StaticFunctionPointer(StaticFunctionPointer&& rhs) : m_pFunc(rhs.m_pFunc) {}
	StaticFunctionPointer(const _FuncPtr& pFunc) : m_pFunc(pFunc) {}
	StaticFunctionPointer(_FuncPtr&& pFunc) : m_pFunc(pFunc) {}


	StaticFunctionPointer& operator=(const StaticFunctionPointer& rhs)
	{
		if(this != &rhs)
		{
			m_pFunc = rhs.m_pFunc;
		}
		return *this;
	}
	StaticFunctionPointer& operator=(StaticFunctionPointer&& rhs)
	{
		if(this != &rhs)
		{
			m_pFunc = rhs.m_pFunc;
		}
		return *this;
	}
	
	StaticFunctionPointer& operator=(const _FuncPtr& pFunc)
	{
		m_pFunc = pFunc;
		return *this;
	}
	StaticFunctionPointer& operator=(_FuncPtr&& pFunc)
	{
		m_pFunc = pFunc;
		return *this;
	}

	operator _FuncPtr()
	{
		return m_pFunc;
	}

	virtual __forceinline retType operator()(tArgs... args) const
	{
		return (m_pFunc)(args...);
	}

	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const
	{
		return new StaticFunctionPointer<retType, tArgs...>(*this);
	}
};

template<typename retType, typename className, typename... tArgs>
class MemberFunctionPointer : public _FuncPtrBase<retType, tArgs...>
{
	typedef retType(className::*_FuncPtr)(tArgs...);
	typedef retType(className::*_FuncPtrConst)(tArgs...) const;

private:
	_FuncPtr m_pFunc;
	className*  m_pInvokingObject;

public:
	MemberFunctionPointer() : m_pFunc(nullptr), m_pInvokingObject(nullptr) {}
	MemberFunctionPointer(const MemberFunctionPointer& rhs) : m_pFunc((_FuncPtr)rhs.m_pFunc), m_pInvokingObject(rhs.m_pInvokingObject) {}
	MemberFunctionPointer(MemberFunctionPointer&& rhs) : m_pFunc((_FuncPtr)rhs.m_pFunc), m_pInvokingObject(rhs.m_pInvokingObject) {}
	MemberFunctionPointer(const _FuncPtr& pFunc) : m_pFunc(pFunc), m_pInvokingObject(nullptr) {}
	MemberFunctionPointer(_FuncPtr&& pFunc) : m_pFunc(pFunc), m_pInvokingObject(nullptr) {}
	MemberFunctionPointer(className* pInvokinObject, const _FuncPtr& pFunc) : m_pFunc(pFunc), m_pInvokingObject(pInvokinObject) {}
	MemberFunctionPointer(className* pInvokinObject, _FuncPtr&& pFunc) : m_pFunc(pFunc), m_pInvokingObject(pInvokinObject) {}
	MemberFunctionPointer(const className* pInvokinObject, const _FuncPtrConst& pFunc) : m_pFunc((_FuncPtr)pFunc), m_pInvokingObject((className*)pInvokinObject) {}
	MemberFunctionPointer(const className* pInvokinObject, _FuncPtrConst&& pFunc) : m_pFunc((_FuncPtr)pFunc), m_pInvokingObject((className*)pInvokinObject) {}


	MemberFunctionPointer& operator=(const MemberFunctionPointer& rhs)
	{
		if(this != &rhs)
		{
			m_pFunc = rhs.m_pFunc;
			m_pInvokingObject = rhs.m_pInvokingObject;
		}
		return *this;
	}

	MemberFunctionPointer& operator=(MemberFunctionPointer&& rhs)
	{
		if(this != &rhs)
		{
			m_pFunc = rhs.m_pFunc;
			m_pInvokingObject = rhs.m_pInvokingObject;
		}
		return *this;
	}

	MemberFunctionPointer& operator=(const _FuncPtr& pFunc)
	{
		m_pFunc = pFunc;
		return *this;
	}

	MemberFunctionPointer& operator=(_FuncPtr&& pFunc)
	{
		m_pFunc = pFunc;
		return *this;
	}

	void SetInvokingObject(className* pInvokingObject)
	{
		m_pInvokingObject = pInvokingObject;
	}

	operator _FuncPtr()
	{
		return m_pFunc;
	}

	virtual __forceinline retType operator()(tArgs... args) const 
	{
		return (m_pInvokingObject->*m_pFunc)(args...);
	}

	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const
	{
		return new MemberFunctionPointer<retType, className, tArgs...>(*this);
	}
};

template<typename retType, typename... tArgs>
class FunctionPointer
{
	typedef retType(*_StaticFuncPtr)(tArgs...);

private:
	_FuncPtrBase<retType, tArgs...>* m_pFuncWrapper;

private:
	inline void Clean()
	{
		if(m_pFuncWrapper)
		{
			delete m_pFuncWrapper;
			m_pFuncWrapper = nullptr;
		}
	}

public:
	FunctionPointer() : m_pFuncWrapper(nullptr) {}
	FunctionPointer(_StaticFuncPtr pFuncPtr)
	{
		m_pFuncWrapper = StaticFunctionPointer<retType, tArgs...>(pFuncPtr).CreateCopy();
	}
	FunctionPointer(const FunctionPointer& rhs)
	{
		m_pFuncWrapper = rhs.m_pFuncWrapper ? rhs.m_pFuncWrapper->CreateCopy() : nullptr;
	}
	FunctionPointer(FunctionPointer&& rhs) : m_pFuncWrapper(rhs.m_pFuncWrapper)
	{
		m_pFuncWrapper = rhs.m_pFuncWrapper;
		rhs.m_pFuncWrapper = nullptr;
	}
	FunctionPointer(const StaticFunctionPointer<retType, tArgs...>& rhs)
	{
		m_pFuncWrapper = rhs.CreateCopy();
	}
	
	template<typename className>
	FunctionPointer(const MemberFunctionPointer<retType, className, tArgs...>& rhs)
	{
		m_pFuncWrapper = rhs.CreateCopy();
	}
	~FunctionPointer() {Clean();}

	FunctionPointer& operator=(const FunctionPointer& rhs)
	{
		if(this != &rhs)
		{
			Clean();

			if(rhs.m_pFuncWrapper)
			{
				m_pFuncWrapper = rhs.m_pFuncWrapper->CreateCopy();
			}
		}
		return *this;
	}

	FunctionPointer& operator=(FunctionPointer&& rhs)
	{
		if(this != &rhs)
		{
			Clean();

			m_pFuncWrapper = rhs.m_pFuncWrapper;
			rhs.m_pFuncWrapper = nullptr;
		}
		return *this;
	}

	FunctionPointer& operator=(_StaticFuncPtr pFuncPtr)
	{
		Clean();
		m_pFuncWrapper = StaticFunctionPointer<retType, tArgs...>(pFuncPtr).CreateCopy();
		return *this;
	}

	FunctionPointer& operator=(const StaticFunctionPointer<retType, tArgs...>& rhs)
	{
		Clean();
		m_pFuncWrapper = rhs.CreateCopy();
		return *this;
	}
	
	template<typename className>
	FunctionPointer& operator=(const MemberFunctionPointer<retType, className, tArgs...>& rhs)
	{
		Clean();
		m_pFuncWrapper = rhs.CreateCopy();
		return *this;
	}

	retType operator()(tArgs... args)
	{
		Assert(m_pFuncWrapper, "Function Pointer - Calling an Invalid Function Pointer!");
		return (m_pFuncWrapper->operator()(args...));
	}

	bool IsNull() const
	{
		return m_pFuncWrapper == nullptr;
	}
};

template<typename retType, typename... tArgs>
__forceinline StaticFunctionPointer<retType, tArgs...> CreateFunctionPointer(retType(*funcPtr)(tArgs...))
{
	return StaticFunctionPointer<retType, tArgs...>(funcPtr);
}

template<typename retType, typename className, typename... tArgs>
__forceinline MemberFunctionPointer<retType, className, tArgs...> CreateFunctionPointer(className* pInvokingObject, retType(className::*funcPtr)(tArgs...))
{
	return MemberFunctionPointer<retType, className, tArgs...>(pInvokingObject, funcPtr);
}

template<typename retType, typename className, typename... tArgs>
__forceinline MemberFunctionPointer<retType, className, tArgs...> CreateFunctionPointer(const className* pInvokingObject, retType(className::*funcPtr)(tArgs...) const)
{
	return MemberFunctionPointer<retType, className, tArgs...>(pInvokingObject, funcPtr);
}

#endif // FUNCTIONPOINTER_H