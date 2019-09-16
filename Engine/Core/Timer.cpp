#include "Timer.h"

Timer::Timer() {
	QueryPerformanceFrequency(&frequency);			// Retreives the frequency of the performance counter 'Ticks-per-second'
}

Timer::~Timer(){}

float Timer::GetCurrentTimeInMilliSeconds() {
	QueryPerformanceCounter(&currentTicks);			// Retrieves the current value of the performance counter 'Ticks'
	currentTicks.QuadPart *= 1000000;				// Conversion to microseconds to avoid a loss of percision when divinding by frequency
	currentTicks.QuadPart /= frequency.QuadPart;	// Division by 'Ticks-per-second'

	return static_cast<float>(currentTicks.QuadPart);
}
