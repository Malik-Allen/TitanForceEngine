#ifndef SKYBOX_H
#define SKYBOX_H

#include "Shader.h"
#include "Mesh.h"
#include "Matrix.h"
#include "ObjLoader.h"
#include <SDL_image.h>
#include "Texture.h"

class SkyBox
{
public:

	class Shader *shaderPtr;
	class Mesh *meshPtr;

	Matrix4 modelMatrix;

	GLuint cubeMapTextureID;
	SDL_Surface *texture;
	int mode;

	SkyBox();
	~SkyBox();

	bool OnCreate();
	unsigned int LoadSkyBox(const char *posX, const char *posY, const char * posZ, const char *negX, const char *negY, const char *negZ);

	void Render() const;
};

#endif

