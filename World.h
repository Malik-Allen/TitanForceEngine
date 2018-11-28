#ifndef WORLD_H
#define WORLD_H

#include "BodyList.h"

using namespace MATH;

#ifdef GRAVITY
#undef GRAVITY 
#define GRAVITY worldGravity
#endif
class World
{
public:
	Vec3 worldGravity; 

	World(Vec3& gravitationalForce);
	~World();

	BodyList *worldObjects;


};

#endif

