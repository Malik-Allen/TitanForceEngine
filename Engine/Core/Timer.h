#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

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
	float GetDeltaTime() const;	// When retreiving delta time we convert 'tick-count' to microseconds before dividing by 'ticks-per-second' to avoid a loss in percision.
	unsigned int GetSleepTime(const unsigned int fps_);

	void SetFPS(const unsigned int fps_);
	unsigned int GetFPS() const { return fps; }

private:

	unsigned int fps;

	LARGE_INTEGER frequency;	// 'Ticks-per-second' 
	LARGE_INTEGER prevTicks, currentTicks;

	//**IMPORTANT: LARGE_INTEGER is a union that has member value called 'QuadPart' which stores a 64bit signed int
		// 'QuadPart' should be used for a compiler with support for 64-bit integers, where as 'HighPart' and 'LowPart' should be used otherwise.

};


#endif