#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vulkan.h>
#include <vector>

namespace Vulkan {

	// The attchments speified in renderpass creation are bound by wrapping them into a VkFrameBuffer object
	// A frame buffer object references all of he VkImageView objects that represent the attachments
	class FrameBuffers {

	public:

		FrameBuffers(const FrameBuffers&) = delete;
		FrameBuffers& operator=(const FrameBuffers&) = delete;
		FrameBuffers(FrameBuffers&&) = delete;
		FrameBuffers& operator = (FrameBuffers&&) = delete;

		FrameBuffers(const VkDevice& device_, class SwapChain* swapChain, const VkRenderPass& renderPass);
		~FrameBuffers();
		
		void OnCreate(const VkDevice& device, SwapChain* swapChain, const VkRenderPass& renderPass);
		void OnDestroy();

		const std::vector<VkFramebuffer>& GetVkFrameBuffers() const { return swapChainFramebuffers; }

	private:


		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkDevice device;
		
	};

}


#endif // !FRAMEBUFFER_H
