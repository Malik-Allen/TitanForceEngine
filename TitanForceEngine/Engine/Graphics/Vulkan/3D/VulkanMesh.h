#ifndef VULKANMESH_H
#define VULKANMESH_H

#include "../../../3D/Mesh.h"

class VulkanMesh : public Mesh
{

public:
	
	VulkanMesh( CameraComponent* camera, SubMesh subMesh, const char* shader );
	~VulkanMesh();

	virtual void GenerateBuffers() override final;

	virtual void Render( glm::mat4 transform ) override final;
};

#endif // !VULKANMESH_H
