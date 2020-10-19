#include "VulkanInstance.h"
#include <string>
#include <set>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <gtc/matrix_transform.hpp>
#include <chrono>


namespace test {
VulkanInstance::VulkanInstance(): 
	instance(nullptr), 
	queuePriority (0.0f),
	currentFrame (0),
	framebufferResized(false),
	window (nullptr)
{}

VulkanInstance::~VulkanInstance() {}



bool VulkanInstance::OnCreate(int width, int height, GLFWwindow* window) {
	// The following are the steps to initializing the complete vulkan instance in connection to glfw
	// The order of things is important, as some memeber variables are dependent on others
	// Moreover the graphics pipeline has its own sequence of operations order of operations
	
	this->window = window;
	if (window == nullptr) {
		Debug::FatalError("A valid window was not passed", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateVulkanInstance()) {	// VkInstance object must be initialized before using the Vulkan API
		Debug::FatalError("Failed to created Vulkan Application & Instance Information", __FILE__, __LINE__);
		return false;
	}

	CreateDebugMessenger();

	if (!OnCreateSurface(window)) {
		Debug::FatalError("Failed to create Vulkan Surface with GLFW", __FILE__, __LINE__);
		return false;
	}

	if (!OnSelectPhysicalDevice()) {
		Debug::FatalError("Failed to select a physical device", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateLogicalDevice()) {
		Debug::FatalError("Failed to create logical device", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateSwapChain(width, height)) {
		Debug::FatalError("Failed to create Vulkan Swapchain", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateImageViews()) {
		Debug::FatalError("Failed to create Vulkan Image Views", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateRenderPassAndInfo()) {
		Debug::FatalError("Failed to create Vulkan Renderpass", __FILE__, __LINE__);
		return false;
	}

	CreateDescriptorSetLayout();

	if (!OnCreateGraphicsPipeline()) {
		Debug::FatalError("Failed to create Vulkan Graphics Pipeline", __FILE__, __LINE__);
		return false;
	}
	
	if (!OnCreateFramebuffers()) {
		Debug::FatalError("Failed to create Vulkan Frame Buffers", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateCommandPool()) {
		Debug::FatalError("Failed to create Vulkan Command Pool", __FILE__, __LINE__);
		return false;
	}

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();

	if (!OnCreateCommandBuffers()) {
		Debug::FatalError("Failed to create Vulkan Command Buffer", __FILE__, __LINE__);
		return false;
	}

	if (!OnCreateSemaphoresAndFences()) {
		Debug::FatalError("Failed to create Vulkan Semaphores", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void VulkanInstance::OnDestroy() {

	if (enableValidationLayers) {
		OnDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++) {
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	// vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	// vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}

bool VulkanInstance::OnCreateVulkanInstance() {
	
	if (enableValidationLayers && !HasValidationLayerSupport()) {
		return false;
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
		Debug::Warning("Vulkan Debugger Ext not initialized!", __FILE__, __LINE__);
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

	queuePriority = 1.0f;
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
	queuePriority = 1.0f;
	deviceQueueInfo.pQueuePriorities = &queuePriority;
}

void VulkanInstance::CreatePhysicalDeviceFeatures() {
	physicalDeviceFeatures = {};
}

bool VulkanInstance::OnCreateSurface(GLFWwindow* window) {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		return false;
	else
		return true;
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
		VkExtent2D actualExtent = { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };

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

bool VulkanInstance::OnCreateGraphicsPipeline() {	// Opportunity to create a system to select a shader

	std::vector<char> vertexShaderCode = ReadFile("C:/TitanForceEngineLibrary/TitanForceEngine/TitanForceEngine/Engine/Shaders/vert.spv");
	std::vector<char> fragShaderCode = ReadFile("C:/TitanForceEngineLibrary/TitanForceEngine/TitanForceEngine/Engine/Shaders/frag.spv");
	
	vertexShaderModule = CreateShaderModuleAndInfo(vertexShaderCode);
	fragShaderModule = CreateShaderModuleAndInfo(fragShaderCode);

	/*vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = vertexShaderModule;
	vertexShaderStageInfo.pName = "main";*/
	CreateVertexShaderStageInfo();

	/*fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";*/
	CreateFragShaderStageInfo();

	/*VkPipelineShaderStageCreateInfo stages[] = { vertexShaderStageInfo, fragShaderStageInfo };

	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;*/

	// Getting Vertex Data from struct
	/*auto bindingDescription = Vertex::GetBindingDescription();
	auto attributesDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributesDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributesDescriptions.data();*/
	CreateVertexInputInfo();

	/*inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;*/
	CreateInputAssemblyInfo();

	/*viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;*/
	CreateViewport();

	/*scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;*/
	CreateScissor();

	/*viewportStateInfo = {};
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;*/
	CreateViewportStateInfo();

	/*rasterizerStateInfo = {};
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
	rasterizerStateInfo.depthBiasSlopeFactor = 0.0f;*/
	CreateRasterizerStateInfo();

	//multisamplingStateInfo = {};
	//multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	//multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
	//multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	//multisamplingStateInfo.minSampleShading = 1.0f;	//optional
	//multisamplingStateInfo.pSampleMask = nullptr;	//optional
	//multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE;	//optional
	//multisamplingStateInfo.alphaToOneEnable = VK_FALSE;	//optional
	CreateMultisamplingStateInfo();

	//colorBlendAttachment = {};
	//colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	//colorBlendAttachment.blendEnable = VK_FALSE;
	//colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	//colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	//colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	//colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	//colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	//colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	//colorBlendAttachment.blendEnable = VK_TRUE;
	//colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	//colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	CreateColorBlendAttachment();

	//colorBlendingInfo = {};
	//colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	//colorBlendingInfo.logicOpEnable = VK_FALSE;
	//colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	//colorBlendingInfo.attachmentCount = 1;
	//colorBlendingInfo.pAttachments = &colorBlendAttachment;
	//colorBlendingInfo.blendConstants[0] = 0.0f; // Optional
	//colorBlendingInfo.blendConstants[1] = 0.0f; // Optional
	//colorBlendingInfo.blendConstants[2] = 0.0f; // Optional
	//colorBlendingInfo.blendConstants[3] = 0.0f; // Optional
	CreateColorBlendingInfo();

	/*VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

	dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.pDynamicStates = dynamicStates;*/
	CreateDynamicStateInfo();

	// pipelineLayoutInfo = {};
	// pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	// pipelineLayoutInfo.setLayoutCount = 0;
	// pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	// pipelineLayoutInfo.pushConstantRangeCount = 0; 
	// pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	/*if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout!");
	}*/
	CreatePipelineLayoutAndInfo();

	//pipelineInfo = {};
	//pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//pipelineInfo.stageCount = 2;
	//pipelineInfo.pStages = stages;
	//pipelineInfo.pVertexInputState = &vertexInputInfo;
	//pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	//pipelineInfo.pViewportState = &viewportStateInfo;
	//pipelineInfo.pRasterizationState = &rasterizerStateInfo;
	//pipelineInfo.pMultisampleState = &multisamplingStateInfo;
	//pipelineInfo.pDepthStencilState = nullptr;	//Optional
	//pipelineInfo.pColorBlendState = &colorBlendingInfo;
	//pipelineInfo.pDynamicState = nullptr;	//Optional
	//pipelineInfo.layout = pipelineLayout;
	//pipelineInfo.renderPass = renderPass;
	//pipelineInfo.subpass = 0;
	//pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	//pipelineInfo.basePipelineIndex = -1;	//Optional

	//if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
	//	throw std::runtime_error("Failed to create graphics pipeline");
	//}
	CreatePipleineAndInfo();

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);

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

	// Getting Vertex Data from struct
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributesDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributesDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributesDescriptions.data();
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
	rasterizerStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
	// Creating multiple descriptors is possible, using pools and descriptor sets *Later*
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

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

	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

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

	/*vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);*/

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
	}

	
	InitCommandBufferAndRenderPass();

	return true;
}

void VulkanInstance::InitCommandBufferAndRenderPass() {

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo commandBuffBeginInfo = {};
		commandBuffBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//commandBuffBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;	//Optional
		//commandBuffBeginInfo.pInheritanceInfo = nullptr;	//Optional

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

		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		// vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, 
			&descriptorSets[i], 0, nullptr);

		vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);	// COME BACK HEERE

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}
}

bool VulkanInstance::OnCreateSemaphoresAndFences() {

	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHTS);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHTS);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHTS);
	imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create semaphores!");
		}
	}
	

	return true;
}

void VulkanInstance::RenderFrame() {

	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	// vkResetFences(device, 1, &inFlightFences[currentFrame]);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	// When the image is out of date/ not the correct frame size, lets recreate the swap chain and resize our image
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {	
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swapchain image!");
	}

	// Checking to see if previous frame is using this index, happens when there is a fence to wait on
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	// Mark this image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	UpdateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitFlags;
	
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;	//Optional
	
	result = vkQueuePresentKHR(presentationQueue, &presentInfo);

	// In case we recreate if optimal to do so
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHTS;
	
}

void VulkanInstance::Wait() {
	vkQueueWaitIdle(presentationQueue);
}

// Intended use is to resize frame to desired width and height, Cleans Up Swap Chain, then Recreates Swap Chain
bool VulkanInstance::RecreateSwapChain() {
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	CleanUpSwapChain();

	if (!OnCreateSwapChain(width, height)) {
		Debug::FatalError("Failed to create Vulkan Swapchain", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Swapchain!");
		return false;
	}

	if (!OnCreateImageViews()) {
		Debug::FatalError("Failed to create Vulkan Image Views", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Image View!");
		return false;
	}

	if (!OnCreateRenderPassAndInfo()) {
		Debug::FatalError("Failed to create Vulkan Renderpass", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Render Pass!");
		return false;
	}

	if (!OnCreateGraphicsPipeline()) {
		Debug::FatalError("Failed to create Vulkan Graphics Pipeline", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Graphics Pipeline!");
		return false;
	}
	
	if (!OnCreateFramebuffers()) {
		Debug::FatalError("Failed to create Vulkan Frame Buffers", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Frame Buffers!");
		return false;
	}

	CreateUniformBuffers();

	CreateDescriptorPool();

	CreateDescriptorSets();

	if (!OnCreateCommandBuffers()) {
		Debug::FatalError("Failed to create Vulkan Command Buffer", __FILE__, __LINE__);
		throw std::runtime_error("Failed to create Vulkan Command Buffers!");
		return false;
	}

	return true;
}


void VulkanInstance::CleanUpSwapChain() {
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	vkDestroySwapchainKHR(device, swapChain, nullptr);


}

void VulkanInstance::CreateVertexBuffer() {

	// VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();	// Getting the size of the buffer
	VkDeviceSize bufferSize = sizeof(vertices[0]) * 4;	// For some reason std::vector<T>::size() is not returning an accurate value, hopefully this only persists here and not when I create meshes etc.
	
	// The Staging buffer affords us to opportunity to load our vertex buffer onto the GPU for better performance

	// Creating staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// Creating Buffer that be used as a source in a memory transfer operation
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	// Creating Buffer that be used as a destination in a memory transfer operation
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

}

uint32_t VulkanInstance::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

// Helper function to create a new buffer
void VulkanInstance::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
	VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;	// Indicates the purpose the buffer will be used for, use bit wise or for multiple purposes
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;		// Buffers can be owned by specific family queues

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);

}

// Helper function to copy the contents of one buffer to another
void VulkanInstance::CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size) {

	// Memory transfer operations are executed using command buffers, just lke drawing commands
		// Therefore we first allocate a temporary comand buffer

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;	// Command buffer used to perform memory transfer
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);	// IMPORTANT: Your'e not supposed to actually use an allocate command for every buffer, see Conclusion: https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	// Telling the driver about this command buffers intent, in this case one time use only to copy a buffer

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};	// Struct is used to manage information about copying buffer contents
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);	// Used to copy the contents of one buffer to another

	vkEndCommandBuffer(commandBuffer);	// We can stop recording at this step here

	// Unlike draw commands there are no events we need to wait on this time
		// We could use a VkWaitForFences or simply use a vkQueueWaitIdle, we choose the latter
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);	// Command buffer clean up
}

void VulkanInstance::CreateIndexBuffer() {
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();	// Getting the size of the buffer
	// VkDeviceSize bufferSize = sizeof(indices[0]) * 6;	// For some reason std::vector<T>::size() is not returning an accurate value, hopefully this only persists here and not when I create meshes etc.

	// The Staging buffer affords us to opportunity to load our vertex buffer onto the GPU for better performance

	// Creating staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// Creating Buffer that be used as a source in a memory transfer operation
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	// Creating Buffer that be used as a destination in a memory transfer operation
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VulkanInstance::CreateDescriptorSetLayout() {

	// Every uniform binding needs to be described using the DescriptorSetLayoutBinding Struct
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	// Flagging which shader stage this uniform will take plave
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional // Relevant for image sampling *Later*

	// We need to specify descriptor set layout during pipeline creation to tell Vulkan which descriptors the shaders will be using
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	
}

void VulkanInstance::CreateUniformBuffers() {

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	//  For swapChain image,  in other words updating uniform buffers for each frame
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void VulkanInstance::UpdateUniformBuffer(uint32_t currentImage) {

	// Below is logic to get time in seconds since rendering has started
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	// Defining the model, view, projection transformations in the uniform buffer object

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void VulkanInstance::CreateDescriptorPool() {

	// First, describe which descriptor types our descriptor sets are going to contain and how many of them
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

	// We will allocate one of these descriptors per frame
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;

	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());	// Max number of descriptors that can be allocated

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

}

void VulkanInstance::CreateDescriptorSets() {
	// Specifying the descriptor pool to allocate from

	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	// No need to free up descriptor sets, they are automatically freed when the descriptor pool is destroyed
	descriptorSets.resize(swapChainImages.size());
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		// Descriptors that refer to buffers, like our uniform buffer descriptor
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		// 
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;

		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	}

}
}