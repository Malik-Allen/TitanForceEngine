#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include "../../Core/Renderer.h"

class VulkanRenderer : public Renderer
{

public:

	VulkanRenderer();
	~VulkanRenderer();

	virtual void OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) override final;

	virtual void OnDestroy() override final;

	virtual void Render() override final;
	virtual void Wait() override final;

	virtual void AddMesh( Mesh* mesh ) override final;

};


#endif // !VULKANRENDERER_H
