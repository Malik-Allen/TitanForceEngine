#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vulkan.h>


namespace Vulkan {

	// Tells Vulkan about the frame buffer attachments that will be used while rendering
	// Subpasses can be attached and used for post-processing effects
	class RenderPass {

	public:
		RenderPass(const RenderPass&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = delete;
		RenderPass& operator = (RenderPass&&) = delete;

		RenderPass(const VkDevice& device_, const VkFormat& imageFormat);
		~RenderPass();
		
		// TODO: Implement Subpass Class and more paramters
		// Creates RenderPass
		void OnCreate(const VkDevice& device_, const VkFormat& imageFormat);
		void OnDestroy();

		VkRenderPass GetRenderPass() const { return renderPass; }
		
	private:

		

		VkDevice device;
		VkRenderPass renderPass;


	};


}

#endif // !RENDERPASS_H
