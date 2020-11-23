#include "Texture.h"

Texture::Texture( const char* fileName ) :
	m_fileName( fileName ),
	m_id( 0 )
{}

Texture::~Texture()
{}

void Texture::Bind()
{}

void Texture::UnBind()
{}

void Texture::LoadTexture( const char * fileName )
{}
