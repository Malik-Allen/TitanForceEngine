#ifndef BODY_H
#define BODY_H

#include <SDL.h>
#include <SDL_image.h>
//#include <string> //Naming things!

#include "Vector.h"

using namespace MATH;

//BodyTypes:
//staticBody:		zero mass, zero velocity, only moved manually
//kinematicBody:	zero mass, non-zero velocity is set by user
//dynamicBody:		positive mass, non-zero velocity determined by forces
enum BodyType {
	staticBody = 0,
	kinematicBody,
	dynamicBody
};

//By default BodyType is set to staticBody
struct BodyDefinition {

	BodyDefinition() {
		type = staticBody;
		position.Load(0.0f, 0.0f, 0.0f);
		velocity.Load(0.0f, 0.0f, 0.0f);
		angle = 0.0f;

		sprite = nullptr;
		spriteName = nullptr;

		objectName = nullptr;
	}

	BodyType type;
	Vec3 position;
	Vec3 velocity;
	Vec3 acceleration;
	float angle;

	SDL_Surface* sprite;
	SDL_Rect spriteShape;
	const char* spriteName;

	const char* objectName;
	

};
class Body
{
public:

	BodyType m_type;
	float m_mass;
	Vec3 m_position;
	Vec3 m_velocity;
	float m_angle;

	SDL_Surface* m_sprite;
	SDL_Rect m_spriteShape;
	const char* m_spriteName;

	const char* m_objectName;

	Body(const BodyDefinition* bodyDef);
	~Body();

	Body& operator=(const Body& newBody);

	void SetType(BodyType newType);
	void SetMass(float newMass);

	void Update(float timeStep);
	void ApplyForce(Vec3& force);

	
	void FindSprite(const char* spriteName);
	bool SetSprite();

	void GiveName(const char* objectName);



private:
	Vec3 m_acceleration;
};
#endif

///// Just some extra stuff for fun
//
//	///		A Sphere could be thought of as a just a center point (x,y,z) 
//	///		comes from Vec3 then just add a radius
//struct Sphere : public Vec3 {
//	float r;
//	Sphere(float s = 0.0f) { x = s; y = s; z = s; r = s; }
//
//	Sphere(float _x, float _y, float _z, float _r) {
//		x = _x; y = _y; z = _z; r = _r;
//	}
//
//	inline Sphere(const Sphere& v) {
//		x = v.x;  y = v.y;  z = v.z; r = v.r;
//	}
//
//	inline void print() {
//		printf("%f %f %f %f\n", x, y, z, r);
//	}
//};


///// A solution to the circular dependency problem.  
///// This is what is called a "forward declaration."
///// See note 3 at the bottom of this page.
//class VMath;
//
/////  float x,y,z; came from vector as the normal to the surface + distance to the surface (d) 
//struct Plane : public Vec3 {
//	float d;
//
//	/// Here's a set of constructors
//	inline Plane(float _s = 0.0f) {
//		x = _s;
//		y = _s;
//		z = _s;
//		d = _s;
//	}
//	inline Plane(float _x, float _y, float _z, float _d) {
//		x = _x;
//		y = _y;
//		z = _z;
//		d = _d;
//	}
//
//	inline Plane(const Plane& v) {
//		x = v.x;
//		y = v.y;
//		z = v.z;
//		d = v.d;
//	}
//
//	/// This creates a normalized equation of a plane.
//	/// Important: See note 3.
//	inline Plane(const Vec3& v0, const Vec3& v1, const Vec3& v2) {
//		Vec3 a = v1 - v0;
//		Vec3 b = v2 - v0;
//		Vec3 n = Vec3(a.y * b.z - a.z * b.y, /// This is the cross product
//			a.z * b.x - a.x * b.z,
//			a.x * b.y - a.y * b.x);
//		float magnitude = float(sqrt(n.x * n.x + n.y * n.y + n.z * n.z)); /// normalize
//
//		Plane(x / magnitude, y / magnitude, z / magnitude, a.x * b.x + a.y * b.y + a.z * b.z);
//	}
//
//
//	/// Convert this plane into a normalized plane
//	inline void normalize() {
//		float a = sqrt(x*x + y * y + z * z);
//		x /= a;
//		y /= a;
//		z /= a;
//		d /= a;
//	}
//	inline void print() {
//		printf("%f %f %f %f\n", x, y, z, d);
//	}
//};