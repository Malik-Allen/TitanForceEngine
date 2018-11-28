#ifndef BODY_H
#define BODY_H

#include "Vector.h"
//#include ""
#include <SDL.h>
#include <SDL_image.h>
#include <string>

/*BODY TYPE*/
//StaticBody: zero mass, zero velocity, only moved manually
//KinematicBody: zero mass, non-zero velocity is set by the user
//DynamicBody: positive mass, non-zero velocity determined by forces

enum BodyType{ //An enumartor is an Enumarated type defined with a custom set of identifiers
	staticBody = 0,
	kinematicBody,
	dynamicBody

}; //Objects of the enumarated type can take any of the enumartors as value

using namespace MATH;

/*FOR NOW*/
//Would like to define gravity in world class in future interation

struct BodyDefinition { //A data structure is a group of data elements under one name

	BodyDefinition() { //Constructor will set the data elements to default values

		type = staticBody;
		angle = 0.0f;
		position.Load(0.0f, 0.0f, 0.0f);
		velocity.Load(0.0f, 0.0f, 0.0f);

		sprite = nullptr;
		spriteName = nullptr;
	}

	BodyType type;

	float angle;

	Vec3 position;
	Vec3 velocity;
	Vec3 acceleration;

	/*FOR NOW*/
	//SDL Sprite will be handle here until I learn otherwise
	SDL_Surface* sprite; 
	SDL_Rect spriteShape;
	const char* spriteName;

};

class Body { //The class will hold the functionality of Body
public:

	BodyType m_type; //m_ naming convention to differentiate local variables

	float m_mass;

	Vec3 m_position;
	Vec3 m_velocity;

	SDL_Surface* m_sprite;
	SDL_Rect m_spriteShape;
	const char* m_spriteName;

	Body(const BodyDefinition* bodyDef);
	~Body();

	void SetType(BodyType newType);
	void SetMass(float newMass);

	void Update(float timeStep);
	void ApplyForce(Vec3& force);

	/*FOR NOW*/
	//Must be a better way to pass the file location
	void FindSprite(const char* spriteName);
	bool SetSprite(); 

	Body& operator=(const Body& newBody);

private:

	Vec3 m_acceleration;

};

#endif