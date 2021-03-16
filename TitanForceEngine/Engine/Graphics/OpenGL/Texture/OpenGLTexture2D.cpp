#include "OpenGLTexture2D.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/include/Utility/Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


OpenGLTexture2D::OpenGLTexture2D( const char* fileName ) :
	Texture2D( fileName ), m_id( 0 )
{
	GenerateTexture();
}

OpenGLTexture2D::~OpenGLTexture2D()
{}

void OpenGLTexture2D::GenerateTexture()
{

	if ( m_fileName == "" )
	{
		DEBUG_LOG( LOG::WARNING, "Failed to load texture: no file name provided" );
		CONSOLE_LOG( LOG::WARNING, "Failed to load texture: no file name provided" );
		return;
	}

	std::string filePath = "./Resources/Textures/" + m_fileName;

	DEBUG_LOG( LOG::INFO, "Generating texture... at file: " + filePath );
	CONSOLE_LOG( LOG::INFO, "Generating texture... at file: " + filePath );

	glGenTextures( 1, &m_id );
	glBindTexture( GL_TEXTURE_2D, m_id );

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	int nrChannels;
	unsigned char *data = stbi_load( filePath.c_str(), &m_width, &m_height, &nrChannels, 0 );

	if ( data )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );

		DEBUG_LOG( LOG::INFO, "Generating texture... COMPLETED: " + filePath );
		CONSOLE_LOG( LOG::INFO, "Generating texture... COMPLETED: " + filePath );
	}
	else
	{
		DEBUG_LOG( LOG::ERRORLOG, "Generating texture... FAILED: " + filePath );
		CONSOLE_LOG( LOG::ERRORLOG, "Generating texture... FAILED: " + filePath );
	}
	stbi_image_free( data );

}

void OpenGLTexture2D::Bind()
{
	glBindTexture( GL_TEXTURE_2D, m_id );
}

void OpenGLTexture2D::Unbind()
{

}
