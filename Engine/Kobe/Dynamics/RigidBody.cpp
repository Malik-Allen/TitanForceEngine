#include "RigidBody.h"

/// Default constructor will populate rigidBody with default values
RigidBody::RigidBody() {
	m_bodyType = dynamicBody;
	m_mass = 1.0f;
	m_radius = 1.0f;
	m_position = Vector3(0.0f);
	m_velocity = Vector3(0.0f);
	m_momentum = Vector3(0.0f);
	m_rotateInertia.LoadIdentity();
	m_orientation = Vector3(0.0f);
	m_angularVelocity = Vector3(0.0f);
}

/// A constructor that populates mass, position and velocity
RigidBody::RigidBody(float mass, const Vector3& position, const Vector3& velocity) {
	m_bodyType = dynamicBody;

	if (mass > 0.0f)	// Checking to see that the passed mass is a non-zero positive number
		m_mass = mass;
	else
		m_mass = 1.0f;	// If not we set the mass to 1 

	m_radius = 1.0f;

	m_position = position;
	m_velocity = velocity;
	m_momentum = m_velocity * m_mass;

	m_rotateInertia.LoadIdentity();	// Will load an identity Matrix of 1.0f when no rotation inertia is given
	m_orientation = Vector3(0.0f);
	m_angularVelocity = Vector3(0.0f);
}

/// Create a RigidBody with the designated bodytype
RigidBody::RigidBody(const BodyType& bodyType, float mass, const Vector3& position, const Vector3& velocity) {
	m_bodyType = bodyType;

	if (m_bodyType != dynamicBody)	// Static and Kinematic Bodies have zero-mass, but we do not like to work with zeroes in our language
		m_mass = VERY_SMALL;
	else {
		if (mass > 0.0f)			// Making sure our mass is again a non-zero positive number
			m_mass = mass;
		else
			m_mass = 1.0f;			// Default mass to 1 in the case of a zero mass
	}

	m_radius = 1.0f;

	m_position = position;

	if (m_bodyType != staticBody) {	// A static body will have a velocity of zero
		m_velocity = velocity;		// Kinematic and Dynamic Body types will take the passed velocity
		m_momentum = m_velocity * m_mass;
	}
	else {
		m_velocity = Vector3(0.0f);
		m_momentum = Vector3(0.0f);
	}
	
	m_rotateInertia.LoadIdentity();
	m_orientation = Vector3(0.0f);
	m_angularVelocity = Vector3(0.0f);

}

/// Construct a RigidBody using a Pre-defined Body
RigidBody::RigidBody(const BodyDefinition& bodyDef) {
	m_bodyType = bodyDef.type;

	if (m_bodyType != dynamicBody)		// If the passed the bodyType is static or kinematic, set the mass to an approxamation of zero
		m_mass = VERY_SMALL;
	else {
		if (bodyDef.mass > 0.0f)		// Otherwise we check to see if the mass is a non-zero positive number 
			m_mass = bodyDef.mass;		// If so we set the mass 
		else
			m_mass = 1.0f;				// If the mass is zero or negative number we set it to 1 by default
	}

	if (bodyDef.radius > 0.0f) 
		m_radius = bodyDef.radius;
	else
		m_radius = 1.0f;

	m_position = bodyDef.position;

	if (m_bodyType != staticBody) {		// When the bodytype is static we set its velocity to zero

		m_velocity = bodyDef.velocity;	// Otherwise we set it to the passed velocity
		m_momentum = m_velocity * m_mass;

		if (bodyDef.rotateInertia > 0.0f)
			m_rotateInertia.LoadIdentity((1.0f / bodyDef.rotateInertia));
		else
			m_rotateInertia.LoadIdentity();

		m_orientation = bodyDef.orientation;
		m_angularVelocity = bodyDef.angularVelocity;
	}
	else {
		m_velocity = Vector3(0.0f);
		m_momentum = Vector3(0.0f);
		m_rotateInertia.LoadIdentity();
		m_orientation = Vector3(0.0f);
		m_angularVelocity = Vector3(0.0f);
	}
}

/// Change the mass of this RigidBody
void RigidBody::SetMass(float mass) {
	if (mass > 0.0f) {
		m_mass = mass;
	}
}

/// Change the radius of this RigidBody
void RigidBody::SetRadius(float radius) {
	if (radius > 0.0f)
		m_radius = radius;
}

/// Change the rotational inertia of this RigidBody
void RigidBody::SetRotateInertia(float rotateInertia_) {
	if (rotateInertia_ > 0.0f) {
		m_rotateInertia.LoadIdentity((1.0f / rotateInertia_));
	}
}

/// Update this RigidBody using Newtons Laws of Physics
void RigidBody::Update(float deltaTime) {

	if (m_bodyType != staticBody) {		// We apply the laws of physics to kinematic and dynamic bodies

		// Pos = Vel * time + 1/2 * accel * time^2
		m_position += m_velocity * deltaTime + (0.5f * m_acceleration * deltaTime * deltaTime);

		// Vel = accel * time;
		m_velocity += m_acceleration * deltaTime;

		// Momentum = Velocity * Mass
		m_momentum = m_velocity * m_mass;

		// Angle = AngularVel * time + 1/2 * AngularAccel * time^2
		m_orientation += m_angularVelocity * deltaTime + (0.5f * m_angularAcceleration * deltaTime * deltaTime);

		// AngularVel = AngularAccel * time;
		m_angularVelocity += m_angularAcceleration * deltaTime;
	}
	else {
		m_velocity = Vector3(0.0f);
		m_angularVelocity = Vector3(0.0f);
	}
}

/// Apply a force that acts upon this RigidBody
void RigidBody::ApplyForce(const Vector3& force) {

	if (m_bodyType == dynamicBody) 			// Dynamic Bodies are affected by forces 
		m_acceleration = force / m_mass;
	else 
		m_acceleration = Vector3(0.0f);		// Static and Kinematic bodies are not affected by forces
	
}

/// Apply torque to this RigidBody
void RigidBody::ApplyTorque(const Vector3& torque) {
	
	if (m_bodyType == dynamicBody) {			// Torque will only affect Dynamic bodies
		m_angularAcceleration =  m_rotateInertia * torque;	// When calcuate the angular accleration it is the torque divided by the rotational inertia 
															// Instead I multiply the torque by 1 / rotateInertia which is the same thing! Mind Blown!
	}
	else
		m_angularAcceleration = Vector3(0.0f);
}



// #Note 1:
/// In the future I may want to create constructors that takes an angle/orientation(Vector3) as an arguemnet
/// Very much still up for debate

// Note #2:
/// Rotational inertia is an identity matrix so that when the orientation of an object is changed it can be manipulated by an identity matrix
/// Also when I store a variable in the rotational Inertia matrix I make it 1 / rotational inertia in order to divide the torque by the rotational inertia through multiplication