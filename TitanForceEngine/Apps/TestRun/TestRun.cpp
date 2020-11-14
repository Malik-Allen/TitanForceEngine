#include "TestRun.h"

#include "../../Engine/Debug/Debug.h"

#include "../../Engine/3D/OBJLoader.h"
#include "../../Engine/3D/Mesh.h"

#include "../../Engine/Math/Transform.h"
#include "../../Engine/Rendering/Render.h"

#include "../../Engine/Camera/Camera.h"
#include "../../Engine/FX/LightSource.h"

#include "../../Engine/Graphics/OpenGL/3D/OpenGLMesh.h"

TestRun::TestRun() :
	App( "Test Run", GraphicsAPI::OpenGL ),
	m_world( new ECS::World() )
{}

TestRun::~TestRun()
{}

bool TestRun::OnCreate()
{
	// Systems
	m_world->RegisterSystem<CameraSystem>();
	m_world->RegisterSystem<TransformUpdater>();
	RenderSystem* renderSystem = m_world->RegisterSystem<RenderSystem>();


	// Entities
	ECS::EntityId camera = m_world->CreateEntities( 1 ).front();
	ECS::EntityId gameObject = m_world->CreateEntities( 1 ).front();
	ECS::EntityId light = m_world->CreateEntities( 1 ).front();
	

	
	// Components
	LightSourceComponent* lightComponent = m_world->AddComponentToEntity<LightSourceComponent>( light );

	std::vector<LightSourceComponent*> lights;
	lights.push_back( lightComponent );
	
	CameraComponent* cameraComponent = m_world->AddComponentToEntity<CameraComponent>( camera, lights );
	m_world->AddComponentToEntity<TransformComponent>( camera );

	// 1. Load Mesh Obj through obj loader
	LoadOBJModel* objLoader = new LoadOBJModel();
	objLoader->LoadModel( "./Resource/Models/F-16C.obj" );
	std::vector<Mesh*> meshes;
	for ( auto& s : objLoader->GetSubMeshes() )
	{
		Mesh* m = new OpenGLMesh( cameraComponent, s, "PhongShader" );
		meshes.push_back( m );
	}

	

	m_world->AddComponentToEntity<RenderComponent>( gameObject, meshes );
	m_world->AddComponentToEntity<TransformComponent>( gameObject );

	for ( auto& m : renderSystem->GetAllMeshes() )
	{
		m_renderer->AddMesh( m );

	}
	

	return true;
}

void TestRun::OnDestroy()
{
	if ( m_world )
	{
		delete m_world;
		m_world = nullptr;
	}

	
}

void TestRun::Update( const float deltaTime )
{
	m_world->Update( deltaTime );

	m_renderer->Render();

}
