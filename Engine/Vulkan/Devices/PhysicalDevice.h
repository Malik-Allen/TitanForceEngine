#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include <vulkan.h>
#include <memory>
#include <vector>
#include <optional>

namespace Vulkan {

	// Vulkan requires commands to be sent to queues, each Queue Family allow for a  subset of commands
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() {
			if (graphicsFamily.has_value() && presentFamily.has_value()) {
				return true;
			}
			else {
				throw std::runtime_error("Queue families are not present");
				return false;
			}
		}
	};

	// Graphics Cards
	class PhysicalDevice {

		friend class LogicalDevice;
		friend class SwapChain;
		friend class CommandPool;

	public:

		PhysicalDevice(const PhysicalDevice&) = delete;
		PhysicalDevice& operator=(const PhysicalDevice&) = delete;
		PhysicalDevice(PhysicalDevice&&) = delete;
		PhysicalDevice& operator = (PhysicalDevice&&) = delete;

		PhysicalDevice(const VkSurfaceKHR& surface, const VkInstance& instance);
		~PhysicalDevice();

		const VkPhysicalDevice& GetVkPhysicalDevice() const { return physicalDevice; }
		const VkSurfaceKHR& GetVkSurface() const { return vkSurface; }

		void OnDestroy();

	private:

		VkPhysicalDevice physicalDevice;
		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
		const std::vector<const char*> pDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };	// Currently only checking for swap chain support

		VkSurfaceKHR vkSurface;
		VkInstance vkInstance;

		// On Create we choose a Graphics Card that suits are needs
		void OnCreate(const VkSurfaceKHR& surface, const VkInstance& instance);
		
		uint32_t GetPhysicalDeviceCount();	// Returns the number of graphics cards on this device
		std::vector<VkPhysicalDevice> GetPhysicalDevices();	// Returns a vector of the graphic cards on this device
		bool IsDeviceSuitable(VkPhysicalDevice pDevice);	// Returns true if @VkPhysicalDevice meets are specified properties
		QueueFamilyIndices GetPhysicalDeviceQueueFamilies(VkPhysicalDevice pDevice);	// Returns the Queue families indices on this device
		std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice pDevice);	// Returns a vector of the queue families from the physical device
		uint32_t GetQueueFamilyCount(VkPhysicalDevice pDevice);	// Returns the number of queue families present on the device
		bool DoesDeviceHaveExtensionSupport(VkPhysicalDevice pDevice);	// Returns true if the device has the required extension support
		uint32_t GetPhysicalDeviceExtensionCount(VkPhysicalDevice pDevice);	// Returns the number of Extensions of the physical device
		std::vector<VkExtensionProperties> GetPhysicalDeviceExtensions(VkPhysicalDevice pDevice);	// Returns a vector of the extensions on the physical device

		void CreatePhysicalDeviceFeatures();


	};




}


#endif // !PHYSICALDEVICE_H
