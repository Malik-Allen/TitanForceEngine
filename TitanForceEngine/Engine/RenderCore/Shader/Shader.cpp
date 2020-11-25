#include "Shader.h"

#include "../../Graphics/Graphics.h"

#if GARPHICS_API == GRAPHICS_OPENGL
#include <glad/glad.h>
#elif GARPHICS_API == GRAPHICS_VULKAN

#endif

#include "../../Debug/Debug.h"

#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader( const EShaderType & shaderType, const std::string& fileName ) :
	m_type( shaderType ),
	m_fileName( fileName )
{
	m_shaderSource = ReadShaderFromFile( m_fileName );
	m_id = CompileShader();
}

Shader::~Shader()
{}

// Returns the location id for this shader
unsigned int Shader::GetShaderId() const
{
	return m_id;
}

unsigned int Shader::GetUniformId( const std::string& uniformName )
{
	// TODO:
		// Make this faster than a map look up in this method
	if ( m_uniformMap.find( uniformName ) != m_uniformMap.end() )
	{
		return m_uniformMap[uniformName];
	}
	return 0;
}

const std::string & Shader::GetFileName() const
{
	return m_fileName;
}

const EShaderType & Shader::GetType() const
{
	return m_type;
}

std::string Shader::ReadShaderFromFile( const std::string& fileName )
{
	std::string filePath = "D://mklal//Documents//GitHub//TitanForceEngine//TitanForceEngine//Resources//Shaders//" + fileName;
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

#if GARPHICS_API == GRAPHICS_OPENGL

unsigned int Shader::CompileShader()
{

	GLenum shaderType;

	switch ( m_type )
	{
	case EShaderType::Vertex:
		shaderType = GL_VERTEX_SHADER;
		break;
	case EShaderType::Tessellation:
		shaderType = GL_TESS_CONTROL_SHADER;
		break;
	case EShaderType::Geometry:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case EShaderType::Fragment:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	default:
		Debug::Error( "Invalid ShaderType Passed!", __FILE__, __LINE__ );
		return 0;
		break;
	}

	GLint compileResult = 0;
	GLuint shader = glCreateShader( shaderType );
	const char* shaderCodePtr = m_shaderSource.c_str();
	const int shaderCodeSize = m_shaderSource.size();

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
		Debug::Error( "Error Compiling shader " + m_fileName + ". Error: \n" + shaderString, __FILE__, __LINE__ );
		return 0;
	}

	return shader;
}

void Shader::SetUpUniformLocations( unsigned int programId )
{
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char *name;
	int maxUniformListLength;
	unsigned int loc;

	glGetProgramiv( programId, GL_ACTIVE_UNIFORMS, &count );
	printf( "There are %d active Uniforms\n", count );

	/// get the length of the longest named uniform 
	glGetProgramiv( programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength );

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char *)malloc( sizeof( char ) * maxUniformListLength );


	for ( int i = 0; i < count; ++i )
	{
		/// Get the name of the ith uniform
		glGetActiveUniform( programId, i, maxUniformListLength, &actualLen, &size, &type, name );
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation( programId, name );
		std::string uniformName = name;
		m_uniformMap[uniformName] = loc;

		printf( "\"%s\" loc:%d\n", uniformName.c_str(), m_uniformMap[uniformName] );
	}
	free( name );
}

#elif GARPHICS_API == GRAPHICS_VULKAN

unsigned int Shader::CompileShader()
{
	return 0;
}

void Shader::SetUpUniformLocations( unsigned int programId )
{}

#endif


