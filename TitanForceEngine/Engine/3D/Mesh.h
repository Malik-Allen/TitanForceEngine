#ifndef MESH_H
#define MESH_H

#include "../../EntityComponentSystem/ECS/ECS.h"

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


class MeshComponent : public ECS::Component
{

public:

	static constexpr uint64_t ID = GENERATE_ID( "MeshComponent" );
	MeshComponent();
	MeshComponent(SubMesh subMesh);
	~MeshComponent();

private:

	SubMesh		m_subMesh;
	UBO_MVP		m_uboMVP;

};



#endif // !MESH_H