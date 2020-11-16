#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <vector>

// A simple vertex layout
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 colour;
};

struct SubMesh
{
	std::vector<Vertex>			vertexList;
	std::vector<int>			meshIndices;
};

struct MVP_Matrix
{
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
};

class CameraComponent;

class Mesh
{

public:

	explicit Mesh( CameraComponent* camera, SubMesh subMesh, const char* shader ) :
		m_camera( camera ),
		m_subMesh( subMesh ),
		m_mvpMatrix( MVP_Matrix() ),
		m_shader( shader )
	{}

	virtual ~Mesh() {}

	virtual void Render( glm::mat4 transform ) = 0;

	const char* GetShader() const { return m_shader; }

protected:

	CameraComponent*	m_camera;
	SubMesh				m_subMesh;
	MVP_Matrix			m_mvpMatrix;
	const char*			m_shader;

	virtual void GenerateBuffers() = 0;

};



#endif // !MESH_H