#include "RenderPass.h"

#include <iostream>


namespace Vulkan {

	RenderPass::RenderPass(const VkDevice& device_, const VkFormat& imageFormat) :
		device(device_),
		renderPass(VK_NULL_HANDLE)
	{
		OnCreate(device_, imageFormat);
	}	


	RenderPass::~RenderPass() {}

	
	void RenderPass::OnCreate(const VkDevice& device_, const VkFormat& imageFormat) {
		
		device = device_;
		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Null device passed to renderpass!");
		}

		// For now our singe color attachment is fine
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Subpaasses are subsequent rendering operations that depend on the contents of the framebuffer in previous passes
		// In other words this is where you will implement multiple rendering operations i.e. post-proceessing effects
		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


		// Each Renderpass can have multiple subpasses
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &subpassDependency;


		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create render pass!");
		}

	}


	void RenderPass::OnDestroy() {
		vkDestroyRenderPass(device, renderPass, nullptr);
	}


}