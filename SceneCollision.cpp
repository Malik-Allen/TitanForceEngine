#include "SceneCollision.h"



SceneCollision::SceneCollision():ray(nullptr), sphere(nullptr) {}


SceneCollision::~SceneCollision()
{
}

bool SceneCollision::OnCreate()
{
	ray = new Ray(startPos, direc);
	sphere = new Sphere(0.0f, 0.0f, 0.0f, 5.0f);

	if (Collider::RaySphereCollisionDetected(*ray, *sphere))
		printf("xPos: %f\nyPos: %f\nzPos: %f\n",Collider::RaySphereCollisionPoint(*ray, *sphere).x, Collider::RaySphereCollisionPoint(*ray, *sphere).y, Collider::RaySphereCollisionPoint(*ray, *sphere).z);
	return false;
}

void SceneCollision::OnDestroy()
{
}

void SceneCollision::Update(const float deltaTime_)
{
}

void SceneCollision::Render() const
{
}

void SceneCollision::HandleEvents(const SDL_Event & sdlEvent)
{
}
