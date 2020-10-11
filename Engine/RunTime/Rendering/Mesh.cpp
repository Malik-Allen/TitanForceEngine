#include "Mesh.h"

Mesh::Mesh(SubMesh subMesh_) :
	subMesh(subMesh_)
{}

Mesh::~Mesh() {}

void Mesh::Render() {}

void Mesh::GenerateBuffers() {
	
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(subMesh.vertexList[0]) * subMesh.vertexList.size();	// Calculating buffer size
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;	// Indicates the purpose the buffer will be used for, use bit wise or for multiple purposes
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;		// Buffers can be owned by specific family queues
	bufferInfo.flags = 0;

	

}