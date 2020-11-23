#include "RenderComponent.h"

#include "../RenderCore/Model/Model.h"

RenderComponent::RenderComponent(Model* model) :
	Component( ID ),
	m_model( model )
{}

RenderComponent::~RenderComponent()
{
	if ( m_model )
	{
		delete m_model;
		m_model = nullptr;
	}
}

void RenderSystem::Update( const float deltaTime )
{
	for ( auto& c : m_components )
	{

		RenderComponent* render = std::get<RenderComponent*>( c );
		TransformComponent* transform = std::get<TransformComponent*>( c );

		

	}

}

std::vector<Model*> RenderSystem::GetModels()
{
	std::vector<Model*> models;
	for ( auto& c : m_components )
	{

		RenderComponent* render = std::get<RenderComponent*>( c );
		models.push_back( render->m_model );
	}
	return models;
}



