#include "SolarSimulation.h"

SolarSimulation::SolarSimulation(SDL_Window* sdlWindow)
{
	window2 = sdlWindow;
}


SolarSimulation::~SolarSimulation()
{
}

bool SolarSimulation::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window2, &w, &h);
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(0.0f, 14.0f, 0.0f, 7.0f, 0.0f, 1.0f);

	return true;
}

void SolarSimulation::OnDestroy(){}

void SolarSimulation::Update(const float time) {

}

void SolarSimulation::Render() {
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window2);
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

	SDL_UpdateWindowSurface(window2);
}
