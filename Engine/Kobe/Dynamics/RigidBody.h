#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Math.h"

using namespace Kobe;

// staticBody:		zero mass, zero velocity, only moved manually
// kinematicBody:	zero mass, non-zero velocity set by user
// dynamicBody:		positive mass, non-zero velocity determined by forces applied on body
enum BodyType {
	staticBody = 0,
	kinematicBody,
	dynamicBody
};

// Body Defintion will define the components of any RigidBody that uses it
// Sharing the same definition across multiple Game Objects
struct BodyDefinition {

	// Default constructor will populate all members with default values
	BodyDefinition() {
		type = dynamicBody;
		mass = 1.0f;
		radius = 1.0f;
		position = Vector3(0.0f);
		velocity = Vector3(0.0f);
		rotateInertia = 1.0f;
		orientation = Vector3(0.0f);
		angularVelocity = Vector3(0.0f);
	}

	BodyType type;
	float mass;
	float radius;

	Vector3 position;
	Vector3 velocity;

	float rotateInertia;
	Vector3 orientation;
	Vector3 angularVelocity;
};

// Class used to represent an object with a center of gravity that is at the center
class RigidBody
{
public:

	//RigidBody(const RigidBody&) = delete;
	//RigidBody&operator=(const RigidBody&) = delete;
	//RigidBody(const RigidBody&&) = delete;
	//RigidBody&operator=(const RigidBody&&) = delete;

	BodyType m_bodyType;	// Default to dynamicBody
	float m_mass;
	float m_radius;

	
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_momentum;

	Matrix3 m_rotateInertia;
	Vector3 m_orientation;
	Vector3 m_angularVelocity;

	// Default constructor will populate rigidBody with default values
	RigidBody();

	// A constructor that populates mass, position and velocity
	RigidBody(float mass, const Vector3& position, const Vector3& velocity);

	// Create a RigidBody with the designated bodytype
	RigidBody(const BodyType& bodyType, float mass, const Vector3& position, const Vector3& velocity);

	// Construct a RigidBody using a Pre-defined Body 
	RigidBody(const BodyDefinition& bodyDef);

	// Change the mass of this RigidBody
	void SetMass(float mass);

	// Change the radius of this RigidBody
	void SetRadius(float radius);

	// Change the rotational inertia of this RigidBody
	void SetRotateInertia(float rotateInertia_);

	// Update this RigidBody using Newtons Laws of Physics
	void Update(float deltaTime);

	// Apply a force that acts upon this RigidBody
	void ApplyForce(const Vector3& force);

	// Apply a torque onto this RigidBody
	void ApplyTorque(const Vector3& torque);

	inline void print() {
		printf("Mass:\t%1.8f\nRadius:\t%1.8f\nType:\t%i\nPos:\t%1.8f\t%1.8f\t%1.8f\nVel:\t%1.8f\t%1.8f\t%1.8f\nAccel:\t%1.8f\t%1.8f\t%1.8f\nOrien:\t%1.8f\t%1.8f\t%1.8f\nAngVel:\t%1.8f\t%1.8f\t%1.8f\nAngAcl:\t%1.8f\t%1.8f\t%1.8f\n\n",
			m_mass, m_radius, m_bodyType, m_position.x, m_position.y, m_position.z, m_velocity.x, m_velocity.y, m_velocity.z, m_acceleration.x,
			m_acceleration.y, m_acceleration.z, m_orientation.x, m_orientation.y, m_orientation.z, m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 
			m_angularAcceleration.x, m_angularAcceleration.y, m_angularAcceleration.z);
	}

	inline void printPos() {
		printf("Pos:\t%1.8f\t%1.8f\t%1.8f\n", m_position.x, m_position.y, m_position.z);
	}

	inline void printVel() {
		printf("Vel:\t%1.8f\t%1.8f\t%1.8f\n", m_velocity.x, m_velocity.y, m_velocity.z);
	}

	inline void printAccel() {
		printf("Accel:\t%1.8f\t%1.8f\t%1.8f\n", m_acceleration.x, m_acceleration.y, m_acceleration.z);
	}

	inline void printOrientation() {
		printf("Orien:\t%1.8f\t%1.8f\t%1.8f\n", m_orientation.x, m_orientation.y, m_orientation.z);
	}

	inline void printAngularVel() {
		printf("AngVel:\t%1.8f\t%1.8f\t%1.8f\n", m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);
	}

	inline void printAngularAccel() {
		printf("AngAcl:\t%1.8f\t%1.8f\t%1.8f\n", m_angularAcceleration.x, m_angularAcceleration.y, m_angularAcceleration.z);
	}

//private:
	Vector3 m_acceleration;
	Vector3 m_angularAcceleration;
};

#endif

// Note #1:
/// Constrain the X, Y, Z Rotation bools