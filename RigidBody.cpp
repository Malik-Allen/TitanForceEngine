#include "RigidBody.h"

// Instantiating withput a BodyDefinition needs:
// BodyType, Mass, Position, Velocity, Angle, Angular Velocity, and Rotational Inertia
RigidBody::RigidBody(BodyType newType, float newMass, Vec3& newPos, Vec3& newVel,
	float newAngle, float newAngleVel, float newRotateInert, float newRadius) {

	m_type = newType;

	if (m_type == dynamicBody) {
		if (newMass > 0.0f)
			m_mass = newMass;
		else
			m_mass = 1.0f;
	}
	else
		m_mass = VERY_SMALL;

	m_position = newPos;
	m_velocity = newVel;
	m_angle = newAngle;
	m_angularVel = newAngleVel;
	m_angularAccel = VERY_SMALL;
	m_rotationalInertia = newRotateInert;
	m_radius = newRadius;


	// Does Rotational Inertia have to be a non-zero positive number?
	/*if (newRotateInert > 0.0f)
		m_rotationalInertia = newRotateInert;
	else
		m_rotationalInertia = 1.0f;*/

}

// Instantiates class with a BodyDefintion to set class default values
RigidBody::RigidBody(const BodyDefinition* bodyDef)
{
	m_type = bodyDef->type;
	m_position = bodyDef->position;
	m_velocity = bodyDef->velocity;

	m_angle = VERY_SMALL;		/// Not able to define this based on body definition unfortunately
	m_angularVel = VERY_SMALL;	/// Not able to define this based on body definition unfortunately
	m_angularAccel = VERY_SMALL;/// Not able to define this based on body definition unfortunately
	m_rotationalInertia = 1.0f;

	m_radius = bodyDef->radius;

	if (m_type == dynamicBody) {
		m_mass = 1.0f;
	}
	else {
		m_mass = VERY_SMALL;
	}
}

RigidBody::~RigidBody()
{
}

RigidBody& RigidBody::operator=(const RigidBody& newBody) {
	m_type = newBody.m_type;
	m_position = newBody.m_position;
	m_velocity = newBody.m_velocity;

	m_angle = newBody.m_angle;
	m_angularVel = newBody.m_angularVel;
	m_rotationalInertia = newBody.m_rotationalInertia;

	m_radius = newBody.m_radius;

	return *this;
}

// Set the BodyType of this Body which includes:
// staticBody, kinematicBody, and dynamicBody
void RigidBody::SetType(BodyType newType) {
	m_type = newType;

	if (m_type == dynamicBody) {
		m_mass = 1.0f;
	}
	else {
		m_mass = VERY_SMALL;
	}
}

// Set the mass of this Body
// Anything less than zero, will result in a mass of 1.0f
void RigidBody::SetMass(float newMass) {

	if (newMass < 0.0f) {
		m_mass = 1.0f;
	}
	else {
		m_mass = newMass;
	}
}

// The rotational inertia of this object
void RigidBody::SetRotationalInertia(float newRotationalInertia) {
	m_rotationalInertia = newRotationalInertia;
}

// Updates the Body's position, velocity, angle and angularVel
// Using the Laws of Physics and Equations of Motion
// Requires delta time
void RigidBody::Update(float timeStep) {
	m_position += m_velocity * timeStep + (0.5f * m_acceleration * timeStep * timeStep);

	m_velocity += m_acceleration * timeStep;

	m_angle += m_angularVel * timeStep + (0.5f * m_angularAccel * timeStep * timeStep);

	m_angularVel += m_angularAccel * timeStep;

	if (m_type == staticBody)
		m_velocity.SetZero();

}

// Applies a force this this body
// Only applies to dynamicBodies
void RigidBody::ApplyForce(Vec3& force) {

	m_acceleration = force / m_mass;

	if (m_type == staticBody || m_type == kinematicBody)
		m_acceleration.SetZero();
}

// Applies torque to this Body
// Impacting the angular acceleration
void RigidBody::ApplyTorque(float torque) {
	m_angularAccel = torque / m_rotationalInertia;
}
