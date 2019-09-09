#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer {

public:

	// The timer class will not need the copy or move operators
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();

	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	float GetCurrentTime() const;
	unsigned int GetSleepTime(const unsigned int fps_);
	

private:

	unsigned int prevTicks;
	unsigned int currentTicks;


};


#endif