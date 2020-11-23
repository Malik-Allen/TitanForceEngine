#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include "../../RenderCore/Renderer.h"

class VulkanRenderer : public IRenderer
{

public:

	VulkanRenderer();
	~VulkanRenderer();

	// Initializes Renderer
	virtual bool OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) override final;
	virtual void OnDestroy() override final;

	virtual void RenderScene( IScene* scene ) override final;

private:

	virtual void BeginScene( IScene* scene ) override final;
	virtual void EndScene() override final;

	virtual void Begin() override final;
	virtual void Present() override final;
	virtual void End() override final;

	virtual void SubmitModel( Model* model ) override final;

};


#endif // !VULKANRENDERER_H
