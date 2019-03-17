#ifndef COLLIDER_H
#define COLLIDER_H

#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "RigidBody.h"
#include "AAB.h"
#include "Algebra.h"

using namespace MATH;

class Collider
{
public:
	Collider();
	~Collider();

	static bool RaySphereCollisionDetected(const Ray &ray, const Sphere &sphere);
	static Vec3 RaySphereCollisionPoint(const Ray &ray, const Sphere &sphere);


	static bool RayAABCollisionDetected(const Ray &ray, const AAB &box);
	static Vec3 RayAABCollisionPoint(const Ray &ray, const Sphere &sphere);


	static bool SphereSphereCollisionDetected(const RigidBody &body1, const RigidBody &body2);
	static void SphereSphereCollisionResponse(RigidBody &body1, RigidBody &body2);
	static void SphereStaticSphereCollisionResponse(RigidBody &body1, const RigidBody& staticSphere);


	static bool SpherePlaneCollisionDetected(const RigidBody& body, const Plane &plane);
	static void SpherePlaneCollisionRespone(RigidBody& body, const Plane &plane);

};


#endif

