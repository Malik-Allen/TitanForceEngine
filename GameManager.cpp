// Must include all used Scenes' HeaderFiles here
#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "SandBox.h"
#include <iostream>


GameManager::GameManager(): window(nullptr), timer(nullptr), currentScene(nullptr)
{
	isRunning = true;
}

bool GameManager::OnCreate() {

	Debug::DebugInit("TitanForceEngine_Log");
	Debug::SetSeverity(MessageType::TYPE_NONE);

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (window == nullptr) {
		OnDestroy();
		return false;
	}

	if (window->OnCreate() == false) {
		OnDestroy();
		Debug::FatalError("Failed to initialize GameManager", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		Debug::FatalError("Failed to initialize Timer", __FILE__, __LINE__);
		return false;
	}

	currentScene = BuildScene(SCENE0);
	if (currentScene == nullptr) {
		OnDestroy();
		Debug::FatalError("Failed to define currentScene", __FILE__, __LINE__);
		return false;
	}

	if (currentScene->OnCreate() == false) {
		OnDestroy();
		Debug::FatalError("Failed to initialize currentScene", __FILE__, __LINE__);
		return false;
	}

	return true;
}


GameManager::~GameManager()
{
	OnDestroy();
}

void GameManager::Run() {
	timer->Start();
	while (isRunning) {

		timer->UpdateFramTicks();

		/// Rendering current scene to the window
		SDL_RenderClear(window->renderer);
		currentScene->Render();
		currentScene->Update(timer->GetDeltaTime());
		SDL_RenderPresent(window->renderer);
		
		/// Event queue managing the different types of inputs, room for more types of Events
		while (SDL_PollEvent(&sdlEvent) != 0) {
			switch (sdlEvent.type) {
			case SDL_EventType::SDL_QUIT:
				isRunning = false;
				return;

			case SDL_EventType::SDL_KEYDOWN:
			case SDL_EventType::SDL_KEYUP:

			default:
				currentScene->InputManagement(sdlEvent);
				break;
			}
		}

		//Event loop running at a proper rate
		//Currently set to 60 frames per second
		SDL_Delay(timer->GetSleepTime(60));
	}
}

void GameManager::OnDestroy() {
	/// Deleting member variables from memory, setting variables to nullptr
	currentScene->OnDestroy(); // In order to destroy everything in the Scene
	delete currentScene;
	currentScene = nullptr;

	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}

	Debug::Info("Deleting GameManager", __FILE__, __LINE__);
}

// Returns the Scene that is asscociated with the argued SCENE_NUMBER
Scene* GameManager::BuildScene(SCENE_NUMBER scene) {

	Scene* newScene = nullptr;

	switch (scene) {
		case SCENE0:
			newScene = new SandBox(window->GetSDL_Window(), window->renderer);
			break;

		default:
			Debug::Error("Incorrect scene number assigned in GameManager", __FILE__, __LINE__);
			newScene = nullptr;
			break;
	}

	return newScene;
}
