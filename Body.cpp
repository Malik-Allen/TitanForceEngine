#include "Body.h"



Body::Body(const BodyDefinition* bodyDef)
{
	m_type = bodyDef->type;
	m_position = bodyDef->position;
	m_velocity = bodyDef->velocity;
	m_angle = bodyDef->angle;

	m_sprite = bodyDef->sprite;
	m_spriteShape = bodyDef->spriteShape;
	m_spriteName = bodyDef->spriteName;

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

	m_sprite = newBody.m_sprite;
	m_spriteShape = newBody.m_spriteShape;

	return *this;
}

void Body::SetType(BodyType newType) {
	m_type = newType;

	if (m_type == dynamicBody) {
		m_mass = 1.0f;
	}
	else {
		m_mass = VERY_SMALL;
	}
}

void Body::SetMass(float newMass) {

	if (newMass < 0.0f) {
		m_mass = 1.0f;
	}
	else {
		m_mass = newMass;
	}
}

void Body::Update(float timeStep) {
	m_position += m_velocity * timeStep + (0.5f * m_acceleration * timeStep * timeStep);

	m_velocity += m_acceleration * timeStep;

	if (m_type == staticBody)
		m_velocity.Load(0.0f, 0.0f, 0.0f);
}

/*TEMPORARY*/
//In order to apply World Gravity apply a tempVec if no other forces are acting upon this object
void Body::ApplyForce(Vec3& force) {

	m_acceleration = force / m_mass + EARTH_GRAVITY;

	if (m_type == staticBody || m_type == kinematicBody)
		m_acceleration.Load(0.0f, 0.0f, 0.0f);
}

//Only Locates sprite for this body
void Body::FindSprite(const char* spriteName) {

	m_spriteName = spriteName;
}

//Returns false if sprite cannot be found
//True if it exists
bool Body::SetSprite() {

	m_sprite = IMG_Load(m_spriteName);

	if (m_sprite == nullptr) {
		return false;
	}

	return true;
}

void Body::GiveName(const char* objectName) {
	m_objectName = objectName;
}


