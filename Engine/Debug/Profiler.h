#ifndef PROFILER_H
#define PROFILER_H

#include "..\Core\Timer.h"
#include <vector>

//#define PROFILER_ENABLED 1

//#if PROFILER_ENABLED == 1
//#define BEGIN_PROFILE( a ) Profiler::BeginProfile()
//#define END_PROFILE( a ) Profiler::EndProfile()

//#else

//#define BEGIN_PROFILE( a )
//#define END_PROFILE( a )

//#endif


struct ProfileFrame {
	// The total time this profile has taken this frame
	float totalFrameTime;

	// The total number of times this profile was called this frame
	unsigned int calls;

	// The hierrachical level of this profile, 0 being the main loop
	unsigned int hierarchicalLvl;
};

struct ProfileHistory {

};

class Profiler {

public:
	// No need for the profiler at the moment to be copied or moved
	Profiler(const Profiler&) = delete;
	Profiler& operator=(const Profiler&) = delete;
	Profiler(Profiler&&) = delete;
	Profiler& operator=(Profiler&&) = delete;

	Profiler();
	~Profiler();

	bool OnCreate();
	
	Timer* timer;

	static void BeginProfile();
	static void EndProfile();

	static float startTime;
	static float endTime;

};


#endif