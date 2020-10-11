#ifndef VKINSTANCE_H
#define VKINSTANCE_H

#include <vulkan.h>
#include <memory>	// Unique_ptr and std lib
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Vulkan {

	// Vulkan Library
	class Instance {

		friend class LogicalDevice;

	public:

		Instance(const Instance&) = delete;
		Instance& operator=(const Instance&) = delete;
		Instance(Instance&&) = delete;
		Instance& operator = (Instance&&) = delete;
		
		Instance(const std::string& ApplicationName, const std::string& EngineName, int Version, bool enableValidationLayers_);
		~Instance();

	private:

		VkInstance instance;
		bool enableValidationLayers;
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		VkDebugUtilsMessengerEXT debugMessenger;

		// Creates Vulkan Instance Library, and initializes Debug Validation Layers if they should be enabled
		void OnCreate(const std::string& ApplicationName, const std::string& EngineName, int Version, bool enableValidationLayers_);

	public:

		
		void OnDestroy();

		const VkInstance& GetVkInstance() const { return instance; }

		bool GetEnabledValidationLayers() const { return enableValidationLayers; }


	private:

		
		std::vector<const char*> GetRequiredExtensions();	// Returns the extensions required for GLFW
		bool HasValidationLayerSupport();	// Returns true if this device has validation layer support					
		uint32_t GetValidationLayerCount();
		std::vector<VkLayerProperties> GetAvailableLayerProperties();

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

			// NOTE: This can be the area in which we filter some of the debug messages
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}


		VkResult OnCreateDebugUtilsMessengerEXT( VkInstance instance_, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		void OnDestroyDebugUtilsMessengerEXT( VkInstance instance_,	VkDebugUtilsMessengerEXT debugMessenger, 
			const VkAllocationCallbacks* pAllocator);


	};

}




#endif // !VKINSTANCE_H
