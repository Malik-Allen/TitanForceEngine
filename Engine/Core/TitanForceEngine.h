#ifndef TITANFORCEENGINE_H
#define TITANFORCEENGINE_H


#include "EngineTimer.h"
#include "../Vulkan/Devices/Window.h"	// GLFW does re-defines a macro called APREIENTY so keep below engine timer--> b/c it has include windows.h and that redefines APREIENTY


#include "../Debug/Profiler.h"
#include "..//Game/GameInterface.h"
#include "..//Game/SceneInterface.h"

// To be used to control whether or not the game will run on start of engine or later in the process
#define RUN_GAME_ON_START 1	

// Singleton Engine Class
class TitanForceEngine {

public:

	// The TitanForceEngine class should not be copied or moved hence removing the functionality
	TitanForceEngine(const TitanForceEngine&) = delete;
	TitanForceEngine& operator=(const TitanForceEngine&) = delete;
	TitanForceEngine(TitanForceEngine&&) = delete;
	TitanForceEngine& operator = (TitanForceEngine&&) = delete;	
	
	bool InitEngine(const std::string& name, const int initWindowWidth, const int initWindowHeight);
	void Run();
	bool IsRunning() const;
	bool IsGameRunning() const;
	void Exit();
	void ExitGame();

	// Gets the instance of the titan force engine
	static TitanForceEngine* GetInstance();

	void SetFPS(const unsigned int fps_);
	void SetGameInterface(GameInterface*);

	int GetCurrentSceneNum() const;
	void SetCurrentSceneNum(int sceneNum);

private:

	TitanForceEngine();
	~TitanForceEngine();
	
	static std::unique_ptr<TitanForceEngine> engineInstance;
	friend std::default_delete<TitanForceEngine>;

	void OnDestroy();

	EngineTimer *engineTimer;
	Profiler *engineProfiler;

	Vulkan::Window* window;

	GameInterface *gameInterface;

	bool isRunning;
	bool isGameRunning;

	unsigned int fps;

	int currentSceneNum;

	void Update(const float deltaTime);	
	void Render();
	void HandleEvents();

	void SetUpInput();


};

#endif