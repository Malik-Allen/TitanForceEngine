#include "Buffer.h"

#include <chrono>

#include "../VulkanRenderer.h"

namespace Vulkan {

	Buffer::Buffer(uint64_t size_, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void* data) :
		buffer(VK_NULL_HANDLE),
		size(size_),
		bufferMemory(VK_NULL_HANDLE)
	{
		OnCreate(size_, usage, properties, data);
	}

	Buffer::~Buffer() {}


	void Buffer::OnCreate(uint64_t size_, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void* data) {
		
		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		size = size_;

		// The Staging buffer affords us to opportunity to load our vertex buffer onto the GPU for better performance

		// Creating staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creating Buffer that be used as a source in a memory transfer operation
		CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* stageData;
		vkMapMemory(device, stagingBufferMemory, 0, size, 0, &stageData);
		memcpy(stageData, data, (size_t)size);
		vkUnmapMemory(device, stagingBufferMemory);

		// Creating Buffer that be used as a destination in a memory transfer operation
		CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
			properties, buffer, bufferMemory);

		CopyBuffer(stagingBuffer, buffer, size);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

	}



	void Buffer::OnDestroy() {
	
		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);

	}


	void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;	// Indicates the purpose the buffer will be used for, use bit wise or for multiple purposes
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;		// Buffers can be owned by specific family queues

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);

	}

	uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		
		auto physicalDevice = Vulkan::VulkanRenderer::GetInstance()->GetPhysicalDevice()->GetVkPhysicalDevice();
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");

	}

	// Helper function to copy the contents of one buffer to another
	void Buffer::CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size) {
		
		auto commandPool = Vulkan::VulkanRenderer::GetInstance()->GetCommandPool()->GetVkCommandPool();
		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();
		auto graphicsQueue = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetGraphicsQueue();

		// Memory transfer operations are executed using command buffers, just lke drawing commands
			// Therefore we first allocate a temporary comand buffer

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;	// Command buffer used to perform memory transfer
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);	// IMPORTANT: Your'e not supposed to actually use an allocate command for every buffer, see Conclusion: https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	// Telling the driver about this command buffers intent, in this case one time use only to copy a buffer

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};	// Struct is used to manage information about copying buffer contents
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);	// Used to copy the contents of one buffer to another

		vkEndCommandBuffer(commandBuffer);	// We can stop recording at this step here

		// Unlike draw commands there are no events we need to wait on this time
			// We could use a VkWaitForFences or simply use a vkQueueWaitIdle, we choose the latter
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);	// Command buffer clean up
	}

	void Buffer::UpdateData(uint64_t size, void* data_) {
		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		void* data;
		vkMapMemory(device, bufferMemory, 0, size, 0, &data);
		memcpy(data, data_, size);
		vkUnmapMemory(device, bufferMemory);

	}

}