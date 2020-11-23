#include "Scene1.h"

#include "../../../Engine/Components/RenderComponent.h"
#include "../../../Engine/RenderCore/Camera/Camera.h"

Scene1::Scene1()
{}

Scene1::~Scene1()
{}

bool Scene1::OnCreate()
{
	// Systems
	m_world->RegisterSystem<TransformUpdater>();
	m_world->RegisterSystem<CameraSystem>();
	m_world->RegisterSystem<RenderSystem>();

	// Camera
	ECS::EntityId camera = m_world->CreateEntities( 1 ).front();
	m_world->AddComponentToEntity<CameraComponent>( camera );
	m_world->AddComponentToEntity<TransformComponent>(
		camera
	);

	// Shader
	ShaderLinker* shaderLinker = new ShaderLinker( "PhongShader" );
	shaderLinker->SubmitShader( new Shader( EShaderType::Vertex, "PhongVertex.glsl" ) );
	shaderLinker->SubmitShader( new Shader( EShaderType::Fragment, "PhongFragment.glsl" ) );
	shaderLinker->LinkShaders();

	// GameObject
	ECS::EntityId gameObject = m_world->CreateEntities( 1 ).front();

	// Attaching transform
	TransformComponent* transform = m_world->AddComponentToEntity<TransformComponent>(
		gameObject,
		glm::vec3(0.0f, 0.0f, -75.0f),
		0.0f,
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3(1.0f, 1.0f, 1.0f)
		);

	// Model
	Model* model1 = new Model( "./Resources/Models/F-16C.obj", "", shaderLinker, nullptr, transform );

	// Render component
	RenderComponent* r = m_world->AddComponentToEntity<RenderComponent>( gameObject, model1 );

	// Generating a lot of Entities
	std::vector<ECS::EntityId> entities = m_world->CreateEntities( 200 );

	glm::vec3 pos = glm::vec3( -50.0f, 0.0f, 0.0f );
	glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );

	for ( int i = 0; i < entities.size(); i++ )
	{
		pos += glm::vec3( 1.0f, 0.0f, 0.0f );
		
		

		TransformComponent* t = m_world->AddComponentToEntity<TransformComponent>(
			entities[i],
			pos,
			0.0f,
			glm::vec3( 0.0f, 1.0f, 0.0f ),
			scale
		);

		if ( i % 2 == 0 )
		{
			Model* model1 = new Model( "./Resources/Models/F-16C.obj", "", shaderLinker, nullptr, t );
			m_world->AddComponentToEntity<RenderComponent>( entities[i], model1 );
		}
		else
		{
			Model* model2 = new Model( "./Resources/Models/Mario.obj", "", shaderLinker, nullptr, t );
			m_world->AddComponentToEntity<RenderComponent>( entities[i], model2 );
		}
		

	}

	return true;
}

void Scene1::OnDestroy()
{}

void Scene1::Update( const float deltaTime )
{
	m_world->Update( deltaTime );
}
