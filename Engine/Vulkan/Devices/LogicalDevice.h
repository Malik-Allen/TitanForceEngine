#ifndef LOGICALDEVICE_H
#define LOGICALDEVICE_H

#include <vulkan.h>
#include <memory>

namespace Vulkan {

	// The Logical Device interfaces with the physical device
	class LogicalDevice {

	public:

		LogicalDevice(const LogicalDevice&) = delete;
		LogicalDevice& operator=(const LogicalDevice&) = delete;
		LogicalDevice(LogicalDevice&&) = delete;
		LogicalDevice& operator = (LogicalDevice&&) = delete;

		
		void OnDestroy();

		const VkDevice& GetVkDevice() const { return vkDevice; }
		const VkQueue& GetGraphicsQueue() const { return graphicsQueue; }
		const VkQueue& GetPresentationQueue() const { return presentationQueue; }

		LogicalDevice(class PhysicalDevice* physicalDevice_, class Instance* instance_);
		~LogicalDevice();

	private:

		// Creates device queues, the logical device and enables physical features
		void OnCreate(PhysicalDevice* physicalDevice_, Instance* instance_);

		VkDevice vkDevice;
		PhysicalDevice* physicalDevice;
		Instance* instance;

		float queuePriority;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;

		


	};

}


#endif // !LOGICALDEVICE_H
