#ifndef RENDER_H
#define RENDER_H

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

#include "../Math/Transform.h"

#include "../Core/Renderer.h"

#include <vector>

class Mesh;

class RenderComponent : public ECS::Component
{
	friend class RenderSystem;

public:

	static constexpr uint64_t ID = GENERATE_ID( "RenderComponent" );


	RenderComponent( std::vector<Mesh*> subMeshes );

	~RenderComponent();

private:

	std::vector<Mesh*>	m_subMeshes;

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

	std::vector<Mesh*> GetAllMeshes() const;
};



#endif // !RENDER_H

