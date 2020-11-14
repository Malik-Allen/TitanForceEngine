#include "Transform.h"

void TransformUpdater::Update( const float deltaTime )
{
	
	for ( auto & c : m_components )
	{
		TransformComponent* t = std::get<TransformComponent*>( c );
		t->m_transform = glm::mat4();
		t->m_transform = glm::translate( t->m_transform, t->m_position );
		t->m_transform = glm::rotate( t->m_transform, t->m_angle, t->m_rotation );
		t->m_transform = glm::scale( t->m_transform, t->m_scale );

	}

}
