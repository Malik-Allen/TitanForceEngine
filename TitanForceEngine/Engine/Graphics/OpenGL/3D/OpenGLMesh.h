#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include "../../../3D/Mesh.h"

#include <glew.h>

class OpenGLMesh : public Mesh
{
	friend class OpenGLRenderer;

public:

	OpenGLMesh( CameraComponent* camera, SubMesh subMesh, const char* shader );
	~OpenGLMesh();

	virtual void GenerateBuffers() override final;

	virtual void Render( glm::mat4 transform ) override final;

private:

	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc;

	GLuint viewPositionLoc;
	GLuint lightPositionLoc;
	GLuint ambientLoc;
	GLuint diffuseLoc;
	GLuint colorLoc;

};


#endif // !OPENGLMESH_H

