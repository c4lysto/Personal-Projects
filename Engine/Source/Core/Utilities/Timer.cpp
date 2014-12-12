#include "Timer.h"

s64 TimeCounter::ms_CPUFrequency = 0;
float TimeCounter::ms_ConversionToSeconds = 0.0f;
float TimeCounter::ms_ConversionToMilliseconds = 0.0f;

TimeCounter Timer::ms_PrevTime;
TimeCounter Timer::ms_CurrTime;
float Timer::ms_fDeltaTime = 0.0f;

void Timer::Init()
{
	TimeCounter::Init();

	ms_PrevTime = ms_CurrTime = UpdateCounter();
	ms_PrevTime = ms_CurrTime;
}

void Timer::UpdateDeltaTime()
{
	ms_fDeltaTime = (ms_CurrTime - ms_PrevTime).InSeconds();
}

void Timer::Update()
{
	ms_PrevTime = ms_CurrTime;
	ms_CurrTime = UpdateCounter();
	UpdateDeltaTime();
}