#include "Body.h"


// Instantiates class with a BodyDefintion to set class default values
Body::Body(const BodyDefinition* bodyDef)
{
	m_type = bodyDef->type;
	m_position = bodyDef->position;
	m_velocity = bodyDef->velocity;

	m_angle = VERY_SMALL;		/// Not able to define this based on body definition unfortunately
	m_angularVel = VERY_SMALL;	/// Not able to define this based on body definition unfortunately
	m_angularAccel = VERY_SMALL;/// Not able to define this based on body definition unfortunately
	m_rotationalInertia = 1.0f;

	if (m_type == dynamicBody) {
		m_mass = 1.0f;
	}
	else {
		m_mass = VERY_SMALL;
	}
}

Body::~Body()
{
}

Body& Body::operator=(const Body& newBody) {
	m_type = newBody.m_type;
	m_position = newBody.m_position;
	m_velocity = newBody.m_velocity;

	m_angle = newBody.m_angle;
	m_angularVel = newBody.m_angularVel;
	m_rotationalInertia = newBody.m_rotationalInertia;

	return *this;
}

// Set the BodyType of this Body which includes:
// staticBody, kinematicBody, and dynamicBody
void Body::SetType(BodyType newType) {
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
void Body::SetMass(float newMass) {

	if (newMass < 0.0f) {
		m_mass = 1.0f;
	}
	else {
		m_mass = newMass;
	}
}

// The rotational inertia of this object
void Body::SetRotationalInertia(float newRotationalInertia) {
	m_rotationalInertia = newRotationalInertia;
}

// Updates the Body's position, velocity, angle and angularVel
// Using the Laws of Physics and Equations of Motion
// Requires delta time
void Body::Update(float timeStep) {
	m_position += m_velocity * timeStep + (0.5f * m_acceleration * timeStep * timeStep);

	m_velocity += m_acceleration * timeStep;

	m_angle += m_angularVel * timeStep + (0.5f * m_angularAccel * timeStep * timeStep);

	m_angularVel += m_angularAccel * timeStep;

	if (m_type == staticBody)
		m_velocity.Load(0.0f, 0.0f, 0.0f);
}

// Applies a force this this body
// Only applies to dynamicBodies
void Body::ApplyForce(Vec3& force) {

	m_acceleration = force / m_mass;

	if (m_type == staticBody || m_type == kinematicBody)
		m_acceleration.Load(0.0f, 0.0f, 0.0f);
}

// Applies torque to this Body
// Impacting the angular acceleration
void Body::ApplyTorque(float torque) {
	m_angularAccel = torque / m_rotationalInertia;
}




