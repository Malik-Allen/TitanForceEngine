#ifndef PROTOTYPEMENUNAVIGATION_H
#define PROTOTYPEMENUNAVIGATION_H

#include "Scene.h"
#include <SDL.h>
#include "MMath.h"
#include "BodyList.h"

using namespace MATH;

class PrototypeMenuNavigation :public Scene
{
private:
	SDL_Window *window1;
	Matrix4 projectionMatrix;

	BodyList* world1 = new BodyList;
	BodyDefinition* actor = new BodyDefinition;

	Body* hit = new Body(actor);

public:

	Vec3 tempVec;

	PrototypeMenuNavigation(SDL_Window* sdlWindow);
	~PrototypeMenuNavigation();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();

	void InputManagement(InputEvent* inputEvent);
};

#endif

