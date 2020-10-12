#include "DescriptorSet.h"

#include "../Graphics.h"
#include "../Commands/CommandBuffer.h"

namespace Vulkan {

	DescriptorSet::DescriptorSet(VkPipelineBindPoint bindPoint) :
		descriptorSet(VK_NULL_HANDLE),
		descriptorPool(VK_NULL_HANDLE),
		pipelineLayout(VK_NULL_HANDLE),
		pipelineBindPoint(bindPoint)
	{}


	DescriptorSet::~DescriptorSet() {}


	void DescriptorSet::OnCreate() {

		auto device = Vulkan::Graphics::GetInstance()->GetLogicalDevice()->GetVkDevice();
		
		// First, describe which descriptor types our descriptor sets are going to contain and how many of them
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		// We will allocate one of these descriptors per frame
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;

		poolInfo.maxSets = 1;	// Max number of descriptors that can be allocated

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	



		// Specifying the descriptor pool to allocate from

		std::vector<VkDescriptorSetLayout> layouts(1, Vulkan::Graphics::GetInstance()->GetGraphicsPipeline()->GetDescriptorSetLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts.data();

		// No need to free up descriptor sets, they are automatically freed when the descriptor pool is destroyed
		
		if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < swapChainImages.size(); i++) {
			// Descriptors that refer to buffers, like our uniform buffer descriptor
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			// 
			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

		}
	
	}


	void DescriptorSet::OnDestroy() {}


	void DescriptorSet::BindDescriptor(Vulkan::CommandBuffer* commandBuffer) const {

		vkCmdBindDescriptorSets(commandBuffer->GetVkCommandBuffer(), pipelineBindPoint, pipelineLayout, 0, 1,
			&descriptorSet, 0, nullptr);

	}


}
