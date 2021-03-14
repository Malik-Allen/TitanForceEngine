#include "Window.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/include/Utility/Debug.h"




Window::Window() :
	m_glfwWindow( nullptr ),
	m_width( 1280 ),
	m_height( 720 )
{}

Window::~Window() {}

bool Window::OnCreate( const std::string& name, const int width, const int height )
{

	if ( !glfwInit() )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to init GLFW!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to init GLFW!" );
		return false;
	}

	m_width = width;
	m_height = height;

	SetPre_Attributes();

	m_glfwWindow = glfwCreateWindow( m_width, m_height, name.c_str(), nullptr, nullptr );

	if ( !m_glfwWindow )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create GLFW window!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to create GLFW window!" );
		return false;
	}

	glfwMakeContextCurrent( m_glfwWindow );

	m_glfwWindow = glfwGetCurrentContext();

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to init GLAD!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to init GLAD!" );
		return false;
	}

	SetPost_Attributes();

	// AssignCallbacks();


	

	// **----------
		// This will be potentially where I introduce a level of compiler directives which will choose the graphics library, and properly instantiate them
			// In this case we are using a vulkan instance for the graphics api so we must initialize it

	// glfwSetWindowUserPointer(window, vulkanInstance);

	return true;
}

void Window::OnDestroy()
{
	glfwDestroyWindow( m_glfwWindow );
	glfwTerminate();
}




void Window::SetPre_Attributes()
{
	// Window hints can be use before the window has been created to give the window properties and functionality. 
	// Once a window has been created a new window hint will not affect it/ cannot be applied after the fact
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	// glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );
	
	
}

void Window::SetPost_Attributes() {
	
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
	glfwWindowHint( GLFW_DEPTH_BITS, 64 );
}




