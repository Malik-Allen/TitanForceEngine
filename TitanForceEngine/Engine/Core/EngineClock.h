#ifndef ENGINECLOCK_H
#define ENGINECLOCK_H

#include "High-ResTimer.h"

class EngineClock : public HighResTimer {
	

public:
	EngineClock(const EngineClock&) = delete;
	EngineClock& operator=(const EngineClock&) = delete;
	EngineClock(EngineClock&&) = delete;
	EngineClock& operator=(EngineClock&&) = delete;

	EngineClock();
	~EngineClock();

	unsigned int GetFPS() { return m_fps; }
	void SetFPS(unsigned int fps);

	void Reset();
	void UpdateFrameTicks();
	float GetDeltaTime() const;	
	unsigned int GetSleepTime(const unsigned int fps);
	float GetCurrentTicks() const;

private:

	unsigned int m_prevTicks, m_currentTicks, m_fps;


};

#endif // !ENGINECLOCK_H