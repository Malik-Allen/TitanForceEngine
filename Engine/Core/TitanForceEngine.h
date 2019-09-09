#ifndef TITANFORCEENGINE_H
#define TITANFORCEENGINE_H

#include "Window.h"
#include "Timer.h"

class TitanForceEngine {

public:

	// The TitanForceEngine class should not be copied or moved hence removing the functionality
	TitanForceEngine(const TitanForceEngine&) = delete;
	TitanForceEngine& operator=(const TitanForceEngine&) = delete;
	TitanForceEngine(TitanForceEngine&&) = delete;
	TitanForceEngine& operator = (TitanForceEngine&&) = delete;

	TitanForceEngine();
	~TitanForceEngine();
	
	bool InitEngine(std::string name, const int initWindowWidth, const int initWindowHeight);
	void Run();
	bool IsRunning() const;
	bool IsGameRunning() const;
	void Stop();
	void StopGame();

	static TitanForceEngine* GetInstance();

private:
	
	static std::unique_ptr<TitanForceEngine> engineInstance;
	friend std::default_delete<TitanForceEngine>;
	
	
	void OnDestroy();

	void Update(const float deltaTime);	// NOTE: Should this be overridable? If, so then why? What are the beneifits?
	void Render();						// NOTE: Consider the ECS model, this may become a job system

	void HandleEvents();

	Window* window;
	Timer* timer;

	bool isRunning;
	bool isGameRunning;

	unsigned int fps;
};

#endif