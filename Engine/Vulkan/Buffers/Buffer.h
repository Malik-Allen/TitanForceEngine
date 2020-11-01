#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan.h>

namespace Vulkan {

	class Buffer {

	public:

		Buffer(uint64_t size_, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void* data = nullptr);
		virtual ~Buffer();

		
		void OnDestroy();

		const VkBuffer& GetBuffer() const { return buffer; }
		VkDeviceSize GetSize() const { return size; }
		const VkDeviceMemory& GetBufferMemory() const { return bufferMemory; }

		void UpdateData(uint64_t size, void* data);

		void OnCreate(uint64_t size_, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void* data = nullptr);
	protected:


		VkBuffer buffer;
		VkDeviceSize size;
		VkDeviceMemory bufferMemory;

		// Helper Function to create buffers
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size);

	};


}


#endif // !BUFFER_H
