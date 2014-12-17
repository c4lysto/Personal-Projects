#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H
#include "UtilitiesInclude.h"

class CriticalSection
{
private:
	CRITICAL_SECTION m_CriticalSection;

public:
	inline CriticalSection(DWORD dwSpinCount = 1000)
	{
		InitializeCriticalSectionAndSpinCount(&m_CriticalSection, dwSpinCount);
	}

	inline ~CriticalSection()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}

	// Undefined to avoid bad things.
	CriticalSection(const CriticalSection& rhs) = delete;
	CriticalSection& operator=(const CriticalSection& rhs) = delete;

	inline void Lock()
	{
		EnterCriticalSection(&m_CriticalSection);
	}

	inline bool TryLock()
	{
		return TryEnterCriticalSection(&m_CriticalSection) ? true : false;
	}

	inline void Unlock()
	{
		if(m_CriticalSection.OwningThread == (void*)GetCurrentThreadId())
			LeaveCriticalSection(&m_CriticalSection);
	}
};

// This Class Is Meant For An Easy Wrapper Of A Scoped Critical Section
class LocalCriticalSection
{
private:
	CriticalSection& m_CriticalSection;

public:
	inline LocalCriticalSection(CriticalSection& critSection, bool bAutoLock = true) : m_CriticalSection(critSection)
	{
		if(bAutoLock)
			m_CriticalSection.Lock();
	}

	inline ~LocalCriticalSection()
	{
		Unlock();
	}

	LocalCriticalSection& operator=(const LocalCriticalSection& rhs);

	inline void Lock()
	{
		m_CriticalSection.Lock();
	}

	inline void TryLock()
	{
		m_CriticalSection.TryLock();
	}

	inline void Unlock()
	{
		m_CriticalSection.Unlock();
	}
};
#endif