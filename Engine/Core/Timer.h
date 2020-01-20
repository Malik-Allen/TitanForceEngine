#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

#define MILLISECONDS_TO_SECONDS (1 / 1000.0f)
#define MICROSECONDS_TO_SECONDS (1 / 1000000.0f)
#define SECONDS_TO_MILLISECONDS (1000 / 1)
#define SECONDS_TO_MICROSECONDS (1000000 / 1)


class Timer {

public:

	// The timer class will not need the copy or move operators
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();

	unsigned int GetCurrentTimeInMicroSeconds();
	unsigned int GetCurrentTimeInMilliSeconds();

private:

	LARGE_INTEGER frequency;	// 'Ticks-per-second' 
	LARGE_INTEGER currentTicks;

	//**IMPORTANT: LARGE_INTEGER is a union that has member value called 'QuadPart' which stores a 64bit signed int
		// 'QuadPart' should be used for a compiler with support for 64-bit integers, where as 'HighPart' and 'LowPart' should be used otherwise.

};


#endif