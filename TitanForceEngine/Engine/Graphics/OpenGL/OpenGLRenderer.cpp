#include "OpenGLRenderer.h"

#include "../../Debug/Debug.h"
#include "../../Devices/Window.h"
#include "3D/OpenGLMesh.h"

#include <vector>
#include <sstream>

OpenGLRenderer::OpenGLRenderer() :
	m_window( nullptr )
{}

OpenGLRenderer::~OpenGLRenderer()
{}

void OpenGLRenderer::OnCreate(
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
		return;
	}

	int major, minor;
	GetInstalledOpenGLInfo( &major, &minor );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, minor );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_MULTISAMPLE ); // enables multisampling

	glViewport( 0, 0, m_window->GetWidth(), m_window->GetHeight() );

	CreateShaderProgram( "PhongShader", "./Engine/Shaders/PhongVertex.glsl", "./Engine/Shaders/PhongFragment.glsl" );
}

void OpenGLRenderer::OnDestroy()
{
	if ( m_shaderPrograms.size() > 0 )
	{
		for ( auto p : m_shaderPrograms )
		{
			glDeleteProgram( p.second );

		}
	}
	m_shaderPrograms.clear();
}

void OpenGLRenderer::Render()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glUseProgram( m_shaderPrograms["PhongShader"] );

	// TODO: Have the meshes added store to an a map where the GLuint (shader program is the key and a vector of meshes with that shader as the data)
}

void OpenGLRenderer::Wait()
{
	glfwSwapBuffers( glfwGetCurrentContext() );
	glUseProgram( 0 );
}

void OpenGLRenderer::AddMesh( Mesh * mesh )
{
	if ( mesh == nullptr )
	{
		return;
	}

	OpenGLMesh* glMesh = dynamic_cast<OpenGLMesh*>( mesh );

	glMesh->shaderProgram = GetShaderProgram( glMesh->GetShader() );
	glMesh->GenerateBuffers();

}

void OpenGLRenderer::CreateShaderProgram(
	const std::string & shaderName,
	const std::string & vertexShaderFileName,
	const std::string & fragmentShaderFileName )
{
	std::string vertexShaderCode = ReadShader( vertexShaderFileName );
	std::string fragmentShaderCode = ReadShader( fragmentShaderFileName );

	if ( vertexShaderCode == "" || fragmentShaderCode == "" )
	{
		return;
	}

	GLuint vertexShader = CreateShader( GL_VERTEX_SHADER, vertexShaderCode, shaderName );
	GLuint fragmentShader = CreateShader( GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName );

	if ( vertexShader == 0 || fragmentShader == 0 )
	{
		return;
	}

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );

	glLinkProgram( program );

	glGetProgramiv( program, GL_LINK_STATUS, &linkResult );

	if ( !linkResult )
	{
		GLint infoLogLength = 0;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );
		std::vector<char> programLog( infoLogLength );
		glGetProgramInfoLog( program, infoLogLength, NULL, &programLog[0] );
		std::string programString( programLog.begin(), programLog.end() );
		Debug::Error( "Failed to link shader " + shaderName + ". Error: " + programString, __FILE__, __LINE__ );
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		glDeleteProgram( program );
		return;
	}

	m_shaderPrograms[shaderName] = program;
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );
}

GLuint OpenGLRenderer::GetShaderProgram( const std::string & shaderName )
{
	if ( m_shaderPrograms.find( shaderName ) != m_shaderPrograms.end() )
	{
		return m_shaderPrograms[shaderName];
	}
	return 0;
}

std::string OpenGLRenderer::ReadShader( const std::string & filePath )
{
	std::string shaderCode = "";
	std::ifstream file;
	file.exceptions( std::ifstream::badbit );
	try
	{
		file.open( filePath );
		std::stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();
		shaderCode = tmpStream.str();
	}
	catch ( std::ifstream::failure error_ )
	{
		Debug::Error( "Could not read the shader: " + filePath, __FILE__, __LINE__ );
		return "";
	}

	return shaderCode;
}

GLuint OpenGLRenderer::CreateShader(
	GLenum shaderType,
	const std::string & source,
	const std::string & shaderName )
{
	GLint compileResult = 0;
	GLuint shader = glCreateShader( shaderType );
	const char* shaderCodePtr = source.c_str();
	const int shaderCodeSize = source.size();

	glShaderSource( shader, 1, &shaderCodePtr, &shaderCodeSize );
	glCompileShader( shader );

	
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileResult );

	if ( !compileResult )
	{
		GLint infoLogLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLogLength );
		std::vector<char> shaderLog( infoLogLength );
		glGetShaderInfoLog( shader, infoLogLength, NULL, &shaderLog[0] );
		std::string shaderString( shaderLog.begin(), shaderLog.end() );
		Debug::Error( "Error Compiling shader " + shaderName + ". Error: \n" + shaderString, __FILE__, __LINE__ );
		return 0;
	}

	return shader;
}

void OpenGLRenderer::GetInstalledOpenGLInfo( int * major, int * minor )
{/// You can to get some info regarding versions and manufacturer
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
