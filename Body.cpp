#include "Body.h"

Body::Body(const BodyDefinition* bodyDef) { //Using const
	
	m_type = bodyDef->type;
	
	m_position = bodyDef->position;
	m_velocity = bodyDef->velocity;

	m_sprite = bodyDef->sprite;
	m_spriteShape = bodyDef->spriteShape;

	if (m_type == dynamicBody) {
		m_mass = 1.0f;
	}
	else {
		m_mass = VERY_SMALL;
	}

}

Body::~Body() {

}

Body& Body::operator=(const Body& newBody) { //For linked list use, when we state a Body will equal another, just to be on the safe side
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
	/*EQXNS Of Motions*/
	m_position += m_velocity * timeStep + (0.5f * m_acceleration * timeStep * timeStep);

	m_velocity += m_acceleration * timeStep;

	/*Simulating the ground*/
	//Fix this make this a box
	if (m_position.y <= 1.0f) {
		m_velocity.y = sqrt(2.0f * 1.0f * 9.81f / 1.0f);
		m_position.y = 1.0f;
	}
	if (m_position.y >= 7.0f) {
		m_velocity.y = -sqrt(2.0f * 1.0f * 9.81f / 1.0f);
		m_position.y = 7.0f;
	}
	if (m_position.x >= 12.0f) {
		m_velocity.x = -sqrt(2.0f * 1.0f / 1.0f);
		m_position.x = 12.0f;
	}
	if (m_position.x <= 1.0f) {
		m_velocity.x = sqrt(2.0f * 1.0f / 1.0f);
		m_position.x = 1.0f;
	}

	if(m_type == staticBody)
		m_velocity.Load(0.0f, 0.0f, 0.0f);


}

/*FUTURE IMPLEMENTATIONS*/
//Collisions, Impulse, Momentum, etc
//Multiple forces
void Body::ApplyForce(Vec3& force) { 

	/*IMPORTANT*/
	//This will be the sum of all force acting on a object
	//Create seprate function for apply world forces
	m_acceleration = force / m_mass + GRAVITY; 

	if (m_type == staticBody)
		m_acceleration.Load(0.0f, 0.0f, 0.0f);
}

void Body::FindSprite(const char* spriteName) {
	m_spriteName = spriteName;
}

bool Body::SetSprite() {
	//m_sprite = SDL_LoadBMP(m_spriteName);

	m_sprite = IMG_Load(m_spriteName);

	if (m_sprite == nullptr) {
		return false;
	}

	return true;
}