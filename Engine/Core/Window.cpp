#include "Window.h"
#include <cstring>


Window::Window(): 
	window(nullptr),
	width(800),
	height(600)
	{}

Window::~Window() {}

bool Window::OnCreate(std::string name, const int newWidth, const int newHeight) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;
	
	this->width = newWidth;
	this->height = newHeight;
	
	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);

	if (!window)
		return false;

	PrintSupportedSDLExtensions();

	if (!OnCreateVulkanInstance()) 
		return false;

	CreateDebugMessengerInfo();

	if (!OnSelectAPhysicalDevice())
		return false;

	if (!OnCreateLogicalDeivce())
		return false;

	return true;

}

void Window::OnDestroy() {
	if (enableValidationLayers) {
		OnDestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
	}
	vkDestroyDevice(vkDevice, nullptr);
	vkDestroyInstance(vkInstance, nullptr);
	SDL_DestroyWindow(window);
	window = nullptr;
}

SDL_Window* Window::GetWindow() const { return window; }

int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }

uint32_t Window::GetSDLExtensionCount() {
	uint32_t extensionCount = 0;
	
	if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &extensionCount, nullptr)) {
		throw std::runtime_error("Failed to get extensions!");
	}
	
	return extensionCount;
}

std::vector<const char*> Window::GetSDLExtensionNames(uint32_t extensionCount) {
	std::vector<const char*> extensionNames;
	extensionNames.resize(extensionCount);

	if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &extensionCount, extensionNames.data())) {
		throw std::runtime_error("Failed To get extensions!");
	}

	if (enableValidationLayers) {
		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensionNames;
}

uint32_t Window::GetAvailableLayersCount() {
	uint32_t layerCount = 0;
	if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
		throw std::runtime_error("Failed to get validation layer count!");
	}

	return layerCount;
}

std::vector<VkLayerProperties> Window::GetAvailableLayerProperties(uint32_t layerCount) {
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	return availableLayers;
}

bool Window::HasValidationLayerSupport() {
	uint32_t avalaiableLayerCount = GetAvailableLayersCount();
	std::vector<VkLayerProperties> availableLayers = GetAvailableLayerProperties(avalaiableLayerCount);

	bool isLayerFound = false;
	for (const char* layerName : validationLayers) {
		isLayerFound = false;

		for (const auto& layerProperty : availableLayers) {

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

VkApplicationInfo Window::CreateApplicationInfo() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Titan Force Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Titan Force Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	//The pNext member of appInfo is a pointer to extensions of this struct
		// Can be used to point to extensions in the future

	return appInfo;
}

bool Window::OnCreateVulkanInstanceInfo(VkApplicationInfo additionalAppInfo) {
	uint32_t sdlExtensionCount = GetSDLExtensionCount();
	std::vector<const char*> sdlExtensionNames = GetSDLExtensionNames(sdlExtensionCount);
	
	VkInstanceCreateInfo vkInstanceInfo = {};
	vkInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceInfo.pApplicationInfo = &additionalAppInfo;
	vkInstanceInfo.enabledExtensionCount = sdlExtensionCount;
	vkInstanceInfo.ppEnabledExtensionNames = sdlExtensionNames.data();

	if (enableValidationLayers) {
		vkInstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		vkInstanceInfo.ppEnabledLayerNames = validationLayers.data();
		CreateDebugMessengerInfo();
		vkInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		vkInstanceInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&vkInstanceInfo, nullptr, &vkInstance) != VK_SUCCESS) {
		return false;
	}
	
	return true;

}

bool Window::OnCreateVulkanInstance() {

	if (enableValidationLayers && !HasValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = CreateApplicationInfo();	// Application info is optinonal info to pass the VkInstance
	
	if (!OnCreateVulkanInstanceInfo(appInfo))
		return false;

	return true;
}

std::vector<VkExtensionProperties> Window::GetSupportedExtenions() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	return extensions;
}

void Window::PrintSupportedSDLExtensions() {
	std::vector<VkExtensionProperties> extensions = GetSupportedExtenions();
	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

uint32_t Window::GetPhysicalDeviceCount() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
	
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find a GPU with Vulkan Support!");
	}

	return deviceCount;
}

std::vector<VkPhysicalDevice> Window::GetPhysicalDevices(uint32_t deviceCount) {
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());
	return devices;
}

bool Window::IsDeviceSuitable(VkPhysicalDevice deviceCandidate) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(deviceCandidate, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(deviceCandidate, &deviceFeatures);

	Window::QueueFamilyIndices indices = FindGraphicsQueueFamily(deviceCandidate);


	return indices.IsComplete();

	//EXAMPLE: return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader);
}

bool Window::OnSelectAPhysicalDevice() {
	physicalDevice = VK_NULL_HANDLE;

	uint32_t deviceCount = GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> devices = GetPhysicalDevices(deviceCount);

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU!");
	}

	return true;
}

Window::QueueFamilyIndices Window::FindQueueFamilies(VkPhysicalDevice physicalDevice_) {
	Window::QueueFamilyIndices queueFamilyIndices = FindGraphicsQueueFamily(physicalDevice_);
	return queueFamilyIndices;
}

Window::QueueFamilyIndices Window::FindGraphicsQueueFamily(VkPhysicalDevice physicalDevice_) {
	Window::QueueFamilyIndices indices;
	std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilyProperties(physicalDevice_);

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

uint32_t Window::GetQueueFamilyCount(VkPhysicalDevice physicalDevice_) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
	return queueFamilyCount;
}

std::vector<VkQueueFamilyProperties> Window::GetQueueFamilyProperties(VkPhysicalDevice physicalDevice_) {
	uint32_t queueFamilyCount = GetQueueFamilyCount(physicalDevice_);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilyProperties.data());

	return queueFamilyProperties;
}

bool Window::OnCreateLogicalDeivce() {

	if (!CreateDeviceCreateInfo())
		return false;

	return true;

}

bool Window::CreateDeviceCreateInfo() {
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	VkDeviceQueueCreateInfo queueInfo = CreateDeviceQueueCreateInfo(physicalDevice);
	deviceCreateInfo.pQueueCreateInfos = &queueInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;

	VkPhysicalDeviceFeatures deviceFeatures = CreatePhysicalDeviceFeatures(physicalDevice);
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = 0;

	if (enableValidationLayers) {
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		deviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &vkDevice) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create device!");
		return false;
	}

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	vkGetDeviceQueue(vkDevice, indices.graphicsFamily, 0, &graphicsQueue);


	return true;
	
}

VkDeviceQueueCreateInfo Window::CreateDeviceQueueCreateInfo(VkPhysicalDevice physicalDevice_) {
	VkDeviceQueueCreateInfo queueCreateInfo = {};

	Window::QueueFamilyIndices indices = FindQueueFamilies(physicalDevice_);

	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	
	return queueCreateInfo;
}

VkPhysicalDeviceFeatures Window::CreatePhysicalDeviceFeatures(VkPhysicalDevice physicalDevice_) {
	VkPhysicalDeviceFeatures deviceFeatures = {};
	// Future development will cover this
	return deviceFeatures;
}

void Window::CreateDebugMessengerInfo() {

	debugCreateInfo = {};
	debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	// We pass the struct the severity of the debug messages we would like to recievce
	debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// Same here, we pass the message types we would like to display
	debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugCreateInfo.pfnUserCallback = DebugCallback; // Point to the callback function
	//debugCreateInfo.pUserData = nullptr; // optional

}

void Window::CreateDebugMessengerInfoEXT() {

	if (!enableValidationLayers) return;

	CreateDebugMessengerInfo();
	
	if (OnCreateDebugUtilsMessengerEXT(vkInstance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up debug messenger!");
	}
	
}