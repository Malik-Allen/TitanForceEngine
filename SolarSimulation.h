#ifndef SOLARSIMULATION_H
#define SOLARSIMULATION_H

#include "Scene.h"
#include "MMath.h"
#include <SDL.h>

using namespace MATH;

class SolarSimulation: public Scene{
private:
	SDL_Window *window2;
	Matrix4 projectionMatrix;

public:
	SolarSimulation(SDL_Window* sdlWindow);
	~SolarSimulation();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
};

#endif
