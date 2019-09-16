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

	float GetCurrentTimeInMilliSeconds();

private:

	LARGE_INTEGER frequency;	// 'Ticks-per-second' 
	LARGE_INTEGER currentTicks;

	//**IMPORTANT: LARGE_INTEGER is a union that has member value called 'QuadPart' which stores a 64bit signed int
		// 'QuadPart' should be used for a compiler with support for 64-bit integers, where as 'HighPart' and 'LowPart' should be used otherwise.

};


#endif