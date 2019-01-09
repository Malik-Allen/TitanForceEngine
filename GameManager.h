#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL.h>
#include "Scene.h"

class GameManager {
private:
	class Window *ptr;
	class Timer *timer;
	bool isRunning;
	Scene *currentScene;

	SDL_Event e;

public:
	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();

	void Run();

	void InputManager();
};


#endif

