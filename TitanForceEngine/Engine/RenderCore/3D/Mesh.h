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

class IMesh
{

public:

	explicit IMesh( const char* objFileName );

	virtual ~IMesh();

	virtual void Render() = 0;

protected:

	SubMesh*		m_subMesh;

	virtual void GenerateBuffers() = 0;

};



#endif // !MESH_H