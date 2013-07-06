#include "Timer.h"
#include <time.h>

Timer::Timer()
{
	srand((unsigned int)time(0));

	m_unFPS = m_unFrames = 0;
	m_dFrameTime = m_dDeltaTime = 0.0;
}

void Timer::Reset()
{
	QueryPerformanceFrequency(&m_lnFrequency);

	QueryPerformanceCounter(&m_lnPrevTime);

	m_unFPS = 0;
}

void Timer::UpdateAll()
{
	++m_unFrames;

	if(m_dFrameTime >= 1.0f)
	{
		m_unFPS = m_unFrames;
		m_dFrameTime = 0.0f;
		m_unFrames = 0;
	}

	QueryPerformanceCounter(&m_lnCurrTime);
	m_dDeltaTime = (m_lnCurrTime.QuadPart - m_lnPrevTime.QuadPart) /
					double(m_lnFrequency.QuadPart);
	m_lnPrevTime = m_lnCurrTime;

	m_dFrameTime += m_dDeltaTime;
}

void Timer::UpdateTimer()
{
	QueryPerformanceCounter(&m_lnCurrTime);
	m_dDeltaTime = (m_lnCurrTime.QuadPart - m_lnPrevTime.QuadPart) /
					double(m_lnFrequency.QuadPart);
	m_lnPrevTime = m_lnCurrTime;

	m_dFrameTime += m_dDeltaTime;
}

void Timer::SeedRand(void)
{
	srand((unsigned int)time(0));
}