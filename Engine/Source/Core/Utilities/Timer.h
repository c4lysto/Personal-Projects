#ifndef TIMER_H
#define TIMER_H

#include "UtilitiesInclude.h"

class TimeCounter
{
	friend class Timer;

private:
	static s64 ms_CPUFrequency;
	static float ms_ConversionToSeconds;
	static float ms_ConversionToMilliseconds;

	static inline void Init()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&ms_CPUFrequency);

		ms_ConversionToSeconds = 1.0f / ms_CPUFrequency;
		ms_ConversionToMilliseconds = ms_ConversionToSeconds * 1000.0f;
	}

private:
	s64 m_Ticks;

public:
	TimeCounter() : m_Ticks(0) {}
	TimeCounter(const TimeCounter& rhs) : m_Ticks(rhs.m_Ticks) {}
	TimeCounter(const s64& rhs) : m_Ticks(rhs) {}
	TimeCounter(s64&& rhs) : m_Ticks(rhs) {}

	const TimeCounter& operator=(const TimeCounter& rhs) {if(this != &rhs){m_Ticks = rhs.m_Ticks;} return *this;}
	const TimeCounter& operator=(TimeCounter&& rhs) {m_Ticks = rhs.m_Ticks; return *this;}

	TimeCounter operator+(const TimeCounter& rhs) const {return TimeCounter(m_Ticks + rhs.m_Ticks);}
	const TimeCounter& operator+=(const TimeCounter& rhs) {m_Ticks += rhs.m_Ticks; return *this;}

	TimeCounter operator-(const TimeCounter& rhs) const {return TimeCounter(m_Ticks - rhs.m_Ticks);}
	const TimeCounter& operator-=(const TimeCounter& rhs) {m_Ticks -= rhs.m_Ticks; return *this;}

	float InSeconds() const {return m_Ticks*ms_ConversionToSeconds;}
	float InMilliseconds() const {return m_Ticks*ms_ConversionToMilliseconds;}

};


class Timer
{
private:
	static TimeCounter ms_PrevTime;
	static TimeCounter ms_CurrTime;
	static float ms_fDeltaTime;

	static void UpdateDeltaTime();

public:

	static void Init();

	static void Update();

	static float GetDeltaTime() {return ms_fDeltaTime;}

	static inline TimeCounter UpdateCounter() {TimeCounter updateTime; QueryPerformanceCounter((LARGE_INTEGER*)&updateTime); return updateTime;}
};

#endif