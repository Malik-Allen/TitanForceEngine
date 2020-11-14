#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

#include <glm.hpp>


class LightSourceComponent : public ECS::Component
{
public:

	static constexpr uint64_t ID = GENERATE_ID( "LightSourceComponent" );

	LightSourceComponent() :
		Component( ID ),
		m_ambient( 0.2f ),
		m_diffuse( 0.8f ),
		m_colour( glm::vec3( 1.0f, 1.0f, 1.0f ) )
	{}

	LightSourceComponent( float ambient, float diffuse, glm::vec3 colour ) :
		Component( ID ),
		m_ambient( ambient ),
		m_diffuse( diffuse ),
		m_colour( colour )
	{}

	~LightSourceComponent() {}

	float* GetAmbient() { return &m_ambient; }
	float* GetDiffuse() { return &m_diffuse; }
	glm::vec3 GetColour() { return m_colour; }

private:

	float		m_ambient;
	float		m_diffuse;
	glm::vec3	m_colour;

};


#endif // !LIGHTSOURCE_H

