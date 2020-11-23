#include "ShaderLinker.h"

#include "../../Graphics/Graphics.h"

#if GRAPHICS_API == GRAPHICS_OPENGL
#include <glad/glad.h>
#elif GRAPHICS_API == GRAPHICS_VULKAN

#endif

#include "../../Debug/Debug.h"

#include <vector>

ShaderLinker::ShaderLinker( const std::string& shaderName ) :
	m_name( shaderName ),
	m_id( 0 ),
	m_shaderChain()
{}

ShaderLinker::~ShaderLinker()
{
	for ( auto s : m_shaderChain )
	{
		if ( s )
		{
			delete s;
			s = nullptr;
		}
	}

	m_shaderChain.empty();

}

// Adds the passed shader to this linker, linking this shader to this program
void ShaderLinker::SubmitShader( Shader * shader )
{
	if ( shader == nullptr )
	{
		Debug::Error( "Failed to submit shader to shader linker: " + m_name, __FILE__, __LINE__ );
		return;
	}

	int type = static_cast<int>( shader->GetType() );
	if ( m_shaderChain[type] != nullptr )
		// In the case when the shader type already exists we need to delete it from the array before we assign the new one
	{
		Debug::Error( "Replacing Shader Type: " + std::to_string( type ) + "Old Shader Name: " + m_shaderChain[type]->GetFileName() + "New Shader: " + shader->GetFileName(), __FILE__, __LINE__ );
		delete m_shaderChain[type];
	}

	m_shaderChain[type] = shader;

}

// Return the uniform id for the passed uniform name from the shader inside of this linker with the passed shader type
unsigned int ShaderLinker::GetUniformId( const EShaderType & shaderType, const std::string& uniformName ) const
{
	int type = (int)shaderType;
	if ( m_shaderChain[type] == nullptr )
	{
		return 0;
	}

	return m_shaderChain[type]->GetUniformId( uniformName );

}

unsigned int ShaderLinker::GetShaderProgramId() const
{
	return m_id;
}


#if GRAPHICS_API == GRAPHICS_OPENGL
// Links/ Binds all shaders submitted to this shader linker
void ShaderLinker::LinkShaders()
{
	if ( m_id != 0 )
	{
		Debug::Info( m_name + ": Deleting old program:" + std::to_string( m_id ), __FILE__, __LINE__ );
		GLuint oldProgram = m_id;
		glDeleteProgram( oldProgram );
	}

	GLuint program = glCreateProgram();
	m_id = program;
	GLuint shader;
	GLint linkResult;

	for ( auto s : m_shaderChain )
	{
		if ( s == nullptr )
		{
			continue;
		}

		shader = s->GetShaderId();
		linkResult = 0;

		glAttachShader( program, shader );

		glLinkProgram( program );

		glGetProgramiv( program, GL_LINK_STATUS, &linkResult );

		if ( !linkResult )
		{
			GLint infoLogLength = 0;
			glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );
			std::vector<char> programLog( infoLogLength );
			glGetProgramInfoLog( program, infoLogLength, NULL, &programLog[0] );
			std::string programString( programLog.begin(), programLog.end() );
			Debug::Error( "Failed to link shader " + s->GetFileName() + ". Error: " + programString, __FILE__, __LINE__ );
			glDeleteShader( shader );
			glDeleteProgram( program );
			return;
		}

		glDeleteShader( shader );

	}

	// SetUpUniform Locations
	for ( auto s : m_shaderChain )
	{
		if ( s == nullptr )
		{
			continue;
		}

		s->SetUpUniformLocations(m_id);
	}

}

#elif GRAPHICS_API == GRAPHICS_VULKAN
// Links/ Binds all shaders submitted to this shader linker
void ShaderLinker::LinkShaders()
{}
#endif