#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "PrototypeMenuNavigation.h"
#include <iostream>



GameManager::GameManager()
{
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
}

bool GameManager::OnCreate() {
	const int SCREEN_WIDTH = 1366;
	const int SCREEN_HEIGHT = 728;
	ptr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (ptr == nullptr) {
		OnDestroy();
		return false;
	}

	if (ptr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

	currentScene = new PrototypeMenuNavigation(ptr->GetSDL_Window());
	if (currentScene == nullptr) {
		OnDestroy();
		return false;
	}

	if (currentScene->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	return true;
}


GameManager::~GameManager()
{
}

void GameManager::Run() {
	timer->Start();
	while (isRunning) {
		timer->UpdateFramTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		
		while (SDL_PollEvent(&e) != 0) { //Event Loop
			if (e.type == SDL_QUIT) {
				isRunning = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				InputManager();
			}
		}
		//Event loop running at a proper rate
		//Currently set to 60 frames per second
		SDL_Delay(timer->GetSleepTime(60));
	}
}
//Is called when a key is down
//Only works when when currentInput in WhateverScene.cpp has been instantiated inside the OnCreate()
//When a key is press switch case changes the value of the enum InputEvent to the current key pressed
//Use the currentInput value when handling input reception inside the WhateverScene.cpp
void GameManager::InputManager() {

	switch (e.key.keysym.sym) {
		case SDLK_1:
			*currentScene->currentInput = KEY_PRESS_1;
			break;

		case SDLK_2:
			*currentScene->currentInput = KEY_PRESS_2;
			break;

		case SDLK_3:
			*currentScene->currentInput = KEY_PRESS_3;
			break;

		case SDLK_4:
			*currentScene->currentInput = KEY_PRESS_4;
			break;

		case SDLK_5:
			*currentScene->currentInput = KEY_PRESS_5;
			break;

		case SDLK_6:
			*currentScene->currentInput = KEY_PRESS_6;
			break;

		case SDLK_7:
			*currentScene->currentInput = KEY_PRESS_7;
			break;

		case SDLK_8:
			*currentScene->currentInput = KEY_PRESS_8;
			break;

		case SDLK_9:
			*currentScene->currentInput = KEY_PRESS_9;
			break;

		case SDLK_0:
			*currentScene->currentInput = KEY_PRESS_0;
			break;

	}
}

void GameManager::OnDestroy() {
	if (ptr) delete ptr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}
