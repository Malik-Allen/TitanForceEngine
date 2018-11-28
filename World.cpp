#include "World.h"



World::World(Vec3& gravitionalForce)
{
	worldGravity = gravitionalForce;

	worldObjects = new BodyList();
}


World::~World()
{
}
