#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <vulkan.h>
#include <vector>

namespace Vulkan {

	class CommandBuffer {

	public:

		CommandBuffer(const CommandBuffer&) = delete;
		CommandBuffer& operator=(const CommandBuffer&) = delete;
		CommandBuffer(CommandBuffer&&) = delete;
		CommandBuffer& operator = (CommandBuffer&&) = delete;

		CommandBuffer();
		~CommandBuffer();

		void OnCreate(const VkDevice& device_, const VkCommandPool& commandPool_, const VkRenderPass& renderPass_);
		void OnDestroy();

		const VkCommandBuffer& GetVkCommandBuffer() const { return commandBuffer; }

		void Begin(VkCommandBufferUsageFlags usage);
		void End();

		void BeginRenderPass(const VkRenderPassBeginInfo& renderPassBeginInfo);

		void EndRenderPass();

		void BindPipeline(const VkPipeline& pipeline);

		void BindVertexBuffers(const std::vector<class Buffer*>& vertexBuffers);

		void BindIndexBuffer(Buffer* indexBuffer);

	private:

		VkDevice device;
		VkCommandBuffer commandBuffer;
		VkCommandPool commandPool;
		VkRenderPass renderPass;

	};


}

#endif // !COMMANDBUFFER_H
