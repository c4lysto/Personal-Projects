#ifndef FPSTIMER_H
#define FPSTIMER_H

#include "Timer.h"

class FPSTimer : public Timer
{
private:
	unsigned int m_unFPS;
	unsigned int m_unFrames;
	double m_dFrameTime;

public:
	FPSTimer() 
	{
		m_unFPS = m_unFrames = 0;
		m_dFrameTime = 0;
	}

	virtual ~FPSTimer() {}

	virtual void Reset() 
	{
		Timer::Reset(); 

		m_unFPS = m_unFrames = 0;
		m_dFrameTime = 0;
	}

	virtual void Update()
	{
		Timer::Update();

		++m_unFrames;
		m_dFrameTime += m_dDeltaTime;

		if(m_dFrameTime >= 1.0f)
		{
			m_unFPS = m_unFrames;
			m_dFrameTime = 0.0f;
			m_unFrames = 0;
		}
	}

	unsigned int GetFPS() const {return m_unFPS;}
};

#endif