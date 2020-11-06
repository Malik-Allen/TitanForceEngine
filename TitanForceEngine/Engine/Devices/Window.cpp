#include "Window.h"

#include "../Debug/Debug.h"

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
		Debug::FatalError( "Failed to init GLFW!", __FILE__, __LINE__ );
		return false;
	}

	m_width = width;
	m_height = height;

	SetPre_Attributes();

	m_glfwWindow = glfwCreateWindow( m_width, m_height, name.c_str(), nullptr, nullptr );

	SetPost_Attributes();

	// AssignCallbacks();


	if ( !m_glfwWindow )
	{
		Debug::FatalError( "Failed to create GLFW Window", __FILE__, __LINE__ );
		return false;
	}

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
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
}

void Window::SetPost_Attributes() {}




