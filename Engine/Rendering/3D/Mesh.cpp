#include "Mesh.h"

#include "../../Vulkan/VulkanRenderer.h"

Mesh::Mesh(SubMesh subMesh_) :
	subMesh(subMesh_),
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{}

Mesh::~Mesh() {}

void Mesh::Render() {

}

void Mesh::GenerateBuffers() {
	
	// Creating the vertex buffer

	uint64_t verticesSize = sizeof(Vertex) * subMesh.vertexList.size();

	vertexBuffer = new Vulkan::Buffer(verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subMesh.vertexList.data());


	// Creating the index buffer
	uint64_t indicesSize = sizeof(int) * subMesh.meshIndices.size();

	indexBuffer = new Vulkan::Buffer(indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, subMesh.meshIndices.data());

	// Binding vertex and index buffers
	std::vector<Vulkan::Buffer*> vertexBuffers = { vertexBuffer };

	Vulkan::VulkanRenderer::GetInstance()->BindVertexBuffers(vertexBuffers);
	Vulkan::VulkanRenderer::GetInstance()->BindIndexBuffer(indexBuffer);


	// Creating Uniform Buffer
	uint64_t mvpUniformSize = sizeof(MVPUniform);
	mvpUniformBuffer = new Vulkan::Buffer(mvpUniformSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mvpUniform);



}