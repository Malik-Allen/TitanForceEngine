#ifndef SCENECOLLISION_H
#define SCENECOLLISION_H

#include "Scene.h"
#include "Ray.h"
#include "Sphere.h"
#include "Collider.h"

using namespace MATH;

class SceneCollision : public Scene
{
public:
	Vec3 startPos = Vec3(2.0f, -1.0f, -1.0f);
	Vec3 direc = Vec3(0.0f, 2.0f, 3.0f);
	Ray *ray;
	Sphere *sphere;

	explicit SceneCollision();
	virtual ~SceneCollision();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};

#endif

