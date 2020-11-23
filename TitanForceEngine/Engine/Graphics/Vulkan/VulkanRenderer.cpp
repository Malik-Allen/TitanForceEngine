#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{}

VulkanRenderer::~VulkanRenderer()
{}

bool VulkanRenderer::OnCreate(
	const char * applicationName,
	const char * engineName,
	int version,
	bool enableValidationLayers,
	Window * window )
{
	return false;
}

void VulkanRenderer::OnDestroy()
{}

void VulkanRenderer::RenderScene( IScene * scene )
{}

void VulkanRenderer::BeginScene( IScene * scene )
{}

void VulkanRenderer::EndScene()
{}

void VulkanRenderer::Begin()
{}

void VulkanRenderer::Present()
{}

void VulkanRenderer::End()
{}

void VulkanRenderer::SubmitModel( Model* model )
{}
