#include "VulkanMesh.h"

VulkanMesh::VulkanMesh( CameraComponent* camera, SubMesh subMesh, const char * shader ) :
	Mesh( camera, subMesh, shader )
{}

VulkanMesh::~VulkanMesh()
{}

void VulkanMesh::GenerateBuffers()
{}

void VulkanMesh::Render( glm::mat4 transform )
{}
