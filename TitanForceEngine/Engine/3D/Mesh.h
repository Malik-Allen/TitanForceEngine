#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <vector>

// A simple vertex layout
struct Vertex
{
	glm::vec3 position;
	glm::vec3 colour;
};


struct UBO_MVP
{
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
};

struct SubMesh
{
	std::vector<Vertex>			vertexList;
	std::vector<int>			meshIndices;
};


class Mesh
{

public:

	explicit Mesh(SubMesh subMesh);
	~Mesh();


	virtual void GenerateBuffers() = 0;

protected:

	SubMesh		m_subMesh;
	UBO_MVP		m_uboMVP;

};



#endif // !MESH_H