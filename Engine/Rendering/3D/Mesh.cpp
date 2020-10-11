#include "Mesh.h"

#include "../../Vulkan/Buffers/Buffer.h"
#include "../../Vulkan/Graphics.h"

Mesh::Mesh(SubMesh subMesh_) :
	subMesh(subMesh_)
{}

Mesh::~Mesh() {}

void Mesh::Render() {}

void Mesh::GenerateBuffers() {
	
	// Creating the vertex buffer

	uint64_t verticesSize = sizeof(Vertex) * subMesh.vertexList.size();

	Vulkan::Buffer* vertexBuffer = new Vulkan::Buffer(verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subMesh.vertexList.data());


	// Creating the index buffer
	uint64_t indicesSize = sizeof(int) * subMesh.meshIndices.size();

	Vulkan::Buffer* indexBuffer = new Vulkan::Buffer(indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subMesh.meshIndices.data());

	

}