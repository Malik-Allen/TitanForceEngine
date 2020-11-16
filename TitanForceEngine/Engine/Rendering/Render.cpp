#include "Render.h"

#include "../3D/Mesh.h"

RenderComponent::RenderComponent(std::vector<Mesh*> subMeshes ) :
	Component( ID ),
	m_subMeshes( subMeshes )
{}

RenderComponent::~RenderComponent()
{
	for ( Mesh* m : m_subMeshes )
	{
		if ( m )
		{
			delete m;
			m = nullptr;
		}
	}
	m_subMeshes.clear();
}

void RenderSystem::Update( const float deltaTime )
{
	for ( auto& c : m_components )
	{

		RenderComponent* render = std::get<RenderComponent*>( c );
		TransformComponent* transform = std::get<TransformComponent*>( c );

		for ( Mesh* m : render->m_subMeshes )
		{
			if ( m )
			{
				m->Render( transform->GetTransform() );
			}
		}

	}

}

std::vector<Mesh*> RenderSystem::GetAllMeshes() const
{
	std::vector<Mesh*> meshes;

	for ( auto& c : m_components )
	{

		RenderComponent* render = std::get<RenderComponent*>( c );

		for ( Mesh* m : render->m_subMeshes )
		{
			if ( m )
			{
				meshes.push_back( m );
			}
		}

	}

	return meshes;

}
