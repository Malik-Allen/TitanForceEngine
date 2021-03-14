#include "Engine.h"
#include "../AppCore/App.h"
#include "../Devices/Window.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/include/Utility/Debug.h"

#include <iostream>

std::unique_ptr<Engine> Engine::g_engineInstance( nullptr );

Engine::Engine() :
	m_engineName(""),
	m_engineClock( nullptr ),
	m_isRunning( false ),
	m_isAppRunning( false ),
	m_fps( 120 ),
	m_app(nullptr),
	m_window(nullptr)
{}

Engine::~Engine() {}

// Initializes Engine Components
bool Engine::Init( 
	const char* engineName, 
	const unsigned int fps, 
	const int windowWidth, 
	const int windowHeight )
{
	DEBUG_INIT();
	m_engineName = engineName;

	m_engineClock = new EngineClock();
	if (m_engineClock == nullptr)
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create engine clock!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to create engine clock!" );
		return false;
	}

	m_fps = fps;
	m_engineClock->SetFPS( m_fps );


	m_window = new Window();
	const char* appName = "Titan Force Engine";
	if ( m_app != nullptr )	
		// If an application already exists lets name the window after that application
	{
		appName = m_app->GetAppName().c_str();
	}

	if ( m_window->OnCreate( appName, windowWidth, windowHeight ) == false )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create window!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to create window!" );
		return false;
	}

	m_isRunning = true;
	return m_isRunning;
}

// Loads Passed Application, beginning with creating its desired renderer
bool Engine::LoadApplication(IApp* app)
{

	if (app == nullptr) {
		DEBUG_LOG( LOG::WARNING, "Failed to laod application: App is null" );
		CONSOLE_LOG( LOG::WARNING, "Failed to laod application: App is null" );
		return false;
	}

	m_app = app;
	
	// TODO: RendererManager on the IApp* will used to create renderer
	if ( m_app->CreateRenderer( m_engineName, 1, true, m_window ) == false )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create application renderer!" );
		CONSOLE_LOG( LOG::ERRORLOG, "Failed to create application renderer!" );
		return false;
	}

	if ( m_app->OnCreate() == false )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create application!" );
		CONSOLE_LOG( LOG::ERRORLOG, "Failed to create application!" );
		return false;
	}

	m_isAppRunning = true;

	return m_isAppRunning;

}


// Runs current application
void Engine::Run()
{

	while ( m_isRunning )
	{
		m_engineClock->UpdateFrameTicks();
		Update(m_engineClock->GetDeltaTime() );
		Sleep(m_engineClock->GetSleepTime(m_engineClock->GetFPS() ) );
	}

	if ( !m_isRunning )
	{
		OnDestroy();
	}

}


bool Engine::IsRunning() const
{
	return m_isRunning;
}

bool Engine::IsAppRunning() const
{
	return m_isAppRunning;
}

void Engine::Exit()
{
	m_isRunning = false;
}

void Engine::ExitApp()
{
	m_isAppRunning = false;
}

Engine* Engine::Get()
{
	if (g_engineInstance == nullptr )
	{
		g_engineInstance.reset( new Engine );
		return g_engineInstance.get();
	}
	return g_engineInstance.get();
}


void Engine::OnDestroy()
{
	ExitApp();
	if (m_app) {
		delete m_app;
		m_app = nullptr;
	}

	if ( m_window )
	{
		m_window->OnDestroy();
		delete m_window;
		m_window = nullptr;
	}


	if (m_engineClock) {
		delete m_engineClock;
		m_engineClock = nullptr;
	}

}

void Engine::Update( const float deltaTime ) {

	std::cout << "Tick Time: " << deltaTime << std::endl;

	if ( m_app != nullptr )
	{
		if ( m_isAppRunning )
		{
			m_app->Update ( deltaTime );
		}
		else
		{
			m_app->OnDestroy ();
		}
	}

	
	glfwPollEvents();

}



