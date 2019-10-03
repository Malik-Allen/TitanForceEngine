#include "VulkanInstance.h"
#include <string>
#include <set>
#include <algorithm>
#include <cstring>
#include <fstream>

VulkanInstance::VulkanInstance(): instance(nullptr){}

VulkanInstance::~VulkanInstance() {}

bool VulkanInstance::OnCreate(int width, int height, GLFWwindow* window) {
	// The following are the steps to initializing the complete vulkan instance in connection to glfw
	// The order of things is important, as some memeber variables are dependent on others
	// Moreover the graphics pipeline has its own sequence of operations order of operations

	if (!OnCreateVulkanInstance())				// VkInstance object must be initialized before using the Vulkan API
		return false;


	CreateDebugMessenger();

	if (!OnCreateSurface(window))				
		return false;

	if (!OnSelectPhysicalDevice())
		return false;

	if (!OnCreateLogicalDevice())
		return false;

	if (!OnCreateSwapChain(width, height))
		return false;

	if (!OnCreateImageViews())
		return false;

	if (!OnCreateRenderPassAndInfo())
		return false;

	if (!OnCreateGraphicsPipeline())
		return false;

	if (!OnCreateFramebuffers())
		return false;

	if (!OnCreateCommandPool())
		return false;

	if (!OnCreateCommandBuffers())
		return false;

	if (!OnCreateSemaphores())
		return false;

	return true;
}

void VulkanInstance::OnDestroy() {
	if (enableValidationLayers) {
		OnDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}

bool VulkanInstance::OnCreateVulkanInstance() {
	
	if (enableValidationLayers && !HasValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available");
	}

	CreateApplicationInfo("TF Engine Prototype", "Titan Force Engine");
	CreateInstanceAndInfo();

	return true;
}

void VulkanInstance::CreateApplicationInfo(std::string applicationName, std::string engineName) {
	applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = applicationName.c_str();
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = engineName.c_str();
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
}

void VulkanInstance::CreateInstanceAndInfo() {
	instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &applicationInfo;

	std::vector<const char*> extensions = VulkanInstance::GetRequiredExtensions();

	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
		CreateDebugMessengerInfo();
		instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;
	}
	else {
		instanceInfo.enabledLayerCount = 0;
	}
	

	// For some reason, the 'vkCreateInstance()' function return a runtime memory access error when the instanceInfo is created outside the function that creates the instance
	// For that reason this function creates the instanceInfo as well as the instance
	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create vulkan instance!");
	}
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions() {
	uint32_t extensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool VulkanInstance::HasValidationLayerSupport() {
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

uint32_t VulkanInstance::GetValidationLayerCount() {
	uint32_t layerCount = 0;
	if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
		throw std::runtime_error("Failed to recognize validation layers!");
	}
	return layerCount;
}

std::vector<VkLayerProperties> VulkanInstance::GetAvailableLayerProperties() {
	uint32_t layerCount = GetValidationLayerCount();
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	return availableLayers;
}

void VulkanInstance::CreateDebugMessenger() {
	if (!enableValidationLayers) return;

	CreateDebugMessengerInfo();

	if (OnCreateDebugUtilsMessengerEXT(instance, &debugMessengerInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up debug messenger ext");
	}
}

void VulkanInstance::CreateDebugMessengerInfo() {
	debugMessengerInfo = {};
	debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugMessengerInfo.pfnUserCallback = DebugCallback;
}

VkResult VulkanInstance::OnCreateDebugUtilsMessengerEXT(
	VkInstance instance_,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance_, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		//throw std::runtime_error("Failed to get function ref!");
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanInstance::OnDestroyDebugUtilsMessengerEXT(
	VkInstance instance_, 
	VkDebugUtilsMessengerEXT debugMessenger, 
	const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance_, debugMessenger, pAllocator);
	}
}

bool VulkanInstance::OnSelectPhysicalDevice() {
	physicalDevice = VK_NULL_HANDLE;

	uint32_t physicalDeviceCount = GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> physicalDevices = GetPhysicalDevices();

	for (const VkPhysicalDevice& pDevice : physicalDevices) {
		if (IsDeviceSuitable(pDevice)) {
			physicalDevice = pDevice;
			break;
		}

	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to select a physical device!");
	}

	return true;
}

uint32_t VulkanInstance::GetPhysicalDeviceCount() {
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		throw std::runtime_error("Failed to find a GPU device with Vulkan support!");
	}

	return physicalDeviceCount;
}

std::vector<VkPhysicalDevice> VulkanInstance::GetPhysicalDevices() {
	uint32_t physicalDeviceCount = GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	return physicalDevices;
}

bool VulkanInstance::IsDeviceSuitable(VkPhysicalDevice pDevice) {
	VkPhysicalDeviceProperties pDeviceProperties;
	vkGetPhysicalDeviceProperties(pDevice, &pDeviceProperties);

	VkPhysicalDeviceFeatures pDeviceFeatures;
	vkGetPhysicalDeviceFeatures(pDevice, &pDeviceFeatures);

	QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(pDevice);
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	bool extensionSupported = DoesDeviceHaveExtensionSupport(pDevice);
	

	return indices.IsComplete() && extensionSupported;

}

QueueFamilyIndices VulkanInstance::GetPhysicalDeviceQueueFamilies(VkPhysicalDevice pDevice) {
	QueueFamilyIndices indices;
	std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilyProperties(pDevice);

	int i = 0;
	for (const VkQueueFamilyProperties& qFamily : queueFamilies) {
		if (qFamily.queueCount > 0 && qFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface, &presentSupport);

		if (qFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

std::vector<VkQueueFamilyProperties> VulkanInstance::GetQueueFamilyProperties(VkPhysicalDevice pDevice) {
	uint32_t queueFamilyCount = GetQueueFamilyCount(pDevice);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilyProperties.data());
	return queueFamilyProperties;
}

uint32_t VulkanInstance::GetQueueFamilyCount(VkPhysicalDevice pDevice) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);
	return queueFamilyCount;
}

bool VulkanInstance::DoesDeviceHaveExtensionSupport(VkPhysicalDevice pDevice) {
	uint32_t extensionCount = GetPhysicalDeviceExtensionCount(pDevice);
	std::vector<VkExtensionProperties> availableExtensions = GetPhysicalDeviceExtensions(pDevice);

	std::set<std::string> requiredExtensions(pDeviceExtensions.begin(), pDeviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

uint32_t VulkanInstance::GetPhysicalDeviceExtensionCount(VkPhysicalDevice pDevice) {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
	if (extensionCount == 0) {
		throw std::runtime_error("No exntensions for this device found!");
	}
	return extensionCount;
}

std::vector<VkExtensionProperties> VulkanInstance::GetPhysicalDeviceExtensions(VkPhysicalDevice pDevice) {
	uint32_t extensionCount = GetPhysicalDeviceExtensionCount(pDevice);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());
	return availableExtensions;
}

bool VulkanInstance::OnCreateLogicalDevice() {
	CreateDeviceAndInfo();
	return true;
}

void VulkanInstance::CreateDeviceAndInfo() {
	deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	CreateDeviceQueueInfo();
	deviceInfo.pQueueCreateInfos = &deviceQueueInfo;
	deviceInfo.queueCreateInfoCount = 1;

	CreatePhysicalDeviceFeatures();
	deviceInfo.pEnabledFeatures = &physicalDeviceFeatures;

	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(pDeviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames = pDeviceExtensions.data();

	if (enableValidationLayers) {
		deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		deviceInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create device!");
	}

	QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(physicalDevice);

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

	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentationQueue);

}

void VulkanInstance::CreateDeviceQueueInfo() {
	deviceQueueInfo = {};

	QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(physicalDevice);

	deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueInfo.queueFamilyIndex = indices.graphicsFamily.value();
	deviceQueueInfo.queueCount = 1;
	float queuePriority = 1.0f;
	deviceQueueInfo.pQueuePriorities = &queuePriority;
}

void VulkanInstance::CreatePhysicalDeviceFeatures() {
	physicalDeviceFeatures = {};
}

bool VulkanInstance::OnCreateSurface(GLFWwindow* window) {

	if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create vulkan surface!");
		return false;
	}
	else {
		return true;
	}
}

bool VulkanInstance::OnCreateSwapChain(int width, int height) {
	CreateSwapChainSupportDetails();
	SelectSwapSurfaceFormat(swapChainSupportDetails.formats);
	SelectSwapPresentMode(swapChainSupportDetails.presentModes);
	SelectSwapExtent(width, height, swapChainSupportDetails.capabilities);

	// NOTE: It is recommended that we use at least one more swap chain than the minimum because we want to avoid a situatuion where the driver has to wait for the minimum
	uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount + 1;

	if (swapChainSupportDetails.capabilities.minImageCount > 0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount) {
		imageCount = swapChainSupportDetails.capabilities.maxImageCount;
	}

	CreateSwapChainAndInfo(imageCount);

	if (vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create SwapChain!");
		return false;
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent2D;

	return true;
}

void VulkanInstance::CreateSwapChainSupportDetails() {

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainSupportDetails.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		swapChainSupportDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainSupportDetails.formats.data());
	}
	else {
		throw std::runtime_error("Could not find formats for swapchain!");
	}

	uint32_t presentModesCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr);

	if (presentModesCount != 0) {
		swapChainSupportDetails.presentModes.resize(presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, swapChainSupportDetails.presentModes.data());
	}
	else {
		throw std::runtime_error("Could not find present modes for swapchain!");
	}

}

void VulkanInstance::SelectSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {
	bool foundFormat = false;
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
			foundFormat = true;
		}
	}

	if (foundFormat == false)
		surfaceFormat = availableFormats[0];
}
void VulkanInstance::SelectSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes) {
	bool foundMode = false;
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
			foundMode = true;
		}
	}
	if(foundMode == false)
		presentMode =  VK_PRESENT_MODE_FIFO_KHR;
}

void VulkanInstance::SelectSwapExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != UINT32_MAX) {
		extent2D = capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { width, height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		extent2D = actualExtent;
	}
}

void VulkanInstance::CreateSwapChainAndInfo(uint32_t imageCount) {

	swapChainInfo = {};
	swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainInfo.surface = surface;

	swapChainInfo.minImageCount = imageCount;
	swapChainInfo.imageFormat = surfaceFormat.format;
	swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainInfo.imageExtent = extent2D;
	swapChainInfo.imageArrayLayers = 1;

	//NOTE: 'Image Usuage' specifies the operations you will use the image in the swap chain for. For 'Post-process' operations you will pass 'VK_IMAGE_USAGE_TRANSFER_DST_BIT' instead
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainInfo.queueFamilyIndexCount = 2;
		swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.queueFamilyIndexCount = 0;	//optional
		swapChainInfo.pQueueFamilyIndices = nullptr;	//optional
	}

	swapChainInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.presentMode = presentMode;
	swapChainInfo.clipped = VK_TRUE;
	swapChainInfo.oldSwapchain = VK_NULL_HANDLE;
}

bool VulkanInstance::OnCreateImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {

		CreateImageView(i);

		if (vkCreateImageView(device, &imageViewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image views!");
			return false;
		}
	}

	return true;
}

void VulkanInstance::CreateImageView(size_t imageNum) {
	imageViewInfo = {};

	imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewInfo.image = swapChainImages[imageNum];
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewInfo.format = swapChainImageFormat;

	imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;
}

bool VulkanInstance::OnCreateGraphicsPipeline() {

	std::vector<char> vertexShaderCode = ReadFile("C:/Users/Malik/source/repos/TitanForceEngine/TitanForceEngine/Engine/RunTime/Shaders/vert.spv");
	std::vector<char> fragShaderCode = ReadFile("C:/Users/Malik/source/repos/TitanForceEngine/TitanForceEngine/Engine/RunTime/Shaders/frag.spv");
	
	vertexShaderModule = CreateShaderModuleAndInfo(vertexShaderCode);
	fragShaderModule = CreateShaderModuleAndInfo(fragShaderCode);

	CreateVertexShaderStageInfo();
	CreateFragShaderStageInfo();
	
	CreateVertexInputInfo();

	CreateInputAssemblyInfo();

	CreateViewport();

	CreateScissor();

	CreateViewportStateInfo();

	CreateRasterizerStateInfo();

	CreateMultisamplingStateInfo();

	CreateColorBlendAttachment();

	CreateColorBlendingInfo();

	CreateDynamicStateInfo();

	CreatePipelineLayoutAndInfo();

	CreatePipleineAndInfo();

	return true;
}

std::vector<char> VulkanInstance::ReadFile(const std::string& fileName){

	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file!");
	}

	// An advantage of starting at the bottom of the file:
		// We can determine the size of the file and create a buffer
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule VulkanInstance::CreateShaderModuleAndInfo(const std::vector<char>& shaderCode) {

	shaderModuleInfo = {};
	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = shaderCode.size();
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

	VkShaderModule newModule;

	if (vkCreateShaderModule(device, &shaderModuleInfo, nullptr, &newModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module!");
	}

	return newModule;

}

void VulkanInstance::CreateVertexShaderStageInfo() {
	vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = vertexShaderModule;
	vertexShaderStageInfo.pName = "main";
}

void VulkanInstance::CreateFragShaderStageInfo() {
	fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
}

void VulkanInstance::CreateVertexInputInfo() {
	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;
}

void VulkanInstance::CreateInputAssemblyInfo() {
	inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
}

void VulkanInstance::CreateViewport() {
	viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
}

void VulkanInstance::CreateScissor() {
	scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
}

void VulkanInstance::CreateViewportStateInfo() {
	viewportStateInfo = {};
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;
}

void VulkanInstance::CreateRasterizerStateInfo() {
	rasterizerStateInfo = {};
	rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerStateInfo.depthClampEnable = VK_FALSE;
	rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerStateInfo.lineWidth = 1.0f;
	rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizerStateInfo.depthBiasEnable = VK_FALSE;
	rasterizerStateInfo.depthBiasClamp = 0.0f;
	rasterizerStateInfo.depthBiasConstantFactor = 0.0f;
	rasterizerStateInfo.depthBiasSlopeFactor = 0.0f;
}

void VulkanInstance::CreateMultisamplingStateInfo() {
	multisamplingStateInfo = {};
	multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingStateInfo.minSampleShading = 1.0f;	//optional
	multisamplingStateInfo.pSampleMask = nullptr;	//optional
	multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE;	//optional
	multisamplingStateInfo.alphaToOneEnable = VK_FALSE;	//optional
}

void VulkanInstance::CreateColorBlendAttachment() {
	colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

void VulkanInstance::CreateColorBlendingInfo() {
	colorBlendingInfo = {};
	colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingInfo.logicOpEnable = VK_FALSE;
	colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlendingInfo.attachmentCount = 1;
	colorBlendingInfo.pAttachments = &colorBlendAttachment;
	colorBlendingInfo.blendConstants[0] = 0.0f; // Optional
	colorBlendingInfo.blendConstants[1] = 0.0f; // Optional
	colorBlendingInfo.blendConstants[2] = 0.0f; // Optional
	colorBlendingInfo.blendConstants[3] = 0.0f; // Optional
}

void VulkanInstance::CreateDynamicStateInfo() {
	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

	dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.pDynamicStates = dynamicStates;
}

void VulkanInstance::CreatePipelineLayoutAndInfo() {
	pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout!");
	}
}

bool VulkanInstance::OnCreateRenderPassAndInfo() {

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &subpassDependency;


	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create render pass!");
	}

	return true;
}

void VulkanInstance::CreatePipleineAndInfo() {

	VkPipelineShaderStageCreateInfo stages[] = { vertexShaderStageInfo, fragShaderStageInfo };

	pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = stages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportStateInfo;
	pipelineInfo.pRasterizationState = &rasterizerStateInfo;
	pipelineInfo.pMultisampleState = &multisamplingStateInfo;
	pipelineInfo.pDepthStencilState = nullptr;	//Optional
	pipelineInfo.pColorBlendState = &colorBlendingInfo;
	pipelineInfo.pDynamicState = nullptr;	//Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;	//Optional

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline");
	}

}

bool VulkanInstance::OnCreateFramebuffers() {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	// We are going to create framebuffers for all the image views
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = { swapChainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer for swapchain image views");
			return false;
		}

	}

	return true;
}

bool VulkanInstance::OnCreateCommandPool() {
	QueueFamilyIndices indices = GetPhysicalDeviceQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex = indices.graphicsFamily.value();
	commandPoolInfo.flags = 0;	//Optional

	if (vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool!");
		return false;
	}


	return true;
}

bool VulkanInstance::OnCreateCommandBuffers() {

	commandBuffers.resize(swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo bufferAllocInfo = {};
	bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferAllocInfo.commandPool = commandPool;
	bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufferAllocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &bufferAllocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffer memory!");
		return false;
	}

	InitCommandBufferAndRenderPass();

	return true;
}

void VulkanInstance::InitCommandBufferAndRenderPass() {

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo commandBuffBeginInfo = {};
		commandBuffBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBuffBeginInfo.flags = 0;	//Optional
		commandBuffBeginInfo.pInheritanceInfo = nullptr;	//Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &commandBuffBeginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin command buffer!");
		}

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = swapChainFramebuffers[i];		//Carefull now!
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}

	}

}

bool VulkanInstance::OnCreateSemaphores() {

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS || 
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create semaphores!");
		return false;
	}

	return true;
}

void VulkanInstance::RenderFrame() {
	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);


	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitFlags;
	
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create draw command buffer!");
	}


	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = waitSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;	//Optional
	

	vkQueuePresentKHR(presentationQueue, &presentInfo);
}
