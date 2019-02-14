#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL.h>
#include "Scene.h"
#include "Debug.h"

/// GameManager Capabilities
/// Upon instantiation, creates a Window, and a world Timer
/// During run time manages the Scenes and Event Queing and Rendering
/// -- Feb 13, 2019

// Future GameManager Iterations Include:
// Loading and unloading Scenes dynamically
// Mouse Inputs -- Feb 13, 2019

class GameManager {
private:
	class Window *window;
	class Timer *timer;
	bool isRunning;
	Scene *currentScene;

	SDL_Event sdlEvent;

public:
	
	enum SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE2,
		SCENE3,
		SCENE4
	};

	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();

	void Run();

	Scene *BuildScene(SCENE_NUMBER newScene);
};


#endif

