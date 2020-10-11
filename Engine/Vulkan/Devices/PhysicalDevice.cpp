#include "PhysicalDevice.h"

#include <set>

#include "Instance.h"
#include "Surface.h"
#include "../RenderPass/SwapChain.h"

namespace Vulkan {

	PhysicalDevice::PhysicalDevice(const VkSurfaceKHR& surface, const VkInstance& instance) :
		physicalDevice(VK_NULL_HANDLE),
		vkSurface(VK_NULL_HANDLE),
		vkInstance(VK_NULL_HANDLE)
	{
		OnCreate(surface, instance);
	}

	PhysicalDevice::~PhysicalDevice() {}


	// On Create we choose a Graphics Card that suits are needs, ( currently uses the first one we find )
	void PhysicalDevice::OnCreate(const VkSurfaceKHR& surface, const VkInstance& instance) {

		vkSurface = surface;
		vkInstance = instance;

		if (surface == VK_NULL_HANDLE) {
			throw std::runtime_error("Null surface passed to physical device!");
		}

		if (vkInstance == VK_NULL_HANDLE) {
			throw std::runtime_error("Null instance passed to physical device!");
		}


		physicalDevice = VK_NULL_HANDLE;

		uint32_t physicalDeviceCount = GetPhysicalDeviceCount();	// Getting graphics card count
		std::vector<VkPhysicalDevice> physicalDevices = GetPhysicalDevices();	// Gettings graphics cards

		// In our case we will be choosing the first available graphics cards
		for (const VkPhysicalDevice& pDevice : physicalDevices) {

			// This is where logic to prioritize other graphics cards over others would be implemented
			if (IsDeviceSuitable(pDevice)) {
				physicalDevice = pDevice;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Failed to select a physical device!");
		}

	}

	void PhysicalDevice::OnDestroy() {}


	// Returns the number of graphics cards on this device
	uint32_t PhysicalDevice::GetPhysicalDeviceCount() {

		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, nullptr);

		if (physicalDeviceCount == 0) {
			throw std::runtime_error("Failed to find a GPU device with Vulkan support!");
		}

		return physicalDeviceCount;

	}

	// Returns a vector of the graphic cards on this device
	std::vector<VkPhysicalDevice> PhysicalDevice::GetPhysicalDevices() {

		uint32_t physicalDeviceCount = GetPhysicalDeviceCount();
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, physicalDevices.data());
		return physicalDevices;

	}

	// Returns true if @VkPhysicalDevice meets are specified properties
	bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice pDevice) {

		// Basic device properties can be queried here, by expanding the following structs properties
		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevice, &pDeviceProperties);

		// Optional features like texture compression can be queried here, by expanding the following structs properties
		VkPhysicalDeviceFeatures pDeviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevice, &pDeviceFeatures);

		// Getting queue familes from device
		QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(pDevice);
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		// Do we have support for required extensions
		bool extensionSupported = DoesDeviceHaveExtensionSupport(pDevice);

		// Proceeding to check if swap chains are supported for the device
		bool swapChainAdequate = false;
		if (extensionSupported) {
			Vulkan::SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(pDevice, vkSurface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.IsComplete() && extensionSupported && swapChainAdequate;

	}

	// Returns the Queue families indices on this device
	QueueFamilyIndices PhysicalDevice::GetPhysicalDeviceQueueFamilies(VkPhysicalDevice pDevice) {

		// Getting queue families from the device
		QueueFamilyIndices indices;
		std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilyProperties(pDevice);


		// Going through all the queue family loking for 
		int i = 0;
		for (const VkQueueFamilyProperties& qFamily : queueFamilies) {

			// Checking the graphics queue family
			if (qFamily.queueCount > 0 && qFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, vkSurface, &presentSupport);

			if (qFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}

			// In our case we only check for a graphics queue and that it is present
			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;

	}


	// Returns a vector of the queue families from the physical device
	std::vector<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties(VkPhysicalDevice pDevice)  {

		uint32_t queueFamilyCount = GetQueueFamilyCount(pDevice);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilyProperties.data());
		return queueFamilyProperties;

	}


	// Returns the number of queue families present on the device
	uint32_t PhysicalDevice::GetQueueFamilyCount(VkPhysicalDevice pDevice) {
		
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);
		return queueFamilyCount;

	}

	// Returns true if the device has the required extension support
	bool PhysicalDevice::DoesDeviceHaveExtensionSupport(VkPhysicalDevice pDevice) {
		
		
		uint32_t extensionCount = GetPhysicalDeviceExtensionCount(pDevice);	// Getting the device extension count
		std::vector<VkExtensionProperties> availableExtensions = GetPhysicalDeviceExtensions(pDevice);	// Getting all the device extensions

		std::set<std::string> requiredExtensions(pDeviceExtensions.begin(), pDeviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		// In ourcase we return true no matter what, this would be an area to make certain we have the device extensions supported
		return requiredExtensions.empty();

	}


	// Returns the number of Extensions of the physical device
	uint32_t PhysicalDevice::GetPhysicalDeviceExtensionCount(VkPhysicalDevice pDevice) {

		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
		if (extensionCount == 0) {
			throw std::runtime_error("No exntensions for this device found!");
		}
		return extensionCount;

	}


	// Returns a vector of the extensions on the physical device
	std::vector<VkExtensionProperties> PhysicalDevice::GetPhysicalDeviceExtensions(VkPhysicalDevice pDevice) {

		uint32_t extensionCount = GetPhysicalDeviceExtensionCount(pDevice);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());
		return availableExtensions;

	}


	// 
	void PhysicalDevice::CreatePhysicalDeviceFeatures() {

		physicalDeviceFeatures = {};

	}
}
