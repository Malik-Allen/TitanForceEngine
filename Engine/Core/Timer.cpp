#include "Timer.h"

Timer::Timer():
	currentTicks()
{
	QueryPerformanceFrequency(&frequency);			// Retreives the frequency of the performance counter 'Ticks-per-second'
}

Timer::~Timer(){}

unsigned int Timer::GetCurrentTimeInMicroSeconds() {
	QueryPerformanceCounter(&currentTicks);			// Retrieves the current value of the performance counter 'Ticks'
	currentTicks.QuadPart *= SECONDS_TO_MICROSECONDS;				// Conversion to microseconds to avoid a loss of percision when divinding by frequency
	currentTicks.QuadPart /= frequency.QuadPart;	// Division by 'Ticks-per-second'
	
	return static_cast<unsigned int>(currentTicks.QuadPart);
}

unsigned int Timer::GetCurrentTimeInMilliSeconds() {
	QueryPerformanceCounter(&currentTicks);
	currentTicks.QuadPart *= SECONDS_TO_MILLISECONDS;					
	currentTicks.QuadPart /= frequency.QuadPart;

	return static_cast<unsigned int>(currentTicks.QuadPart);
}
