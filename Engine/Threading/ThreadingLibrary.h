#ifndef THREADINGLIBRARY_H
#define THREADINGLIBRARY_H
#include <Windows.h>
#include <atomic>

class ThreadingLibrary {

public:

	ThreadingLibrary(const ThreadingLibrary&) = delete;
	ThreadingLibrary& operator=(const  ThreadingLibrary&) = delete;
	ThreadingLibrary(ThreadingLibrary&&) = delete;
	ThreadingLibrary& operator=(ThreadingLibrary&&) = delete;

	ThreadingLibrary() = delete;

	static std::atomic<int> example;

	static void Add(int sum);

private:

	CRITICAL_SECTION criticalSection;


	

};

#endif