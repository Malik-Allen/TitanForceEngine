#ifndef PROFILER_H
#define PROFILER_H

#include "..\Core\Timer.h"
#include <vector>

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
	
private:

	Timer* timer;

};


#endif