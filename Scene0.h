#ifndef SCENE0_H
#define SCENE0_H

#include "MMath.h"
#include "Scene.h"
#include "Body.h"
#include "BodyList.h"
#include "World.h"
#include <SDL.h>

//#ifndef GRAVITY
//#define GRAVITY Vec3(0.0f, -9.81f, 0.0f)
//#endif

using namespace MATH;
class Scene0 : public Scene {
private:
	SDL_Window *window;
	Matrix4 projectionMatrix;
	//SDL_Surface *jetSkiImage;
	Vec3 worldGravity = (0.0f, 0.0f, 0.0f);
	Vec3 m_force, m_force2;
	BodyList *worldObjects = new BodyList;
	BodyDefinition *planet = new BodyDefinition;

	World *earth = new World(worldGravity);

	Body *m_jetski = new Body(planet);
	Body *m_boat = new Body(planet);
	Body *m_tree = new Body(planet);
	float timePassed;
	bool rotated = false;
	bool rotated2 = false;
	//SDL_Rect jetSkiRect;
public:
	Scene0(SDL_Window* sdlWindow);
	~Scene0();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
};

#endif


