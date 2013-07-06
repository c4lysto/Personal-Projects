#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
private:
	unsigned int m_unFPS;
	unsigned int m_unFrames;
	double m_dFrameTime;
	double m_dDeltaTime;
	LARGE_INTEGER m_lnFrequency;
	LARGE_INTEGER m_lnPrevTime;
	LARGE_INTEGER m_lnCurrTime;

public:
	Timer();
	~Timer(){}

	void Reset();
	// Used to update EVERYTHING
	void UpdateAll();

	// updates Just the timer and deltaTime
	void UpdateTimer();

	float GetDeltaTimeF() const {return float(m_dDeltaTime);}
	double GetDeltaTimeD() const {return m_dDeltaTime;}

	unsigned int GetFPS() const {return m_unFPS;}

	void SeedRand(void);

	// Use these for More of a Manual Timer
	// Updates m_fDeltaTime
	// Does NOT update FPS
	inline void StartTimer() {QueryPerformanceCounter(&m_lnPrevTime);}
	inline void StopTimer() {QueryPerformanceCounter(&m_lnCurrTime);
							 m_dDeltaTime = (m_lnCurrTime.QuadPart - m_lnPrevTime.QuadPart) /
								 double(m_lnFrequency.QuadPart);}
};

#endif