#ifndef TITANFORCEENGINE_H
#define TITANFORCEENGINE_H


#include "EngineTimer.h"
#include "Window.h"	// GLFW does re-defines a macro called APREIENTY so keep below engine timer--> b/c it has include windows.h and that redefines APREIENTY
#include "..\Debug\Debug.h"
#include "..\Debug\Profiler.h"

class TitanForceEngine {

public:

	// The TitanForceEngine class should not be copied or moved hence removing the functionality
	TitanForceEngine(const TitanForceEngine&) = delete;
	TitanForceEngine& operator=(const TitanForceEngine&) = delete;
	TitanForceEngine(TitanForceEngine&&) = delete;
	TitanForceEngine& operator = (TitanForceEngine&&) = delete;	
	
	bool InitEngine(std::string name, const int initWindowWidth, const int initWindowHeight);
	void Run();
	bool IsRunning() const;
	bool IsGameRunning() const;
	void Stop();
	void StopGame();

	// Gets the instance of the titan force engine
	static TitanForceEngine* GetInstance();

	void SetFPS(const unsigned int fps_);

private:

	TitanForceEngine();
	~TitanForceEngine();
	
	// Using the singleton design pattern, I am preventing multiple instance of the engine running within the same program.
	// I only want one instanc of my engine around
	static std::unique_ptr<TitanForceEngine> engineInstance;
	friend std::default_delete<TitanForceEngine>;
	
	void OnDestroy();

	void Update(const float deltaTime);	// NOTE: Should this be overridable? If, so then why? What are the beneifits?
	void Render();						// NOTE: Consider the ECS model, this may become a job system

	void HandleEvents();

	Window* window;
	EngineTimer* engineTimer;
	Profiler* engineProfiler;

	bool isRunning;
	bool isGameRunning;

	unsigned int fps;

	
};

#endif