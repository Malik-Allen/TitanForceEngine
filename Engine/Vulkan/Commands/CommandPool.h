#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H

#include <vulkan.h>

namespace Vulkan {


	// Command Pools manage the memory that is used to store the buffers and command buffers allocated from them
	class CommandPool {

	public:

		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(const CommandPool&) = delete;
		CommandPool(CommandPool&&) = delete;
		CommandPool& operator = (CommandPool&&) = delete;
		
		CommandPool(const VkDevice& device_, class PhysicalDevice* physicalDevice_);
		~CommandPool();

		void OnDestroy();

		VkCommandPool GetVkCommandPool() const { return commandPool; }

	private:

		// TODO:
		// Take the queue family index as a parameter, at the moment I am only using the graphics family
		void OnCreate(const VkDevice& device_, PhysicalDevice* physicalDevice_);

		VkDevice device;
		VkCommandPool commandPool;


	};


}

#endif // !COMMANDPOOL_H