#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:

	Texture( const char* fileName );
	~Texture();

	void Bind();
	void UnBind();

	unsigned int GetId() const { return m_id; }

private:

	const char*			m_fileName;
	unsigned int		m_id;

	void LoadTexture( const char* fileName );

};

#endif // !TEXTURE_H

