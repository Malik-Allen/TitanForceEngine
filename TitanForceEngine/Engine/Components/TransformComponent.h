#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class TransformComponent : public ECS::Component
{
	friend class TransformUpdater;
public:

	static constexpr uint64_t ID = GENERATE_ID( "TransformComponent" );

	TransformComponent() :
		Component( ID ),
		m_position( glm::vec3( 0.0f ) ),
		m_rotation( glm::vec3( 0.0f, 1.0f, 0.0f ) ),
		m_angle( 0.0f ),
		m_scale( glm::vec3( 1.0f, 1.0f, 1.0f ) )
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate( model, m_position );
		model = glm::rotate( model, m_angle, m_rotation );
		model = glm::scale( model, m_scale );
		m_transform = model;
	}

	TransformComponent( glm::vec3 position, float angle, glm::vec3 rotation, glm::vec3 scale ) :
		Component( ID ),
		m_position( position ),
		m_angle( angle ),
		m_rotation( rotation ),
		m_scale( scale )
	{
		glm::mat4 model = glm::mat4( 1.0f );
		model = glm::translate( model, m_position );
		model = glm::rotate( model, m_angle, m_rotation );
		model = glm::scale( model, m_scale );
		m_transform = model;
	}

	~TransformComponent() {}

	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetRotation() const { return m_rotation; }
	glm::mat4 GetTransform() const { return m_transform; }

private:

	glm::vec3	m_position;
	float		m_angle;
	glm::vec3	m_rotation;
	glm::vec3	m_scale;
	glm::mat4	m_transform;

};

class TransformUpdater : public ECS::System<TransformComponent>
{
public:
	static constexpr uint64_t ID = GENERATE_ID( "TransformUpdater" );

	TransformUpdater() :
		System( ID )
	{}

	~TransformUpdater() {}

	virtual void Update( const float deltaTime ) override final;

};



#endif // !TRANSFORM_H

