#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"

class Texture2D : public ITexture
{
public:

	explicit Texture2D( const char* fileName );
	virtual ~Texture2D() {}

	virtual void GenerateTexture() = 0;
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

protected:

	int m_width;
	int m_height;

};



#endif // !TEXTURE2D_H


