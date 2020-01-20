#ifndef ENGINETIMER_H
#define ENGINETIMER_H

#include "Timer.h"

class EngineTimer : public Timer {
	

public:
	EngineTimer(const EngineTimer&) = delete;
	EngineTimer& operator=(const EngineTimer&) = delete;
	EngineTimer(EngineTimer&&) = delete;
	EngineTimer& operator=(EngineTimer&&) = delete;

	EngineTimer();
	~EngineTimer();

	unsigned int GetFPS() { return fps; }
	void SetFPS(unsigned int fps_);

	void Reset();
	void UpdateFrameTicks();
	float GetDeltaTime() const;	
	unsigned int GetSleepTime(const unsigned int fps_);
	float GetCurrentTicks() const;

private:

	unsigned int prevTicks, currentTicks, fps;


};

#endif