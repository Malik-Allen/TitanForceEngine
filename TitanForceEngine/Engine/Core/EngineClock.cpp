#include "EngineClock.h"

EngineClock::EngineClock():
	HighResTimer(),
	m_prevTicks(0),
	m_currentTicks(0),
	m_fps(120)
{
	EngineClock::Reset();
}

EngineClock::~EngineClock() {}

void EngineClock::SetFPS(unsigned int fps_) { m_fps = fps_;}

void EngineClock::Reset() {
	m_prevTicks = m_currentTicks = HighResTimer::GetCurrentTimeInMilliSeconds();
}

void EngineClock::UpdateFrameTicks() {
	m_prevTicks = m_currentTicks;
	m_currentTicks = HighResTimer::GetCurrentTimeInMilliSeconds();
}

float EngineClock::GetDeltaTime() const {
	return static_cast<float>(m_currentTicks - m_prevTicks) * MILLISECONDS_TO_SECONDS;	// Conversion to seconds
}

float EngineClock::GetCurrentTicks() const {
	return static_cast<float>(m_currentTicks) * MILLISECONDS_TO_SECONDS;
}

unsigned int EngineClock::GetSleepTime(const unsigned int fps_) {
	unsigned int milliSecsPerFrame = SECONDS_TO_MILLISECONDS / fps_;

	if (milliSecsPerFrame == 0)
		return 0;

	unsigned int sleepTime = milliSecsPerFrame - HighResTimer::GetCurrentTimeInMilliSeconds();

	if (sleepTime > milliSecsPerFrame)
		return milliSecsPerFrame;

	return sleepTime;
}