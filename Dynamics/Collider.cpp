#include "Collider.h"

Collider::Collider(){}

Collider::~Collider(){}

/// Calculates the solution for a quadratic equation, using the coefficients of the quadratic (a is t^2, b is t and c is independent of t)
float CMath::Quadratic(float a, float b, float c, bool returnSmallest) {
	float d = Discriminant(a, b, c);

#ifdef _DEBUG	// May not always want to throw an error so this is uo for debate
	if (d < 0.0f) {		// When the discriminant is less than zero then there is no solution
		std::string errorMsg("Error! Not able to find the sqrt of a negative number!");
		throw errorMsg;
	}
#endif

	if (d < 0.0f)			// When not in debug mode and the discriminant is less than zero it will return a solution of zero
		return 0.0f;
	else if (d == 0.0f) {	// When the disrcriminant is zero there will be only one solution, so return the solultion
		return ((-b) + sqrt(d)) / (2.0f * a);
	}
	else {					// Otherwise when the discriminant is greater than zero, there will be two or more solutions
		float t1 = ((-b) + sqrt(d)) / (2.0f * a);
		float t2 = ((-b) - sqrt(d)) / (2.0f * a);

		if (returnSmallest == true) {		// Returns the smaller solution when the program asks for it
			if (t1 < t2)
				return t1;
			else
				return t2;
		}
		else if (returnSmallest == false) {	// Returns the larger solution when the program prompts
			if (t1 < t2)
				return t2;
			else
				return t1;
		}
	}

}




/// Returns true when this Ray and Sphere Collide
bool Collider::RaySphereCollisionDetected(const SHAPES::Ray& ray, const SHAPES::Sphere& sphere) {
	// We can determine if this Ray and Sphere Collide using the following eqxn
	// V^2 * t^2 + 2(S * V) + S^2 - r^2 /// This is the quadrtic relation that defines the collision between a ray and a sphere
	
	float a = MATH::VMath::Dot(ray.direction, ray.direction);
	float b = 2.0f * (MATH::VMath::Dot(ray.origin, ray.direction));
	float c = MATH::VMath::Dot(ray.origin, ray.origin) - (sphere.r * sphere.r);

	float d = CMath::Discriminant(a, b, c);		// We use the discriminant of this quadratic equation to determine whether or not a collision occurs

	if (d < 0.0f)								// When the discriminant is less than zero/ a negative number then no collision occurs
		return false;
	else {

		float t1 = CMath::Quadratic(a, b, c, true);		// Using the quadratic function to provide the two possible solutions
		float t2 = CMath::Quadratic(a, b, c, false);

		if (d == 0.0f) {								// When the discriminant is zero, then there is one solution and it doesnt matter which solution we use(Collision point is a tangent)
			if (t1 < 0.0f)								// But if the solution is less than zero than there is no collison
				return false;
			else
				return true;
		}
		else {
			if (t1 < 0.0f && t2 < 0.0f)					// When both solutions are negative then the sphere is the opposite direction of the ray
				return false;
			else
				return true;
		}
	}
									// Otherwise, when the discriminant is zero or greater, then there are 1 or more collision points
}

/// Returns the point at which a Ray and Sphere Collision takes place
Vector3 Collider::GetCollisionPoint(const SHAPES::Ray& ray, const SHAPES::Sphere& sphere) {

	float a = MATH::VMath::Dot(ray.direction, ray.direction);
	float b = 2.0f * (MATH::VMath::Dot(ray.origin, ray.direction));
	float c = MATH::VMath::Dot(ray.origin, ray.origin) - (sphere.r * sphere.r);

	float d = CMath::Discriminant(a, b, c);

	// Up until this point, this is the same code as OnCollisionEnter
	// At this point we know there is a collision, so find both solutions for the quadratic relation

	float t1 = CMath::Quadratic(a, b, c, true);		// Using the quadratic function to provide the two possible solutions
	float t2 = CMath::Quadratic(a, b, c, false);

	SHAPES::Ray result(ray);

	if (d == 0.0f)									// When the discriminant is zero, then there is one solution and it doesnt matter which solution we use(Collision point is a tangent)
			return result.CurrentPosition(t1);
	else {											// Otherwise, the discriminant will be greater than zero, meaning two solutions are possible
		
		if (t1 < 0.0f || t2 < 0.0f) {				// If one the solutions is negative and the other positive than the ray starts in the sphere
													// Therefore when the ray starts inside of the sphere, I want to return the exiting collision point instead of the entering collision point
													// So, we return the position at the larger of the two solutions
			if (t1 > t2)
				return result.CurrentPosition(t1);
			else
				return result.CurrentPosition(t2);
			
		}
		else if(t1 > 0.0f && t2 > 0.0f){			// Otherwise when both solutions are positive then it assumed that the sphere lies in the direction of the ray
													// Now we simply return the entering point which is the smallest of the two solutions
			if (t1 < t2)
				return result.CurrentPosition(t1);
			else
				return result.CurrentPosition(t2);
		}
	}
}

/// Returns true when the Ray and Box Intersect
bool Collider::RayAABoxCollisionDetected(const SHAPES::Ray& ray, const SHAPES::Box& box) {

	if (ray.direction.x == 0.0f && ray.direction.y == 0.0f && ray.direction.z == 0.0f) {	// When all direction components are zero there are no intersections at x or rx, etc, therefore V is parralel in this regard
		return false;
	}
	else {

		float tx, ty, tz;	// Will be the three values of t where we will check for collisions at

		if (ray.direction.x != 0.0f) {		// Collisions only occur when the direction component of the vector is not zero

			if (ray.direction.x < 0.0f) {			// When the direction component is less than zero there is no intersection at plane x, 
													// Since the direction of the ray is incoming from the back of the box,  
													// No need to check the front for collision
				
				tx = (box.rx - ray.origin.x) / ray.direction.x;			// Calculate some t value using the x components of the ray, and r = x 
				
			}
			else if (ray.direction.x > 0.0f) {		// When the direction component is greater than zero there is no intersection at rx
													// Since the direction of the ray is incoming from the front of the box,  
													// No need to check the back for collision

				tx = (0.0f - ray.origin.x) / ray.direction.x;			// Calculate some t value using the x components of the ray, and r = rx 
			}

		}
		else				// When the t value is less than zero, then there is no intersection
			tx = -1.0f;		// So we set this t value to a negative number to be vittoed out later in the code

		// Same principles apply to the other axes of the ray's direction dircetion
		
		if (ray.direction.y != 0.0f) {
			if (ray.direction.y < 0.0f)
				ty = (box.ry - ray.origin.y) / ray.direction.y;
			else if (ray.direction.y > 0.0f)
				ty = (0.0f - ray.origin.y) / ray.direction.y;
		}
		else
			ty = -1.0f;

		if (ray.direction.z != 0.0f) {
			if (ray.direction.z < 0.0f)
				tz = (box.rz - ray.origin.z) / ray.direction.z;
			else if (ray.direction.z > 0.0f)
				tz = (0.0f - ray.origin.z) / ray.direction.z;
		}
		else
			tz = -1.0f;
		
			
			
		if (tx < 0.0f && ty < 0.0f && tz < 0.0f)	// When all values of t are less than zero, there are no points of collision
			return false;
		else {

			SHAPES::Ray result(ray);
			MATH::Vector3 collisionPoint;

			if (tx > 0.0f) {														// When t is greater than zero the ray will eventually intersect with the plane
				collisionPoint = result.CurrentPosition(tx);
				if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&		// This criteria makes sure the point we find is the closest and a valid point of insterction
					0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
					0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
					return true;
			}

			if (ty > 0.0f) {
				collisionPoint = result.CurrentPosition(ty);
				if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&
					0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
					0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
					return true;
			}

			if (tz > 0.0f) {
				collisionPoint = result.CurrentPosition(tz);
				if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&
					0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
					0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
					return true;
			}
																					// At this point if we have not found a satisfying point one of two things are happening 
																					// #1 There are no collisions that satisfy this relation 
																					// #2 The ray starts in one of the planes, so we do one final check to see if it does

			if (tx == 0.0f || ty == 0.0f || tz == 0.0f)								// When the value of t is zero that means it lies on the plane
				return true;
			else
				return false;														// Otherwise no collision takes place 
		}
	}

}

/// Returns the point at which the intersection takes place between a Ray and Box
Vector3 Collider::GetCollisionPoint(const SHAPES::Ray& ray, const SHAPES::Box& box) {
	float tx, ty, tz;	// Will be the three values of t where we will check for collisions at

	if (ray.direction.x != 0.0f) {
		if (ray.direction.x < 0.0f)
			tx = (box.rx - ray.origin.x) / ray.direction.x;
		else if (ray.direction.x > 0.0f)
			tx = (0.0f - ray.origin.x) / ray.direction.x;
	}
	else
		tx = -1.0f;

	if (ray.direction.y != 0.0f) {
		if (ray.direction.y < 0.0f)
			ty = (box.ry - ray.origin.y) / ray.direction.y;
		else if (ray.direction.y > 0.0f)
			ty = (0.0f - ray.origin.y) / ray.direction.y;
	}
	else
		ty = -1.0f;

	if (ray.direction.z != 0.0f) {
		if (ray.direction.z < 0.0f)
			tz = (box.rz - ray.origin.z) / ray.direction.z;
		else if (ray.direction.z > 0.0f)
			tz = (0.0f - ray.origin.z) / ray.direction.z;
	}
	else
		tz = -1.0f;

	SHAPES::Ray result(ray);
	MATH::Vector3 collisionPoint;

	if (tx >= 0.0f) {														// When t is greater than zero the ray will eventually intersect with the plane
		collisionPoint = result.CurrentPosition(tx);
		if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&		// This criteria makes sure the point we find is the closest and a valid point of insterction
			0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
			0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
			return collisionPoint;
	}

	if (ty >= 0.0f) {
		collisionPoint = result.CurrentPosition(ty);
		if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&
			0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
			0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
			return collisionPoint;
	}

	if (tz >= 0.0f) {
		collisionPoint = result.CurrentPosition(tz);
		if (0.0f <= collisionPoint.x && collisionPoint.x <= box.rx &&
			0.0f <= collisionPoint.y && collisionPoint.y <= box.ry &&
			0.0f <= collisionPoint.z && collisionPoint.z <= box.rz)
			return collisionPoint;
	}


}

/// Returns true when a collisions is detected between a rigidbody and plane
bool Collider::SpherePlaneCollisionDetected(const RigidBody& body, const SHAPES::Plane& plane) {

	if (body.m_bodyType == dynamicBody) {
		if (MATH::VMath::Distance(body.m_position, plane) - body.m_radius <= 0.0f)
			return true;
		else
			return false;
	}
	else
		return false;
}

/// Responds appropiately to the collision between a rigidbody and a plane
void Collider::CollisionResponse(RigidBody& body, const SHAPES::Plane& plane) {
	// Projecting the (neg) initial velocity onto the normalized normal vector of the plane
	// Finding the final velocity using the equation FinalVelocity = InitialVelocity + 2 x ProjectedVector
	MATH::Vector3 finalVel = body.m_velocity + (2.0f * (SHAPES::SMath::ProjectOnPlane(-body.m_velocity, plane)));
	body.m_velocity = finalVel;
}

/// Returns true when a collision is detected between two non-kinematic rigidbodies occurs
bool Collider::RigidBodyCollisionDetected(const RigidBody& body1, const RigidBody& body2) {

	if (body1.m_bodyType == staticBody && body2.m_bodyType == staticBody)	// When both bodies are static then there is no need to check for collisions
		return false;
	else {
		if (MATH::VMath::Distance(body1.m_position, body2.m_position) <= (body1.m_radius + body2.m_radius))
			return true;
		else
			return false;
	}
}

/// Responds appropiately to the collision between two non-kinematic rigidbodies
void Collider::CollisionResponse(RigidBody& body1, RigidBody& body2) {

	if (body1.m_bodyType != staticBody && body2.m_bodyType != staticBody) {

		float coeffRestitution = 0.0f;		// The coefficient of restitution determines the inelasticity and elacticity of a collision
											// With a coefficient of 0, the collision will be completely inelastic; The two objects collide and stick together
											// With a coefficient of 1, the collision will be completelt elastic

		MATH::Vector3 collisionVelocity = (body1.m_momentum + body2.m_momentum) / (body1.m_mass + body2.m_mass);			// To determine the final velocity of the collisions we apply the conservation of momentum law
		MATH::Vector3 body1Result = (-coeffRestitution * (body1.m_velocity - body2.m_velocity)) + collisionVelocity;		// Then we use of coefficient of resitution to calculate the final velocities of the objects in this collision
		MATH::Vector3 body2Result = -((-coeffRestitution * (body1.m_velocity - body2.m_velocity)) - collisionVelocity);
		body1.m_velocity = body1Result;
		body2.m_velocity = body2Result;

	}
	else {	// Once we determine one of the rigidbodies is static we will reflect the dynamic body's velocity against the staticBody's

		MATH::Vector3 finalVel;
		MATH::Vector3 diffVec;
		if (body1.m_bodyType == staticBody) {
			diffVec = body2.m_position - body1.m_position;
			finalVel = body2.m_velocity + (2.0f * (MATH::VMath::Project(-body2.m_velocity, diffVec)));
			body2.m_velocity = finalVel;
		}
		else if (body2.m_bodyType == staticBody) {
			diffVec = body1.m_position - body2.m_position;
			finalVel = body1.m_velocity + (2.0f * (MATH::VMath::Project(-body1.m_velocity, diffVec)));
			body1.m_velocity = finalVel;
		}
	}

}


