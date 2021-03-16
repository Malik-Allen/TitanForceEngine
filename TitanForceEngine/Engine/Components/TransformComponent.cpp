#include "TransformComponent.h"

void TransformUpdater::Update( const float deltaTime )
{
	
	for ( auto & c : m_components )
	{
		TransformComponent* t = std::get<TransformComponent*>( c );

		if ( t )
		{
			t->m_angle += 0.05f;	// just to apply a rotation
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate( model, t->m_position );
			model = glm::rotate( model, t->m_angle, t->m_rotation );
			model = glm::scale( model, t->m_scale );
			t->m_transform = model;
			
		}
		

	}

}
