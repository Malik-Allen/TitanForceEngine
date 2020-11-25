#ifndef RENDER_H
#define RENDER_H

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

#include "../RenderCore/Model/Model.h"

class RenderComponent : public ECS::Component
{
	friend class RenderSystem;

public:

	static constexpr uint64_t ID = GENERATE_ID( "RenderComponent" );


	RenderComponent( Model* model );

	~RenderComponent();

	Model* GetModel() const { return m_model; }

private:

	Model*	m_model;

};


class RenderSystem : public ECS::System<RenderComponent, TransformComponent>
{

public:

	static constexpr uint64_t ID = GENERATE_ID( "RenderSystem" );

	RenderSystem() :
		System( ID )
	{}

	~RenderSystem() {}

	virtual void Update( const float deltaTime ) override final;

	std::vector<Model*> GetModels();
};



#endif // !RENDER_H

