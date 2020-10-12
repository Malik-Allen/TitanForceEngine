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

		void Begin(VkCommandBufferUsageFlags usage) const;
		void End() const;

		void BeginRenderPass(const VkRenderPassBeginInfo& renderPassBeginInfo) const;

		void EndRenderPass() const;

		void BindPipeline(const VkPipeline& pipeline) const;

		void BindVertexBuffers(const std::vector<class Buffer*>& vertexBuffers) const;

		void BindIndexBuffer(Buffer* indexBuffer) const;

		void Draw(uint32_t vertexCount) const;
		void DrawIndex(uint32_t indexCount) const;

	private:

		VkDevice device;
		VkCommandBuffer commandBuffer;
		VkCommandPool commandPool;
		VkRenderPass renderPass;

	};


}

#endif // !COMMANDBUFFER_H
