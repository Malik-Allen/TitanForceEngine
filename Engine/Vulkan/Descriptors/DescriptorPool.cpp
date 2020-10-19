#include "DescriptorPool.h"

#include "../VulkanRenderer.h"

namespace Vulkan {

	DescriptorPool::DescriptorPool(VkDescriptorType type, uint32_t descriptorCount) :
		descriptorPool(VK_NULL_HANDLE)
	{
		OnCreate(type, descriptorCount);
	}


	DescriptorPool::~DescriptorPool() {}


	void DescriptorPool::OnCreate(VkDescriptorType type, uint32_t descriptorCount) {

		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		// First, describe which descriptor types our descriptor sets are going to contain and how many of them
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = descriptorCount;

		// We will allocate one of these descriptors per frame
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;

		poolInfo.maxSets = descriptorCount;	// Max number of descriptors that can be allocated

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}

	}


	void DescriptorPool::OnDestroy() {

		auto device = Vulkan::VulkanRenderer::GetInstance()->GetLogicalDevice()->GetVkDevice();

		vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	}


}