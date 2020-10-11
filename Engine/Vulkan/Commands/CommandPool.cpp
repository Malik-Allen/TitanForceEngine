#include "CommandPool.h"

#include "../Devices/PhysicalDevice.h"

namespace Vulkan {

	CommandPool::CommandPool(const VkDevice& device_, PhysicalDevice* physicalDevice_) :
		device(device_),
		commandPool(VK_NULL_HANDLE)
	{
		OnCreate(device_, physicalDevice_);
	}


	CommandPool::~CommandPool() {}

	
	void CommandPool::OnCreate(const VkDevice& device_, PhysicalDevice* physicalDevice_) {

		device = device_;
		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null device passed to command pool!");
		}

		if (physicalDevice_ == nullptr) {
			throw std::runtime_error("Error! Null physical device passed to command pool!");
		}
		
		// Command buffers are executed by submiting them on one of the devic queues
		// Each command pool can only allocate command buffers that are submitted on a single type of queue

		QueueFamilyIndices indices = physicalDevice_->GetPhysicalDeviceQueueFamilies(physicalDevice_->GetVkPhysicalDevice());

		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		commandPoolInfo.flags = 0;	//Optional


		// We only need to create the command buffers at the begining of the program and then execute them many times in the main loop
		if (vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create command pool!");
		}

	}


	void CommandPool::OnDestroy() {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}

}