#include "App.h"

#include "../Debug/Debug.h"

#if GRAPHICS_API == GRAPHICS_OPENGL
#include "../Graphics/OpenGL/OpenGLRenderer.h"
#elif GRAPHICS_API == GRAPHICS_VULKAN
#include "../Graphics/Vulkan/VulkanRenderer.h"
#endif

IApp::IApp( const std::string& appName ) :
	m_appName( appName ),
	m_scene( nullptr ),
	m_renderer( nullptr )
{}

IApp::~IApp()
{
	if ( m_renderer )
	{
		delete m_renderer;
		m_renderer = nullptr;
	}

	if ( m_scene )
	{
		delete m_scene;
		m_scene = nullptr;
	}
}

const std::string& IApp::GetAppName() const
{
	return m_appName;
}

// Creates the render using the graphicsAPI of this application
bool IApp::CreateRenderer(
	const char* engineName,
	int version,
	bool enableValidationLayers,
	Window* window )
{

#if GRAPHICS_API == GRAPHICS_OPENGL

	m_renderer = new OpenGLRenderer();
	Debug::Info( "Creating OpenGL Application: " + m_appName, __FILE__, __LINE__ );

#elif GRAPHICS_API == GRAPHICS_VULKAN

	m_renderer = new VulkanRenderer();
	Debug::Info( "Creating Vulkan Application: " + *m_appName, __FILE__, __LINE__ );

#endif
	
	if ( m_renderer == nullptr )
	{
		Debug::Error( "Failed to create renderer for app: " + m_appName, __FILE__, __LINE__ );
		return false;
	}
	else
	{
		return m_renderer->OnCreate( m_appName.c_str(), engineName, version, enableValidationLayers, window );
	}

}

