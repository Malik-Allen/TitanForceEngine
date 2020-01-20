#include "EngineTimer.h"
#include <iostream>

EngineTimer::EngineTimer():
	Timer(),
	prevTicks(0),
	currentTicks(0),
	fps(120)
{
	EngineTimer::Reset();
}

EngineTimer::~EngineTimer() {}

void EngineTimer::SetFPS(unsigned int fps_) { fps = fps_;}

void EngineTimer::Reset() {
	prevTicks =  currentTicks = Timer::GetCurrentTimeInMilliSeconds();
}

void EngineTimer::UpdateFrameTicks() {
	prevTicks = currentTicks;
	currentTicks = Timer::GetCurrentTimeInMilliSeconds();
}

float EngineTimer::GetDeltaTime() const {
	return static_cast<float>(currentTicks - prevTicks) * MILLISECONDS_TO_SECONDS;	// Conversion to seconds
}

float EngineTimer::GetCurrentTicks() const {
	return static_cast<float>(currentTicks) * MILLISECONDS_TO_SECONDS;
}

unsigned int EngineTimer::GetSleepTime(const unsigned int fps_) {
	unsigned int milliSecsPerFrame = SECONDS_TO_MILLISECONDS / fps_;

	if (milliSecsPerFrame == 0)
		return 0;

	unsigned int sleepTime = milliSecsPerFrame - Timer::GetCurrentTimeInMilliSeconds();

	if (sleepTime > milliSecsPerFrame)
		return milliSecsPerFrame;

	return sleepTime;
}