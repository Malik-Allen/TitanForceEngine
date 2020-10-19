#include "UniformBuffer.h"

#include "../Descriptors/DescriptorSet.h"

#include "../VulkanRenderer.h"



namespace Vulkan {

	UniformBuffer::UniformBuffer(uint64_t size, void* data) :
		Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data),
		descriptorSet(nullptr)
	{}


	UniformBuffer::~UniformBuffer() {}

	void UniformBuffer::UpdateDescriptorSet(VkDescriptorType type, VkPipelineBindPoint pipelineBindPoint) {

		auto descriptorPool = Vulkan::VulkanRenderer::GetInstance()->GetGraphicsDescriptorPool()->GetVkDescriptorPool();

		if (descriptorSet == nullptr)
			descriptorSet = new Vulkan::DescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, descriptorPool);
		else
			descriptorSet->OnCreate(VK_PIPELINE_BIND_POINT_GRAPHICS, descriptorPool);


		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		// Descriptors that refer to buffers, like our uniform buffer descriptor
		bufferInfo = {};
		bufferInfo.buffer = this->Buffer::buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = this->Buffer::size;

		// 
		descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = VK_NULL_HANDLE;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;

		descriptorWrite.descriptorType = type;
		descriptorWrite.descriptorCount = 1;

		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	}


}