#include "DescriptorSet.h"

#include "../VulkanRenderer.h"
#include "../Commands/CommandBuffer.h"

namespace Vulkan {

	DescriptorSet::DescriptorSet(VkPipelineBindPoint bindPoint, const VkDescriptorPool& descriptorPool_) :
		descriptorSet(VK_NULL_HANDLE),
		descriptorPool(VK_NULL_HANDLE),
		pipelineLayout(VK_NULL_HANDLE),
		pipelineBindPoint(bindPoint)
	{
		OnCreate(bindPoint, descriptorPool);
	}


	DescriptorSet::~DescriptorSet() {}


	void DescriptorSet::OnCreate(VkPipelineBindPoint bindPoint, const VkDescriptorPool& descriptorPool_) {

		pipelineBindPoint = bindPoint;
		descriptorPool = descriptorPool_;
		if (descriptorPool == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null Descriptor pool passed to descriptor set!");

		}

		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		// Specifying the descriptor pool to allocate from

		std::vector<VkDescriptorSetLayout> layouts(1, Vulkan::VulkanRenderer::GetInstance()->GetGraphicsPipeline()->GetDescriptorSetLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts.data();

		// No need to free up descriptor sets, they are automatically freed when the descriptor pool is destroyed
		
		if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

	
	}


	void DescriptorSet::OnDestroy() {}


	void DescriptorSet::BindDescriptor(Vulkan::CommandBuffer* commandBuffer) const {

		vkCmdBindDescriptorSets(commandBuffer->GetVkCommandBuffer(), pipelineBindPoint, pipelineLayout, 0, 1,
			&descriptorSet, 0, nullptr);

	}


}
