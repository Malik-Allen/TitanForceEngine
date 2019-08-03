#include "Shapes.h"

using namespace SHAPES;

///// ***** PLANE ***** \\\\\

/// A constructor for Plane that uses 3 Vector3 to define the Plane
Plane::Plane(const MATH::Vector3& v0, const MATH::Vector3& v1, const MATH::Vector3& v2) {
	MATH::Vector3 temp1 = v1 - v0;	// We can create a plane using two Vectors
	MATH::Vector3 temp2 = v2 - v1;	// We create these two intersecting Vectors by taking the differences between 3 vectors

	MATH::Vector3 normal(temp1.y * temp2.z - temp1.z * temp2.y,
		-(temp1.x * temp2.z - temp1.z * temp2.x),
		temp1.x * temp2.y - temp1.y * temp2.x);	// We cross product of a two Vectors in order to find the normal of plane

	float a = sqrt(pow(normal.x, 2.0f) + pow(normal.y, 2.0f) + pow(normal.z, 2.0f));
#ifdef _DEBUG
	if (fabs(a) < VERY_SMALL) {
		std::string errorMsg("Error! Dividing nearly by zero!");
		throw errorMsg;
	}
#endif

	// We use the normal to define the plane
	x = normal.x / a;
	y = normal.y / a;
	z = normal.z / a;
	d = -normal.x * v1.x + -normal.y * v1.y + -normal.z * v1.z;
	// The d-component of the plane is the dot product of (-)normal and a point on a plane,
	// In this case I use the given point v1
}

/// Converts this plane into a normalized plane
void Plane::normalize() {
	float a = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
	x /= a;
	y /= a;
	z /= a;
	d /= a;
}

/// Returns the direction of the plane to its opposite sideS
Plane Plane::flipped() {
	return Plane(-x, -y, -z, d);
}

/// Calculates the intersection between the Plane and the Ray
MATH::Vector3 Plane::IntersecPoint(const Ray& r_) {

	float a = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));	// Magnitude of normal of the Plane
#ifdef _DEBUG
	if (fabs(a) < VERY_SMALL) {
		std::string errorMsg("Error! Dividing nearly by zero!");
		throw errorMsg;
	}
#endif

	MATH::Vector3 normal(x / a, y / a, z / a);	// Normalizing this Plane's normal

	Ray r(r_);
	float check1 = (normal.x * r.direction.x + normal.y * r.direction.y + normal.z * r.direction.z);	// Normal Dot Ray Direction
	float check2 = (normal.x * r.origin.x + normal.y * r.origin.y + normal.z * r.origin.z) + d;			// Normal Dot Ray Origin + D

#ifdef _DEBUG
	if (check1 == 0.0f) {	// When Check1 is zero, the line is parallel to the plane
							// However, if Check2 is also zero, then the Ray lies inside of the Plane
		if (check2 == 0.0f) {
			std::string errorMsg("Error! Ray lies inisde the Plane and intersects infinitely!");
			throw errorMsg;
		}
		std::string errorMsg("Error! Ray is parallel to Plane and does not intersect!");
		throw errorMsg;
	}
#endif

	float t = -check2 / check1;
	return r.CurrentPosition(t);

}

/// Calculates the projections of one Vector3 onto a Plane's normal
MATH::Vector3 SMath::ProjectOnPlane(const MATH::Vector3& v, const Plane& p) {
	Plane plane(p);
	plane.normalize();

	float dot = v.x * plane.x + v.y * plane.y + v.z * plane.z;
	float mag = sqrt(pow(plane.x, 2.0f) + pow(plane.y, 2.0f) + pow(plane.z, 2.0f));
	float scalar = dot / (mag * mag);
	return plane * scalar;
}

/// Calculates the distance between a given point (Vector3) and Plane
float SMath::Distance(const MATH::Vector3& v, const Plane& p) {
	return v.x * p.x + v.y * p.y + v.z * p.z - p.d;
}

/// Translates Plane by given Vector3
Plane SMath::Translate(const Plane& p, const MATH::Vector3& v) {
	return Plane(p.x - v.x, p.y - v.y, p.z - v.z, p.d);
}
