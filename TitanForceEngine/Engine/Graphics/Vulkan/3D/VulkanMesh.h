#ifndef VULKANMESH_H
#define VULKANMESH_H

#include "../../../RenderCore/3D/Mesh.h"

class VulkanMesh : public IMesh
{

public:
	
	VulkanMesh(const char* objFileName );
	~VulkanMesh();

	virtual void GenerateBuffers() override final;

	virtual void Render() override final;
};

#endif // !VULKANMESH_H
