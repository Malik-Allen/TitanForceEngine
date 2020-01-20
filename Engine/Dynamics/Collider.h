#ifndef COLLIDER_H
#define COLLIDER_H

#include "RigidBody.h"
#include "..//Math/Shapes.h"

// Class used to represent an object that can be collided with, object must have a RigidBody
class Collider
{
public:
	Collider();
	~Collider();

	Collider(const Collider&) = delete;
	Collider &operator=(const Collider&) = delete;
	Collider(const Collider&&) = delete;
	Collider &operator=(const Collider&&) = delete;


	// Returns true if the Ray and Sphere Collide
	static bool RaySphereCollisionDetected(const SHAPES::Ray& ray, const SHAPES::Sphere& sphere);
	// Returns the point at which a Ray and Sphere Collision takes place
	static Vector3 GetCollisionPoint(const SHAPES::Ray& ray, const SHAPES::Sphere& sphere);
	
	// Returns true when the Ray and Box Intersect
	static bool RayAABoxCollisionDetected(const SHAPES::Ray& ray, const SHAPES::Box& box);
	// Returns the point at which the intersection takes place between a Ray and Box
	static Vector3 GetCollisionPoint(const SHAPES::Ray& ray, const SHAPES::Box& box);

	// Returns true when a collisions is detected between a rigidbody and plane
	static bool SpherePlaneCollisionDetected(const RigidBody& body, const SHAPES::Plane& plane);
	// Responds appropiately to the collision between a rigidbody and a plane
	static void CollisionResponse(RigidBody& body, const SHAPES::Plane& plane);

	// Returns true when a collision is detected between two non-kinematic rigidbodies occurs
	static bool RigidBodyCollisionDetected(const RigidBody& body1, const RigidBody& body2);
	// Responds appropiately to the collision between two non-kinematic rigidbodies
	static void CollisionResponse(RigidBody& body1, RigidBody& body2);


};

class CMath {
public:

	// Calculates the discriminant using the quadratic's coefficients
	inline static float Discriminant(float a, float b, float c) {
		return (b * b) - (4.0f * a * c);
	}

	// Calculates the solution for a quadratic equation, using the coefficients of the quadratic (a is t^2, b is t and c is independent of t)
	static float Quadratic(float a, float b, float c, bool returnSmallest);

};

#endif

// Note #1:
/// Witin the Ray Sphere Collision Detection, when the Ray starts inside of the ray, it will return the exiting collision point, 
/// The entering point would be in the opposite direction of the ray

// Note #2:
/// On Collision Exit!!!!

// Note #3:
/// When detecting a collision between a ray and a box, if the ray starts in the box, no collision will be detected
/// Furthermore, if the ray starts on the plane it will return the starting point as the collision point

// Note #4:
/// Best Practice, to limit the size of the ray in ray collision detected, when you find the collision point parse the function's magnitude of the resultant vector
/// This will provide the flexibility you will need along with the speed of ray calling ray instead