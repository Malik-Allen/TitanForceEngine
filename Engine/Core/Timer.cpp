#include "Timer.h"

Timer::Timer():
	fps(60)
	{}

Timer::~Timer(){}

void Timer::Start() {
	QueryPerformanceFrequency(&frequency);	// Retreives the frequency of the performance counter 'Ticks-per-second'
	QueryPerformanceCounter(&prevTicks);	// Retrieves the current value of the performance counter 'Ticks'
	QueryPerformanceCounter(&currentTicks);
}

void Timer::UpdateFrameTicks() {
	prevTicks = currentTicks;
	QueryPerformanceCounter(&currentTicks);
}

float Timer::GetDeltaTime() const {
	LARGE_INTEGER elapsedTime;
	elapsedTime.QuadPart = currentTicks.QuadPart - prevTicks.QuadPart;
	elapsedTime.QuadPart *= 1000000;	// Conversion to microseconds to avoid a loss of percision when divinding by frequency
	elapsedTime.QuadPart /= frequency.QuadPart;	// Division by 'Ticks-per-second'
	return static_cast<float>(elapsedTime.QuadPart) / 1000000.0f;	// Conversion to seconds
}

unsigned int Timer::GetSleepTime(const unsigned int fps_) {
	unsigned int milliSecsPerFrame = 1000 / fps_;
	
	if (milliSecsPerFrame = 0)
		return 0;

	LARGE_INTEGER elapsedTime;
	elapsedTime.QuadPart = currentTicks.QuadPart - prevTicks.QuadPart;
	elapsedTime.QuadPart *= 1000000;
	elapsedTime.QuadPart /= frequency.QuadPart;
	
	unsigned int sleepTime = milliSecsPerFrame - static_cast<unsigned int>(static_cast<float>(elapsedTime.QuadPart));
	
	if (sleepTime > milliSecsPerFrame)
		return milliSecsPerFrame;

	return sleepTime;
}

void Timer::SetFPS(const unsigned int newFPS) {
	fps = newFPS;
}