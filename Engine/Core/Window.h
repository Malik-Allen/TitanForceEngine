#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>
#include <vector>
#include <SDL_vulkan.h>
#include <vulkan.h>
#include <iostream>
#include <optional>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Window {

public:
	// The Window class will not be using the copy and move operations
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator= (Window&&) = delete;

	Window();
	~Window();

	bool OnCreate(std::string name, const int width, const int height);
	void OnDestroy();

	SDL_Window* GetWindow() const;
	int GetWidth() const;
	int GetHeight() const;

private:

	SDL_Window* window;

	int width;
	int height;


	VkInstance vkInstance; // In order to make a VkInstance a 'vkCreateInfo' struct containing the 'global extensions and validation layers' is needed


	uint32_t GetSDLExtensionCount();
	std::vector<const char*> GetSDLExtensionNames(uint32_t extensionCount);

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	uint32_t GetAvailableLayersCount();
	std::vector<VkLayerProperties> GetAvailableLayerProperties(uint32_t layerCount);
	bool HasValidationLayerSupport();

	VkApplicationInfo CreateApplicationInfo();
	bool OnCreateVulkanInstanceInfo(VkApplicationInfo additionalAppInfo);
	bool OnCreateVulkanInstance();

	std::vector<VkExtensionProperties> GetSupportedExtenions();
	void PrintSupportedSDLExtensions();
	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	uint32_t GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> GetPhysicalDevices(uint32_t deviceCount);
	bool IsDeviceSuitable(VkPhysicalDevice deviceCandidate);	// NOTE: This area of the code has room for a whole system for determining the best candidate device(GPU) 
																// based on the candidate's properties and features.

	bool OnSelectAPhysicalDevice();								// **IMPORTANT: We are currently taking the first Physical Device (GPU) we find.
	
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		// IMPORTANT**: Updated to the C++17 compiler, just worried the implementation is working but not in the way i expected it
		bool IsComplete() {
				if (graphicsFamily.has_value()) {
					return true;
				}
				else {
					throw std::runtime_error("No graphics family found!");
					return false;
				}
			
		}
	};

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);

	Window::QueueFamilyIndices FindGraphicsQueueFamily(VkPhysicalDevice physicalDevice);
	uint32_t GetQueueFamilyCount(VkPhysicalDevice physicalDevice);
	std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice physicalDevice);


	VkDevice vkDevice;
	bool OnCreateLogicalDeivce();
	bool CreateDeviceCreateInfo();
	VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(VkPhysicalDevice physicalDevice);
	VkPhysicalDeviceFeatures CreatePhysicalDeviceFeatures(VkPhysicalDevice physicalDevice);

	VkQueue graphicsQueue;

	VkDebugUtilsMessengerEXT debugMessenger;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		// NOTE: This can be the area in which we filter some of the debug messages
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void CreateDebugMessengerInfo();
	void CreateDebugMessengerInfoEXT();
	VkResult OnCreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			throw std::runtime_error("Failed to get function ref!");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void OnDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

};



#endif