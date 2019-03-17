#include "SkyBox.h"



SkyBox::SkyBox(): shaderPtr(nullptr), meshPtr(nullptr), texture(nullptr){}


SkyBox::~SkyBox()
{
}

bool SkyBox::OnCreate() {
	shaderPtr = new Shader("skyboxVert.glsl", "skyboxFrag.glsl");									// Creating a reference to skybox shader using the Vertex and Frags
	if (!ObjLoader::loadOBJ("cube.obj")) {															// Loading a cube object
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	if (!shaderPtr)
		return false;
}

unsigned int SkyBox::LoadSkyBox(const char *posX, const char *posY, const char *posZ, const char *negX, const char *negY, const char *negZ)
{
	glGenTextures(1, &cubeMapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	
	// Process of taking each texture and binding it to each side of the cube/ skybox
	texture = IMG_Load(posX);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);

	texture = IMG_Load(posY);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);

	texture = IMG_Load(posZ);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);

	texture = IMG_Load(negX);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);
	
	texture = IMG_Load(negY);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);

	texture = IMG_Load(negZ);
	mode = (texture->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	SDL_FreeSurface(texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return cubeMapTextureID;
}

void SkyBox::Render()const {
	glUseProgram(shaderPtr->getProgram());
	glUniformMatrix4fv(shaderPtr->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

	meshPtr->Render();
}
