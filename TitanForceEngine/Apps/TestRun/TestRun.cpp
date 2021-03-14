#include "TestRun.h"

#include "Scenes/Scene1.h"

TestRun::TestRun() :
	IApp( "Test Run" )
{}

TestRun::~TestRun()
{}

bool TestRun::OnCreate()
{

	m_scene = new Scene1();
	if ( !m_scene->OnCreate() )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create Scene1 in Test Run!" );
		return false;
	}

	return true;
}

void TestRun::OnDestroy()
{

}

void TestRun::Update( const float deltaTime )
{

	m_scene->Update( deltaTime );

	m_renderer->RenderScene( m_scene );


}
