#include "CommandBuffer.h"

#include <iostream>

#include "../Buffers/Buffer.h"

namespace Vulkan {

	CommandBuffer::CommandBuffer() :
		device(VK_NULL_HANDLE),
		commandBuffer(VK_NULL_HANDLE),
		commandPool(VK_NULL_HANDLE),
		renderPass(VK_NULL_HANDLE)
	{}


	CommandBuffer::~CommandBuffer() {}


	// // Allocates memory for this commmend buffer
	void CommandBuffer::OnCreate(const VkDevice& device_, const VkCommandPool& commandPool_, const VkRenderPass& renderPass_) {

		device = device_;
		commandPool = commandPool_;
		renderPass = renderPass_;

		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null device passed to command buffer!");
		}

		if (commandPool == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null command pool passed to command buffer!");
		}

		if (renderPass == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null render pass passed to command buffer!");
		}

		VkCommandBufferAllocateInfo bufferAllocInfo = {};
		bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		bufferAllocInfo.commandPool = commandPool;
		bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		bufferAllocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device, &bufferAllocInfo, &commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffer memory!");
		}



	}


	void CommandBuffer::OnDestroy() {}


	void CommandBuffer::Begin(VkCommandBufferUsageFlags usage) {

		VkCommandBufferBeginInfo commandBuffBeginInfo = {};
		commandBuffBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//commandBuffBeginInfo.flags = usage;	//Optional
		//commandBuffBeginInfo.pInheritanceInfo = nullptr;	//Optional

		if (vkBeginCommandBuffer(commandBuffer, &commandBuffBeginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin command buffer!");
		}

	}


	void CommandBuffer::End() {

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}

	}

	void CommandBuffer::BeginRenderPass(const VkRenderPassBeginInfo& renderPassBeginInfo) {

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	}

	void CommandBuffer::EndRenderPass() {

		vkCmdEndRenderPass(commandBuffer);

	}

	void CommandBuffer::BindPipeline(const VkPipeline& pipeline) {

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	}

	void CommandBuffer::BindVertexBuffers(const std::vector<Buffer*>& vertexBuffers) {

		std::vector<VkBuffer> buffers;
		buffers.resize(vertexBuffers.size());

		for (auto buffer : vertexBuffers) {
			buffers.push_back(buffer->GetBuffer());
		}

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets);

	}

	void CommandBuffer::BindIndexBuffer(Buffer* indexBuffer) {

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

	}

	

}