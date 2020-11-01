#include "TitanForceEngine.h"

#include "../Vulkan/VulkanRenderer.h"

#include "../Debug/Debug.h"

std::unique_ptr<TitanForceEngine> TitanForceEngine::engineInstance( nullptr );
void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );

TitanForceEngine::TitanForceEngine() :
	engineTimer( nullptr ),
	window( nullptr ),
	renderer( nullptr ),
	gameInterface( nullptr ),
	isRunning( false ),
	isGameRunning( false ),
	fps( 120 ),
	currentSceneNum( 0 )
{}

TitanForceEngine::~TitanForceEngine() {}

// Initializes Engine Components
bool TitanForceEngine::InitEngine( const std::string& engineName, const int windowWidth, const int windowHeight, const RendererType& typeOfRenderer )
{

	Debug::DebugInit();
	Debug::SetSeverity( MessageType::TYPE_INFO );	// Allows for all message types

	// 1. Create Our Engine Timer Class
	engineTimer = new EngineTimer();
	if ( engineTimer == nullptr )
	{
		Debug::FatalError( "Failed to create engine timer!", __FILE__, __LINE__ );
		return false;
	}

	// 2. Create our desired Renderer
	bool rendererInit = false;
	switch ( typeOfRenderer )
	{

	case RendererType::Vulkan:

		// For Vulkan, we initilize the window first
		window = new Window();
		if ( !window->OnCreate( engineName, windowWidth, windowHeight ) )
		{
			Debug::FatalError( "Failed to create window!", __FILE__, __LINE__ );
			return false;
		}

		

		Vulkan::VulkanRenderer::GetInstance()->OnCreate( "Prototype", engineName.c_str(), 1, true, window );


		break;

	default:

		break;

	}


	






	SetUpInput();

	if ( gameInterface )
	{
		if ( !gameInterface->OnCreate() )
		{
			Debug::FatalError( "Failed to load game!", __FILE__, __LINE__ );
			return false;
		}
	}
	else
	{
		Debug::Info( "Engine running without game.", __FILE__, __LINE__ );
	}

	Debug::Info( "All Core Engine Systems Have be successfully initialized!", __FILE__, __LINE__ );

	TitanForceEngine::SetFPS( fps );

	return isRunning = true;
}

bool TitanForceEngine::InitEngineRenderer( const Renderer& renderer )
{



}

void TitanForceEngine::Run()
{

	// BEGIN_PROFILE("Hello");

	while ( isRunning )
	{
		engineTimer->UpdateFrameTicks();
		Update( engineTimer->GetDeltaTime() );
		Render();
		Sleep( engineTimer->GetSleepTime( engineTimer->GetFPS() ) );
	}

	if ( !isRunning )
	{
		OnDestroy();
	}

	// END_PROFILE("GoodBye");
}

bool TitanForceEngine::IsRunning() const
{
	return isRunning;
}

bool TitanForceEngine::IsGameRunning() const
{
	return isGameRunning;
}

void TitanForceEngine::Exit()
{
	isRunning = false;
}

void TitanForceEngine::ExitGame()
{
	isGameRunning = false;
}

TitanForceEngine* TitanForceEngine::GetInstance()
{
	if ( engineInstance == nullptr )
	{
		engineInstance.reset( new TitanForceEngine );
		return engineInstance.get();
	}
	return engineInstance.get();
}

void TitanForceEngine::SetFPS( const unsigned int fps_ )
{
	engineTimer->SetFPS( fps_ );
}

void TitanForceEngine::SetGameInterface( GameInterface* game )
{
	gameInterface = game;
}

void TitanForceEngine::OnDestroy()
{

	if ( window )
	{
		window->OnDestroy();
		delete window;
		window = nullptr;
	}

	Vulkan::VulkanRenderer::GetInstance()->OnDestroy();

	if ( engineTimer )
	{
		delete engineTimer;
		engineTimer = nullptr;
	}

	if ( engineInstance != nullptr )
	{
		engineInstance = nullptr;
	}

	Debug::Info( "Engine Stopped Running Successfully", __FILE__, __LINE__ );
}

void TitanForceEngine::Update( const float deltaTime )
{
	if ( gameInterface )
		gameInterface->Update( deltaTime );

	HandleEvents();
}

void TitanForceEngine::Render()
{

	if ( gameInterface )
	{
		gameInterface->Render();
	}

	if ( Vulkan::VulkanRenderer::GetInstance()->IsRendering() )
	{
		Vulkan::VulkanRenderer::GetInstance()->Render();
		Vulkan::VulkanRenderer::GetInstance()->Wait();
	}
}

void TitanForceEngine::SetUpInput()
{
	glfwSetKeyCallback( window->GetWindow(), KeyCallback );
	glfwSetInputMode( window->GetWindow(), GLFW_STICKY_KEYS, 1 );
}

void TitanForceEngine::HandleEvents()
{
	if ( window )
		glfwPollEvents();

}

void KeyCallback( GLFWwindow *window, int key, int code, int action, int mods )
{
	std::cout << key << std::endl;
	if ( key == GLFW_KEY_ESCAPE )
	{
		switch ( action )
		{

		case GLFW_RELEASE:
			TitanForceEngine::GetInstance()->Exit();
			break;

		default:
			break;
		}
	}
}

int TitanForceEngine::GetCurrentSceneNum() const
{
	return currentSceneNum;
}

void TitanForceEngine::SetCurrentSceneNum( int sceneNum )
{
	currentSceneNum = sceneNum;
}
