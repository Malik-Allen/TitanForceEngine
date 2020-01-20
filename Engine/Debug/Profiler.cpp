#include "Profiler.h"
#include <string>
#include <iostream>

Profiler::Profiler() :timer(nullptr) {}

Profiler::~Profiler(){}

bool Profiler::OnCreate() {
	timer = new Timer();
	if (timer == nullptr) {
		throw std::runtime_error("Failed to create profiler timer!");
		return false;
	}
	//startTime = timer->GetCurrentTimeInMilliSeconds();
	//endTime = timer->GetCurrentTimeInMilliSeconds();
	return true;
}



void Profiler::BeginProfile() {
	std::string name;
	name = __func__;
	std::cout << name << std::endl;
}

void Profiler::EndProfile() {
	
}


