#include "Profiler.h"

Profiler::Profiler():timer(nullptr){}

Profiler::~Profiler(){}

bool Profiler::OnCreate() {
	timer = new Timer();
	if (timer == nullptr) {
		throw std::runtime_error("Failed to create profiler timer!");
		return false;
	}

	return true;
}


