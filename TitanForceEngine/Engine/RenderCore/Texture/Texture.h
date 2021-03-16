#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class ITexture
{
public:

	ITexture( const char* fileName ) : m_fileName( fileName ) {}
	virtual ~ITexture() {}

	virtual void GenerateTexture() {};
	virtual void Bind() {};
	virtual void Unbind() {};

protected:

	std::string m_fileName;


};

#endif // !TEXTURE_H

