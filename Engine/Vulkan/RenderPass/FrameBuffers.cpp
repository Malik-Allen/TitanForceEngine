#include "FrameBuffers.h"

#include "SwapChain.h"

namespace Vulkan {


	FrameBuffers::FrameBuffers(const VkDevice& device_, SwapChain* swapChain, const VkRenderPass& renderPass) :
		device(device_)
	{
		OnCreate(device_, swapChain, renderPass);
	}


	FrameBuffers::~FrameBuffers() {}
	
	// TODO:
	// Implement multiple attachments, vector of attachments
	void FrameBuffers::OnCreate(const VkDevice& device_, SwapChain* swapChain, const VkRenderPass& renderPass) {

		if (swapChain == nullptr) {
			throw std::runtime_error("Error! Null swap chain passed to frame buffers!");
		}

		if (renderPass == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null renderpass passed to frame buffers!");
		}

		device = device_;
		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null device passed to frame buffers!");
		}

		// The image that we have to use for the attachment depends on which image the swap chain returns when we retrieve one for presentation. 
		// That means that we have to create a framebuffer for all of the images in the swap chain and use the one that corresponds to the retrieved image at drawing time.

		swapChainFramebuffers.resize(swapChain->swapChainImageViews.size());

		// We are going to create framebuffers for all the image views
		for (size_t i = 0; i < swapChain->swapChainImageViews.size(); i++) {
			VkImageView attachments[] = { swapChain->swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;	// One for now, more attachments, we will just use the size of the passed vector of attachments
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChain->swapChainExtent.width;
			framebufferInfo.height = swapChain->swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create framebuffer for swapchain image views");
			}
		}
	
	}


	void FrameBuffers::OnDestroy() {
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}
	
}