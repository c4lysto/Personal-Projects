#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>
#include <time.h>

class Timer
{
private:
	LARGE_INTEGER m_lnFrequency;
	LARGE_INTEGER m_lnPrevTime;
	LARGE_INTEGER m_lnCurrTime;

protected:
	double m_dDeltaTime;

public:
	Timer()
	{
		srand((unsigned int)time(0));
		m_dDeltaTime = 0.0;
	}

	virtual ~Timer(){}

	virtual void Reset()
	{
		QueryPerformanceFrequency(&m_lnFrequency);
		QueryPerformanceCounter(&m_lnPrevTime);
	}

	inline virtual void Update()
	{
		QueryPerformanceCounter(&m_lnCurrTime);
		m_dDeltaTime = (m_lnCurrTime.QuadPart - m_lnPrevTime.QuadPart) /
			double(m_lnFrequency.QuadPart);
		m_lnPrevTime = m_lnCurrTime;
	}

	float GetDeltaTimeF() const {return float(m_dDeltaTime);}
	double GetDeltaTimeD() const {return m_dDeltaTime;}

	inline void SeedRand(void)
	{
		srand((unsigned int)time(0));
	}

	// Use these for More of a Manual Timer
	// Updates m_fDeltaTime
	// Does NOT update FPS
	inline void StartTimer() {QueryPerformanceCounter(&m_lnPrevTime);}
	inline void StopTimer() {QueryPerformanceCounter(&m_lnCurrTime);
							 m_dDeltaTime = (m_lnCurrTime.QuadPart - m_lnPrevTime.QuadPart) /
								 double(m_lnFrequency.QuadPart);}
};

#endif