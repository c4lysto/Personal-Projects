#include "CriticalSection.h"

inline CriticalSection::CriticalSection(DWORD dwSpinCount)
{
	InitializeCriticalSectionAndSpinCount(&m_CriticalSection, dwSpinCount);
}

inline CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_CriticalSection);
}

__forceinline void CriticalSection::Lock()
{
	EnterCriticalSection(&m_CriticalSection);
}

__forceinline bool CriticalSection::TryLock()
{
	return TryEnterCriticalSection(&m_CriticalSection) ? true : false;
}

__forceinline void CriticalSection::Unlock()
{
	if(m_CriticalSection.OwningThread == (HANDLE)GetCurrentThreadId())
		LeaveCriticalSection(&m_CriticalSection);
}

inline ScopedCriticalSection::ScopedCriticalSection(CriticalSection& critSection, bool bAutoLock) : m_CriticalSection(critSection)
{
	if(bAutoLock)
		m_CriticalSection.Lock();
}

inline ScopedCriticalSection::~ScopedCriticalSection()
{
	m_CriticalSection.Unlock();
}