#include "Instance.h"
#include "../../Debug/Debug.h"

namespace Vulkan {	

	Instance::Instance(const std::string& ApplicationName, const std::string& EngineName, int Version, bool enableValidationLayers_) :
		instance(VK_NULL_HANDLE),
		enableValidationLayers(enableValidationLayers_),
		debugMessenger(VK_NULL_HANDLE)
	{
		OnCreate(ApplicationName, EngineName, Version, enableValidationLayers_);
	}

	Instance::~Instance() {}

	// Creates Vulkan Instance Library, and initializes Debug Validation Layers if they should be enabled
	void Instance::OnCreate(const std::string& ApplicationName, const std::string& EngineName, int Version, bool enableValidationLayers_) {
		enableValidationLayers = enableValidationLayers_;

		// Creating Application Info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = ApplicationName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(Version, 0, 0);
		appInfo.pEngineName = EngineName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(Version, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;


		// Creating instance info
		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;


		// Getting GLFW extensions
		std::vector<const char*> extensions = Instance::GetRequiredExtensions();	

		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceInfo.ppEnabledExtensionNames = extensions.data();


		// Checking to see if we want to enable validation support and our device does not have validation layer support
		if (enableValidationLayers && !HasValidationLayerSupport()) {

			Debug::Warning("Warning! Validation Layers Enabled, but device has no validation layer support!", __FILE__, __LINE__);
			enableValidationLayers = false;

		}


		// Creating debug validation layers
		if (enableValidationLayers) {	

			instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceInfo.ppEnabledLayerNames = validationLayers.data();
			
			VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
			debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugMessengerInfo.pfnUserCallback = DebugCallback;


			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

			// Creating vulkan instance
			if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create vulkan instance!");
			}

			// Creating Debug Messenger
			if (OnCreateDebugUtilsMessengerEXT(instance, &debugMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
				throw std::runtime_error("Failed to set up debug messenger ext");
			}

			return;

		}
		else {

			instanceInfo.enabledLayerCount = 0;

		}



		// Creating vulkan instance
		if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create vulkan instance!");
		}

	}

	void Instance::OnDestroy() {

		if (enableValidationLayers) {
			OnDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		vkDestroyInstance(instance, nullptr);
	}

	// Returns the extensions required for GLFW
	std::vector<const char*> Instance::GetRequiredExtensions() {
		uint32_t extensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	
	// Returns true if this device has validation layer support	
	bool Instance::HasValidationLayerSupport() {
		uint32_t availableLayersCount = GetValidationLayerCount();
		std::vector<VkLayerProperties> availableLayers = GetAvailableLayerProperties();

		bool isLayerFound = false;
		for (const char* layerName : validationLayers) {
			isLayerFound = false;

			for (const VkLayerProperties& layerProperty : availableLayers) {

				if (strcmp(layerName, layerProperty.layerName) == 0) {
					isLayerFound = true;
					break;
				}
			}

			if (!isLayerFound)
				return false;
		}

		return true;
	}

	uint32_t Instance::GetValidationLayerCount() {
		uint32_t layerCount = 0;
		if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
			throw std::runtime_error("Failed to recognize validation layers!");
		}
		return layerCount;
	}

	std::vector<VkLayerProperties> Instance::GetAvailableLayerProperties() {
		uint32_t layerCount = GetValidationLayerCount();
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		return availableLayers;
	}

	VkResult Instance::OnCreateDebugUtilsMessengerEXT(VkInstance instance_, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return func(instance_, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			// throw std::runtime_error("Failed to get function ref!");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void Instance::OnDestroyDebugUtilsMessengerEXT(VkInstance instance_, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance_, debugMessenger, pAllocator);
		}
	}


}

