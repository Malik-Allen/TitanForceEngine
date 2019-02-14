#ifndef BODY_H
#define BODY_H

#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"

/// Current capabilities of Body Class includes:
/// BodyType impacts the type of physics acted upon the GameObject
/// Physics through the equations of motion in 3 dimensions
/// Rotation physics applied the Body -- Feb 13, 2019

// Future Iterations for the Body Class includes:
// Communication with a static collision class
// Multiple apply force implimentation -- Feb 13, 2019


using namespace MATH;

//staticBody: zero mass, zero velocity, only moved manually
//kinematicBody: zero mass, non-zero velocity is set by user
//dynamicBody: positive mass, non-zero velocity determined by forces
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
	}

	BodyType type;
	Vec3 position;
	Vec3 velocity;
};

class Body
{
public:

	BodyType m_type;
	float m_mass;
	Vec3 m_position;
	Vec3 m_velocity;

	float m_angle;
	float m_angularVel;
	float m_rotationalInertia;

	Body(const BodyDefinition* bodyDef);
	~Body();

	Body& operator=(const Body& newBody);

	void SetType(BodyType newType);
	void SetMass(float newMass);
	void SetRotationalInertia(float newRotationalInertia);

	void Update(float timeStep);
	void ApplyForce(Vec3& force);
	void ApplyTorque(float torque);


private:
	Vec3 m_acceleration;
	float m_angularAccel;
};
#endif