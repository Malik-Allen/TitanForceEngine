#include "Window.h"
#include <cstring>
#include <set>
#include <cstdint>
#include <algorithm>


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

	if (!OnCreateVulkanSurface())
		return false;

	CreateDebugMessengerInfo();

	if (!OnSelectAPhysicalDevice())
		return false;

	if (!OnCreateLogicalDeivce())
		return false;

	if (!OnCreateSwapChain())
		return false;

	if (!OnCreateImageViews())
		return false;

	return true;

}

void Window::OnDestroy() {
	if (enableValidationLayers) {
		OnDestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
	}
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(vkDevice, imageView, nullptr);
	}
	vkDestroySwapchainKHR(vkDevice, swapChain, nullptr);
	vkDestroyDevice(vkDevice, nullptr);
	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
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

bool Window::OnCreateVulkanInstance() {

	if (enableValidationLayers && !HasValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = CreateApplicationInfo();	// Application info is optinonal info to pass the VkInstance

	if (!OnCreateVulkanInstanceInfo(appInfo))
		return false;

	return true;
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

bool Window::OnCreateVulkanSurface() {
	if (!SDL_Vulkan_CreateSurface(window, vkInstance, &vkSurface)) {
		throw std::runtime_error("Failed to create vulkan surface!");
		return false;
	}
	else {
		return true;
	}
	
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

	bool extensionSupported = HasDeviceExtensionSupport(deviceCandidate);
	bool swapChainAdequate = false;

	if (extensionSupported) {
		Window::SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(deviceCandidate);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	else {
		throw std::runtime_error("No extensions supported!");
	}

	return indices.IsComplete() && extensionSupported && swapChainAdequate;

	//EXAMPLE: return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader);
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

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	vkGetDeviceQueue(vkDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(vkDevice, indices.presentFamily.value(), 0, &presentationQueue);

	return true;
}

VkDeviceQueueCreateInfo Window::CreateDeviceQueueCreateInfo(VkPhysicalDevice physicalDevice_) {
	VkDeviceQueueCreateInfo queueCreateInfo = {};

	Window::QueueFamilyIndices indices = FindQueueFamilies(physicalDevice_);

	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
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

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice_, i, vkSurface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

std::vector<VkQueueFamilyProperties> Window::GetQueueFamilyProperties(VkPhysicalDevice physicalDevice_) {
	uint32_t queueFamilyCount = GetQueueFamilyCount(physicalDevice_);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilyProperties.data());

	return queueFamilyProperties;
}

uint32_t Window::GetQueueFamilyCount(VkPhysicalDevice physicalDevice_) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
	return queueFamilyCount;
}

bool Window::HasDeviceExtensionSupport(VkPhysicalDevice physicalDevice_) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool Window::OnCreateSwapChain() {
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	// NOTE: It is recommended that we use at least one more swap chain than the minimum because we want to avoid a situatuion where the driver has to wait for the minimum
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; 

	if (swapChainSupport.capabilities.minImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = vkSurface;

	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;
	//NOTE: 'Image Usuage' specifies the operations you will use the image in the swap chain for. For 'Post-process' operations you will pass 'VK_IMAGE_USAGE_TRANSFER_DST_BIT' instead
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily) {
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;	//optional
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;	//optional
	}

	swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(vkDevice, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create SwapChain!");
		return false;
	}

	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	return true;

}

Window::SwapChainSupportDetails Window::QuerySwapChainSupport(VkPhysicalDevice physicalDevice_) {
	Window::SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice_, vkSurface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, vkSurface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, vkSurface, &formatCount, details.formats.data());
	}
	else {
		throw std::runtime_error("Could not find formats for swapchain!");
	}

	uint32_t presentModesCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice_, vkSurface, &presentModesCount, nullptr);

	if (presentModesCount != 0) {
		details.presentModes.resize(presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice_, vkSurface, &presentModesCount, details.presentModes.data());
	}
	else {
		throw std::runtime_error("Could not find present modes for swapchain!");
	}

	return details;
}

VkSurfaceFormatKHR Window::ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Window::ChooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Window::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { width, height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

bool Window::OnCreateImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo imageViewCreateInfo = {};

		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = swapChainImages[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = swapChainImageFormat;

		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(vkDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image views!");
			return false;
		}
	}

	

	return true;
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