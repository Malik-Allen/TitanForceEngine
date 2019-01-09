#ifndef TIMER_H
#define TIMER_H

class Timer {
private:
	unsigned int prevTicks;
	unsigned int currTicks;

public:
	Timer();
	~Timer();

	void Start();
	void UpdateFramTicks();
	float GetDeltaTime() const;
	float GetWorldTime();
	unsigned int GetSleepTime(const unsigned int fps) const;

};


#endif

