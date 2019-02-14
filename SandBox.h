#ifndef SANDBOX_H
#define SANDBOX_H
#include "Scene.h"
#include <SDL.h>
#include "GameObjList.h" 

using namespace MATH;

class SandBox : public Scene
{
private:
	SDL_Window *window;
	Matrix4 projectionMatrix;
	SDL_Renderer *renderer;

	float timePassed;
	float torque;

	GameObjList *worldObjects;
	BodyDefinition* actor;

	GameObject *dummy;
	GameObject *van;

	GameObject *starship;

	Vec3 vector1, vector2;

public:
	SandBox(SDL_Window* sdlWindow, SDL_Renderer *sdlRenderer);
	~SandBox();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float time);
	virtual void Render();
	virtual void InputManagement(const SDL_Event &sdlEvent);
};

#endif
