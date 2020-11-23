#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include "../../../RenderCore/3D/Mesh.h"

#include <glad/glad.h>

class OpenGLMesh : public IMesh
{
	friend class OpenGLRenderer;

public:

	OpenGLMesh( const char* objFileName );
	~OpenGLMesh();

	virtual void GenerateBuffers() override final;

	virtual void Render() override final;

private:

	GLuint VAO, VBO;

};


#endif // !OPENGLMESH_H

