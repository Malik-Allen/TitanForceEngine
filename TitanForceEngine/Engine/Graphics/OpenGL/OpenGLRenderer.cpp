#include "OpenGLRenderer.h"

#include "../../Devices/Window.h"
#include "../../RenderCore/Model/Model.h"
#include "../../Components/RenderComponent.h"
#include "../../RenderCore/Camera/Camera.h"

#include "../../Debug/Debug.h"

#include <string>

OpenGLRenderer::OpenGLRenderer() : 
	IRenderer()
{}

OpenGLRenderer::~OpenGLRenderer()
{}

bool OpenGLRenderer::OnCreate(
	const char * applicationName,
	const char * engineName,
	int version,
	bool enableValidationLayers,
	Window * window )
{
	m_window = window;

	if ( !gladLoadGL() )
	{
		Debug::Error( "Failed Init GL with Glad", __FILE__, __LINE__ );
		return false;
	}

	int major, minor;
	GetInstalledOpenGLInfo( &major, &minor );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, minor );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_MULTISAMPLE ); // enables multisampling

	glViewport( 0, 0, m_window->GetWidth(), m_window->GetHeight() );

	return true;
}

void OpenGLRenderer::OnDestroy()
{}

void OpenGLRenderer::RenderScene( IScene * scene )
{
	BeginScene( scene );

	Begin();
	Present();
	End();

	EndScene();
}

void OpenGLRenderer::BeginScene( IScene * scene )
{
	
	if ( scene == nullptr )
	{
		return;
	}

	// TEMP
		// What I will be doing in the future is clearing out command queue, instead of vector of meshes

	m_models.clear();

	for ( auto m : scene->m_world->GetSystem<RenderSystem>()->GetModels() )
	{
		SubmitModel( m );
	}

	m_camera = scene->m_world->GetSystem<CameraSystem>()->GetCameras().front();
	// m_camera = m_scene->GetAllComponent<CameraComponent>().front();

	// std::vector<RenderComponent*> renderComponents = m_scene->m_world->GetAllComponent<RenderComponent>();
	// for ( auto r : renderComponents )
	// {
	// 	SubmitModel( r->m_model );
	// }

}

void OpenGLRenderer::EndScene()
{}

void OpenGLRenderer::Begin()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
}

void OpenGLRenderer::Present()
{
	for ( auto& m : m_models )
	{
		if ( m )
		{
			m->Render( m_camera );
		}
	}
}

void OpenGLRenderer::End()
{
	glfwSwapBuffers( glfwGetCurrentContext() );
	glUseProgram( 0 );
}

void OpenGLRenderer::SubmitModel( Model* model )
{
	m_models.push_back( model );
}

void OpenGLRenderer::GetInstalledOpenGLInfo( int * major, int * minor )
{
	// You can to get some info regarding versions and manufacturer
	const GLubyte *version = glGetString( GL_VERSION );
	/// You can also get the version as ints	
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );


	glGetIntegerv( GL_MAJOR_VERSION, major );
	glGetIntegerv( GL_MINOR_VERSION, minor );
	Debug::Info( "OpenGL version: " + std::string( (char*)glGetString( GL_VERSION ) ), __FILE__, __LINE__ );
	Debug::Info( "Graphics card vendor " + std::string( (char*)vendor ), __FILE__, __LINE__ );
	Debug::Info( "Graphics card name " + std::string( (char*)renderer ), __FILE__, __LINE__ );
	Debug::Info( "GLSL Version " + std::string( (char*)glslVersion ), __FILE__, __LINE__ );
	return;
}
