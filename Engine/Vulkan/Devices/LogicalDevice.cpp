#include "LogicalDevice.h"

#include <set>

#include "PhysicalDevice.h"
#include "Instance.h"

namespace Vulkan {

	LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice_, Instance* instance_) :
		vkDevice(VK_NULL_HANDLE),
		physicalDevice(physicalDevice_),
		instance(instance_),
		queuePriority(0.0f)
	{
		OnCreate(physicalDevice_, instance_);
	}

	LogicalDevice::~LogicalDevice() {}

	// Creates device queues, the logical device and enables physical features
	void LogicalDevice::OnCreate(PhysicalDevice* physicalDevice_, Instance* instance_) {

		physicalDevice = physicalDevice_;
		instance = instance_;

		if (physicalDevice == nullptr) {
			throw std::runtime_error("Null physical device passed to logical device!");
		}

		if (instance == VK_NULL_HANDLE) {
			throw std::runtime_error("Null instance passed to logical device!");
		}

		// Getting all the queue families on the current physical device
		QueueFamilyIndices indices = physicalDevice->GetPhysicalDeviceQueueFamilies(physicalDevice->GetVkPhysicalDevice());
		
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };


		queuePriority = 1.0f;	// Vulkan allows to use influence the scheduling of command buffer execution using a floating point number between 0.0 and 1.0

		// In creating the device queue info, we specify the number of queues we want for a single family
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Create logical device info
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceInfo.pQueueCreateInfos = queueCreateInfos.data();



		// An opporunity to specify device features that we will be using 
		physicalDevice->CreatePhysicalDeviceFeatures();
		deviceInfo.pEnabledFeatures = &physicalDevice->physicalDeviceFeatures;

		// Enabling physical device extensions
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice->pDeviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = physicalDevice->pDeviceExtensions.data();


		// Enabling Validation Layers
		if ( instance->GetEnabledValidationLayers() ) {

			deviceInfo.enabledLayerCount = static_cast<uint32_t>(instance->validationLayers.size());
			deviceInfo.ppEnabledLayerNames = instance->validationLayers.data();

		}
		else {
			deviceInfo.enabledLayerCount = 0;
		}
		

		// Instantiating the device, queue handles are automatically created as well 
		if (vkCreateDevice(physicalDevice->GetVkPhysicalDevice(), &deviceInfo, nullptr, &vkDevice) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create device!");
		}


		vkGetDeviceQueue(vkDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);	// Because we only using a single queue per family we use index 0
		vkGetDeviceQueue(vkDevice, indices.presentFamily.value(), 0, &presentationQueue);
		
	}

	void LogicalDevice::OnDestroy() {

		vkDestroyDevice(vkDevice, nullptr);

	}

	
	
}