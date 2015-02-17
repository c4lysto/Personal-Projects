#ifndef FUNCTIONPOINTER_H
#define FUNCTIONPOINTER_H

template<typename retType, typename... tArgs>
class _FuncPtrBase
{
public:
	virtual inline retType operator()(tArgs... args) const = 0;
	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const = 0;
};

template<typename funcPtrType, typename retType, typename... tArgs>
class _DeferredFuncPtr : public _FuncPtrBase<retType, tArgs...>
{
public:
	funcPtrType m_pFunc;

public:
	_DeferredFuncPtr(const _DeferredFuncPtr& rhs) : m_pFunc(rhs.m_pFunc){}
	_DeferredFuncPtr(_DeferredFuncPtr&& rhs) : m_pFunc(rhs.m_pFunc) {}
	_DeferredFuncPtr(const funcPtrType& pFunc) : m_pFunc(pFunc) {}

	virtual __forceinline retType operator()(tArgs... args) const
	{
		return (m_pFunc)(args...);
	}

	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const
	{
		return new _DeferredFuncPtr<funcPtrType, retType, tArgs...>(*this);
	}
};

template<typename funcPtrType, typename retType, typename className, typename... tArgs>
class _DeferredMemberFuncPtr : public _DeferredFuncPtr<funcPtrType, retType, tArgs...>
{
protected:
	className* m_pInvokingObject;

public:
	_DeferredMemberFuncPtr(const _DeferredMemberFuncPtr& rhs) : _DeferredFuncPtr(rhs.m_pFunc), m_pInvokingObject(rhs.m_pInvokingObject) {}
	_DeferredMemberFuncPtr(_DeferredMemberFuncPtr&& rhs) : _DeferredFuncPtr(rhs.m_pFunc), m_pInvokingObject(rhs.m_pInvokingObject) {}
	_DeferredMemberFuncPtr(className* pInvokingObject, const funcPtrType& pFunc) : _DeferredFuncPtr(pFunc), m_pInvokingObject(pInvokingObject) {}
	_DeferredMemberFuncPtr(className* pInvokingObject, funcPtrType&& pFunc) : _DeferredFuncPtr(pFunc), m_pInvokingObject(pInvokingObject) {}
	_DeferredMemberFuncPtr(const funcPtrType& pFunc) : m_pFunc(pFunc) {}

	virtual __forceinline retType operator()(tArgs... args) const
	{
		return (m_pInvokingObject->*m_pFunc)(args...);
	}

	virtual _FuncPtrBase<retType, tArgs...>* CreateCopy() const
	{
		return new _DeferredMemberFuncPtr<funcPtrType, retType, className, tArgs...>(*this);
	}
};

// Wrapper Around __cdecl Calling Convention Function Pointer
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

	template<typename funcPtrType>
	StaticFunctionPointer(const _DeferredFuncPtr<funcPtrType, retType, tArgs...>& rhs) : m_pFunc(rhs.m_pFunc) {}

	template<typename funcPtrType>
	StaticFunctionPointer(_DeferredFuncPtr<funcPtrType, retType, tArgs...>&& rhs) : m_pFunc(rhs.m_pFunc) {}


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

	template<typename funcPtrType>
	StaticFunctionPointer& operator=(const _DeferredFuncPtr<funcPtrType, retType, tArgs...>& rhs)
	{
		m_pFunc = rhs.m_pFunc;
		return *this;
	}
	
	template<typename funcPtrType>
	StaticFunctionPointer& operator=(_DeferredFuncPtr<funcPtrType, retType, tArgs...>&& rhs)
	{
		m_pFunc = rhs.m_pFunc;
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
	MemberFunctionPointer(className* pInvokinObject, const _FuncPtr& pFunc) : m_pFunc(pFunc), m_pInvokingObject(pInvokinObject) {}
	MemberFunctionPointer(className* pInvokinObject, _FuncPtr&& pFunc) : m_pFunc(pFunc), m_pInvokingObject(pInvokinObject) {}
	MemberFunctionPointer(const className* pInvokinObject, const _FuncPtrConst& pFunc) : m_pFunc((_FuncPtr)pFunc), m_pInvokingObject((className*)pInvokinObject) {}
	MemberFunctionPointer(const className* pInvokinObject, _FuncPtrConst&& pFunc) : m_pFunc((_FuncPtr)pFunc), m_pInvokingObject((className*)pInvokinObject) {}

	template<typename funcPtrType>
	MemberFunctionPointer(const _DeferredMemberFuncPtr<funcPtrType, retType, className, tArgs...>& rhs) : m_pFunc(rhs.m_pFunc){}

	template<typename funcPtrType>
	MemberFunctionPointer(_DeferredMemberFuncPtr<funcPtrType, retType, className, tArgs...>&& rhs) : m_pFunc(rhs.m_pFunc){}


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

	template<typename funcPtrType>
	StaticFunctionPointer& operator=(const _DeferredMemberFuncPtr<funcPtrType, retType, tArgs...>& rhs)
	{
		m_pFunc = rhs.m_pFunc;
		return *this;
	}

	template<typename funcPtrType>
	StaticFunctionPointer& operator=(_DeferredMemberFuncPtr<funcPtrType, retType, tArgs...>&& rhs)
	{
		m_pFunc = rhs.m_pFunc;
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

	template<typename funcPtrWrapperType>
	inline void Assign(funcPtrWrapperType rhs)
	{
		Clean();
		m_pFuncWrapper = rhs.CreateCopy();
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

	template<typename funcPtrType>
	FunctionPointer(const _DeferredFuncPtr<funcPtrType, retType, tArgs...>& rhs)
	{
		m_pFuncWrapper = rhs.CreateCopy();
	}

	template<typename funcPtrType, typename className>
	FunctionPointer(const _DeferredFuncPtr<funcPtrType, retType, className, tArgs...>& rhs)
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
		Assign(StaticFunctionPointer<retType, tArgs...>(pFuncPtr));
		return *this;
	}

	FunctionPointer& operator=(const StaticFunctionPointer<retType, tArgs...>& rhs)
	{
		Assign(rhs);
		return *this;
	}
	
	template<typename className>
	FunctionPointer& operator=(const MemberFunctionPointer<retType, className, tArgs...>& rhs)
	{
		Assign(rhs);
		return *this;
	}

	template<typename funcPtrType>
	FunctionPointer& operator=(const _DeferredFuncPtr<funcPtrType, retType, tArgs...>& rhs)
	{
		Assign(rhs);
		return *this;
	}

	template<typename funcPtrType, typename className>
	FunctionPointer& operator=(const _DeferredFuncPtr<funcPtrType, retType, className, tArgs...>& rhs)
	{
		Assign(rhs);
		return *this;
	}

	__forceinline retType operator()(tArgs... args) const
	{
		Assert(m_pFuncWrapper, "Function Pointer - Calling an Invalid Function Pointer!");
		return (m_pFuncWrapper->operator()(args...));
	}

	bool IsNull() const
	{
		return m_pFuncWrapper == nullptr;
	}
};

#pragma warning(disable : 4003) // Not Enough Actual Parameters For Macro '...'

#define CREATE_FUNC_PTR_WRAP(createFuncWrap, callConv, constQualifier) createFuncWrap(callConv, constQualifier)

#if _WIN64
#define CREATE_FUNC_PTR_WRAP_ALL(createFuncWrap, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __cdecl, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __vectorcall, constQualifier) //\
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __clrcall, constQualifier)
#else // if !_WIN64
#define CREATE_FUNC_PTR_WRAP_ALL(createFuncWrap, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __stdcall, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __cdecl, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __fastcall, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __vectorcall, constQualifier) \
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __thiscall, constQualifier) //\
		CREATE_FUNC_PTR_WRAP(createFuncWrap, __clrcall, constQualifier)
#endif // !_WIN64

// START - Static Function Pointers

#define CREATE_STATIC_FUNC_WRAP(callConv, constQualifier) \
template<typename retType, typename... tArgs> \
__forceinline _DeferredFuncPtr<retType(callConv *)(tArgs...), retType, tArgs...> \
			CreateFunctionPointer(retType(callConv *funcPtr)(tArgs...)) \
{ \
	return _DeferredFuncPtr<retType(callConv *)(tArgs...), retType, tArgs...>(funcPtr); \
}

// Lambda Support
//template<typename funcPtrType, typename... tArgs> \
//__forceinline _DeferredFuncPtr<

CREATE_FUNC_PTR_WRAP_ALL(CREATE_STATIC_FUNC_WRAP, )
#undef CREATE_STATIC_FUNC_WRAP

// END - Static Function Pointers

#define CREATE_MEMBER_FUNC_WRAP(callConv, constQualifier) \
template<typename retType, typename className, typename... tArgs> \
__forceinline _DeferredMemberFuncPtr<retType(callConv className::*)(tArgs...) constQualifier, retType, className, tArgs...> \
			CreateFunctionPointer(constQualifier className* pInvokingObject, retType(callConv className::*funcPtr)(tArgs...) constQualifier) \
{ \
	return _DeferredMemberFuncPtr<retType(callConv className::*)(tArgs...) constQualifier, retType, className, tArgs...>(pInvokingObject, funcPtr); \
}

CREATE_FUNC_PTR_WRAP_ALL(CREATE_MEMBER_FUNC_WRAP, )
CREATE_FUNC_PTR_WRAP_ALL(CREATE_MEMBER_FUNC_WRAP, const)
#undef CREATE_MEMBER_FUNC_WRAP

#undef CREATE_FUNC_PTR_WRAP_ALL
#undef CREATE_FUNC_PTR_WRAP

#pragma warning(default : 4003)

#endif // FUNCTIONPOINTER_H