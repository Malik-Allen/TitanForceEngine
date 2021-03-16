#ifndef OPENGL_TEXTURE_2D_H
#define OPENGL_TEXTURE_2D_H

#include "../../../RenderCore/Texture/Texture2D.h"

#include <glad/glad.h>


class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D( const char* fileName );
	~OpenGLTexture2D();

	virtual void GenerateTexture() override;
	virtual void Bind() override;
	virtual void Unbind() override;

private:

	GLuint m_id;

};


#endif // !OPENGL_TEXTURE_2D_H




