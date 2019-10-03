#ifndef ENGINETIMER_H
#define ENGINETIMER_H

#include "Timer.h"

class EngineTimer : public Timer {
	

public:
	// The engine timer will not use the move and copy operators, so we will delete them
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
	float GetDeltaTime() const;	// When retreiving delta time we convert 'tick-count' to microseconds before dividing by 'ticks-per-second' to avoid a loss in percision.
	float GetSleepTime(const unsigned int fps_);

private:

	float prevTicks, currentTicks;
	unsigned int fps;


};

#endif