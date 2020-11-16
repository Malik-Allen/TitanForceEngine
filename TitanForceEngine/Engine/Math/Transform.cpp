#include "Transform.h"

void TransformUpdater::Update( const float deltaTime )
{
	
	for ( auto & c : m_components )
	{
		TransformComponent* t = std::get<TransformComponent*>( c );

		if ( t )
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate( model, t->m_position );
			model = glm::rotate( model, t->m_angle, t->m_rotation );
			model = glm::scale( model, t->m_scale );
			t->m_transform = model;

			/*t->m_transform = glm::translate( t->m_transform, t->m_position );
			t->m_transform = glm::rotate( t->m_transform, t->m_angle, t->m_rotation );
			t->m_transform = glm::scale( t->m_transform, t->m_scale );*/
		}
		

	}

}
