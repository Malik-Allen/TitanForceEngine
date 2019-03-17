#include "Timer.h"

Timer::Timer() : prevTicks(0), currentTicks(0) {
}

Timer::~Timer() {}

void Timer::Start() {
	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks() {
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {
	return static_cast<float>(currentTicks - prevTicks) / 1000.0f;
}

unsigned int Timer::GetSleepTime(const unsigned int fps_) const {
	unsigned int milliSecsPerFrame = 1000 / fps_;
	if (milliSecsPerFrame == 0) {
		return 0;
	}
	unsigned int sleepTime = milliSecsPerFrame - SDL_GetTicks();
	if (sleepTime > milliSecsPerFrame) {
		return milliSecsPerFrame;
	}
	return sleepTime;
}

float Timer::GetCurrentTicks() const {
	return static_cast<float>(currentTicks) / 1000.0f;
}