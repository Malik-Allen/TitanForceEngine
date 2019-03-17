#include "Collider.h"

/***Current Work Around because there is no object type of const MMATH:: Vec3*/

Collider::Collider()
{
}

Collider::~Collider()
{
}

// Check to see if there a collision between a ray and a sphere
bool Collider::RaySphereCollisionDetected(const Ray &ray, const Sphere &sphere) {

	// Using the quadratic equation to determine whether this ray and sphere intersect
	// In the quadatric eqxn a = Ray's Direction Squared
	// b = 2 x Ray's Starting Position x Ray's Direction
	// Finally c = Ray's Starting Position Squared - The Sphere's Radius Squared

	Vec3 rayStartPos = Vec3(ray.startPosition);							// Because the const Vec3 does not contain the member's functionality necessary, I am using a copy constructor here to access the functionality
	Vec3 rayDirection = Vec3(ray.direction);							// **Current work around**

	float a = rayDirection.DotProduct(rayDirection);
	float b = 2.0f * (rayStartPos.DotProduct(rayDirection));
	float c = rayStartPos.DotProduct(rayStartPos) - (sphere.r * sphere.r);

	float d = Algebra::CalculateDiscriminant(a, b, c);					// Using the discriminant to determine whether or not this ray intersects with the given sphere

	if (d >= 0.0f)														// If the discriminant is less than zero then, there are no intersection points between the ray and sphere
		return true;
	else																// If the discriminant is equal to zero then, the ray is tangent to the sphere
		return false;

}

// This function will return the closest collison point if there is one detected
Vec3 Collider::RaySphereCollisionPoint(const Ray &ray, const Sphere &sphere) {
	//	Repition of code here currently looks unavoidable

	Vec3 rayStartPos = Vec3(ray.startPosition);							// Because the const Vec3 does not contain the member's functionality necessary, I am using a copy constructor here to access the functionality
	Vec3 rayDirection = Vec3(ray.direction);							// **Current work around**
	Ray r = Ray(ray);							

	float a = rayDirection.DotProduct(rayDirection);
	float b = 2.0f * rayStartPos.DotProduct(rayDirection);
	float c = rayStartPos.DotProduct(rayStartPos) - (sphere.r * sphere.r);

	float t1 = Algebra::CalculateQuadratic(a, b, c, true);
	float t2 = Algebra::CalculateQuadratic(a, b, c, false);

	if (Algebra::CalculateDiscriminant(a, b, c) == 0.0f) {				// Because this is only called after a Collision Detection the Discriminant is either zero or greater
		return r.CurrentPosition(t1);									// When the discriminant is zero there is one solution
	}
	else {																// If the position of the smaller t value is smaller than the startPoint of the ray,
		if (abs(t1) < abs(t2)) {										// Checking if the Ray is in the direction of the sphere
			
			if (t1 < 0.0f && t2 < 0.0f) {								// If both values of t are negative than the ray is away from the sphere
				Debug::Warning("Ray is in opposite direction of sphere, and does not collide", __FILE__, __LINE__);
				//return Vec3(0.0f, 0.0f, 0.0f);
			}
			else if (t1 > 0.0f && t2 > 0.0f) {							// If both values of t are greater than zero than, the ray has to collisions points and start outisde of the sphere
				if (t1 > t2)
					return r.CurrentPosition(t2);
				else
					return r.CurrentPosition(t1);
			}
			else {														// If either of the values of t are less than zero then the starting point of the sphere is inside of the ray
				if (t1 > t2)
					return r.CurrentPosition(t2);
				else
					return r.CurrentPosition(t1);
			}
			
		}
		else {
			Debug::Warning("Ray does not intersect with the sphere!", __FILE__, __LINE__);
			//return Vec3(0.0f, 0.0f, 0.0f);
		}
	}

}
























bool Collider::RayAABCollisionDetected(const Ray & ray, const AAB & box)
{
	Ray ray_ = Ray(ray);
	AAB box_ = AAB(box);

	if (ray.direction.x == 0.0f) {

	}
	return false;
}

Vec3 Collider::RayAABCollisionPoint(const Ray & ray, const Sphere & sphere)
{
	return Vec3();
}

// Check to see if a collision is detected between two Bodies
bool Collider::SphereSphereCollisionDetected(const RigidBody & body1, const RigidBody & body2)
{
	RigidBody rb1 = RigidBody(body1);
	RigidBody rb2 = RigidBody(body2);

	if (rb1.m_position.Distance(rb2.m_position) <= (rb1.m_radius + rb2.m_radius)){		// If the distance between the center of the two bodies are less than or equal to the two radii
		return true;																		// Collision have been detected
	}
	return false;
}


// Responds to collisions between two bodies
void Collider::SphereSphereCollisionResponse(RigidBody &body1, RigidBody &body2)
{
	if (SphereSphereCollisionDetected(body1, body2)) {				// Checking to see if the two bodies have collided
																	// If they have, now we need to check if any of the body's are static
		if (body1.m_type == staticBody) {							// If body1 is static,
			SphereStaticSphereCollisionResponse(body2, body1);			// Call the SphereStaticSphere Response with Body1 as the static Body
		}
		else if (body2.m_type == staticBody) {						// If body2 is static,
			SphereStaticSphereCollisionResponse(body1, body2);			// Call the SphereStaticSphere Response with Body2 as the static Body
		}
		else {														// Else if non of the bodies are static, respond
			// Some sort of response to the sphere's collision
		}

	}
}

void Collider::SphereStaticSphereCollisionResponse(RigidBody &body1, const RigidBody &staticSphere){}

// Checks if the sphere and plane have collided
bool Collider::SpherePlaneCollisionDetected(const RigidBody &body, const Plane &plane)
{
	Plane plane_ = Plane(plane);
	if (plane_.Distance(body) <= 0.0f) {	// Check if the distance between the sphere and plane is less than or equal to zero
		return true;							// Returns true if it is
	}
	return false;
}

void Collider::SpherePlaneCollisionRespone(RigidBody &body, const Plane &plane)
{
	if (SpherePlaneCollisionDetected(body, plane)) {			// If a collision has been detected
		
		Plane plane_ = Plane(plane);
		Vec3 initVel = -1.0f * body.m_velocity;
		plane_.Normalize();

		Vec3 projection = initVel.ProjectedOn(plane_);			// Projecting the (neg) initial velocity onto the normalized normal vector of the plane

		Vec3 finalVel = body.m_velocity + (2.0f * projection);	// Finding the final velocity using the equation FinalVelocity = InitialVelocity + 2 x ProjectedVector

		// Changing the body's velocity
		// Using Momentum etc
		
	}
}
