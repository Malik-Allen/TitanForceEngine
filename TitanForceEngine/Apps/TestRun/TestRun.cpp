#include "TestRun.h"

#include "../../Engine/Camera/Camera.h"

TestRun::TestRun() :
	App( "Test Run" ),
	m_world( new ECS::World() )
{}

TestRun::~TestRun()
{}

bool TestRun::OnCreate()
{
	// Systems
	m_world->RegisterSystem<CameraSystem>();

	// Entities
	m_world->CreateEntitiesWithComponents<CameraComponent>( 1 );
	

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
}
