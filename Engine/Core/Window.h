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

	bool OnCreate(std::string name, const int width, const int height);	// Starting point!
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
	
	bool HasValidationLayerSupport();
	uint32_t GetAvailableLayersCount();
	std::vector<VkLayerProperties> GetAvailableLayerProperties(uint32_t layerCount);

	bool OnCreateVulkanInstance();
	VkApplicationInfo CreateApplicationInfo();
	bool OnCreateVulkanInstanceInfo(VkApplicationInfo additionalAppInfo);
	std::vector<VkExtensionProperties> GetSupportedExtenions();
	void PrintSupportedSDLExtensions();

	VkSurfaceKHR vkSurface;
	bool OnCreateVulkanSurface();
	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	bool OnSelectAPhysicalDevice();								// **IMPORTANT: We are currently taking the first Physical Device (GPU) we find.
	uint32_t GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> GetPhysicalDevices(uint32_t deviceCount);
	bool IsDeviceSuitable(VkPhysicalDevice deviceCandidate);	// NOTE: This area of the code has room for a whole system for determining the best candidate device(GPU) 
																// based on the candidate's properties and features.

	VkDevice vkDevice;
	bool OnCreateLogicalDeivce();
	bool CreateDeviceCreateInfo();
	VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(VkPhysicalDevice physicalDevice);
	VkPhysicalDeviceFeatures CreatePhysicalDeviceFeatures(VkPhysicalDevice physicalDevice);
	
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		// IMPORTANT**: Updated to the C++17 compiler
			// Worried the implementation of std::option is working but beyond my current knowledge
		bool IsComplete() {
				if (graphicsFamily.has_value() && presentFamily.has_value()) {
					return true;
				}
				else {
					throw std::runtime_error("No graphics family found!");
					return false;
				}
		}
	};

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice_);
	Window::QueueFamilyIndices FindGraphicsQueueFamily(VkPhysicalDevice physicalDevice_);
	std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice physicalDevice_);
	uint32_t GetQueueFamilyCount(VkPhysicalDevice physicalDevice_);

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	bool HasDeviceExtensionSupport(VkPhysicalDevice physicalDevice_);

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	VkSwapchainKHR swapChain;
	bool OnCreateSwapChain();
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice_);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	bool OnCreateImageViews();

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