#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector.h"

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
		type = dynamicBody;
		position.set(0.0f, 0.0f, 0.0f);
		velocity.set(0.0f, 0.0f, 0.0f);
		rotationalInertia = 1.0f;
		radius = 1.0f;
	}

	BodyType type;
	Vec3 position;
	Vec3 velocity;
	float rotationalInertia;
	float radius;
};

class RigidBody
{
public:
	BodyType m_type;
	float m_mass;
	Vec3 m_position;
	Vec3 m_velocity;

	float m_angle;
	float m_angularVel;
	float m_rotationalInertia;

	float m_radius;

	RigidBody(BodyType newType, float newMass, Vec3& newPos, Vec3& newVel,
		float newAngle, float newAngleVel, float newRotateInert, float newRadius);
	RigidBody(const BodyDefinition* bodyDef);
	~RigidBody();

	RigidBody& operator=(const RigidBody& newBody);

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

