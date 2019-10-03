#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Used to keep track of the present queue families in device
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

// Storing information on the swapchain details
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


/*Vulkan Instance will contain all set up data and functionality associated with Vulkan setup*/
/*In order to abstract the unnecessary data accessbility inside the window class*/
/*GLFW should have been initialized before using this class*/
class VulkanInstance {

public:
	// Vulkan instance will not need the copy and move operators
	VulkanInstance(const VulkanInstance&) = delete;
	VulkanInstance& operator=(const VulkanInstance&) = delete;
	VulkanInstance(VulkanInstance&&) = delete;
	VulkanInstance& operator=(VulkanInstance&&) = delete;

	
	VulkanInstance();
	~VulkanInstance();

	bool OnCreate(int width, int height, GLFWwindow* window);
	void OnDestroy();

	VkDevice GetDevice(){ return device; }

private:
	
	// The creation of a Vulkan Instance requires you to gather the applications info, as well as,
		// the validation layers, the required extensions
	bool OnCreateVulkanInstance();
	void CreateApplicationInfo(std::string applicationName, std::string engineName);
	void CreateInstanceAndInfo();
	std::vector<const char*> GetRequiredExtensions();

	// Validation Layers do the following:
	/*Checking the values of parameters against the specification to detect misuse
	Tracking creation and destruction of objects to find resource leaks
	Checking thread safety by tracking the threads that calls originate from
	Logging every call and its parameters to the standard output
	Tracing Vulkan calls for profiling and replaying */
	bool HasValidationLayerSupport();							// Currently, only checking to see if the first validation layer exists
	uint32_t GetValidationLayerCount();
	std::vector<VkLayerProperties> GetAvailableLayerProperties();
	
	// Debug messenger prints to console informing of errors relating the instances mis-handling
	void CreateDebugMessenger();
	void CreateDebugMessengerInfo();
	
	// Callback used to create debug messages... 
	// **NEED_DEBUG**
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		// NOTE: This can be the area in which we filter some of the debug messages
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	VkResult OnCreateDebugUtilsMessengerEXT(
		VkInstance instance_,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);
	void OnDestroyDebugUtilsMessengerEXT(
		VkInstance instance_, 
		VkDebugUtilsMessengerEXT debugMessenger, 
		const VkAllocationCallbacks* pAllocator);

	/*Physical Device Set Up*/
	// The below functions will select a GPU with ability to use the Vulkan features
	bool OnSelectPhysicalDevice();
	uint32_t GetPhysicalDeviceCount();
	std::vector<VkPhysicalDevice> GetPhysicalDevices();
	bool IsDeviceSuitable(VkPhysicalDevice pDevice);
	QueueFamilyIndices GetPhysicalDeviceQueueFamilies(VkPhysicalDevice pDevice);
	std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(VkPhysicalDevice pDevice);
	uint32_t GetQueueFamilyCount(VkPhysicalDevice pDevice);
	bool DoesDeviceHaveExtensionSupport(VkPhysicalDevice pDevice);
	uint32_t GetPhysicalDeviceExtensionCount(VkPhysicalDevice pDevice);
	std::vector<VkExtensionProperties> GetPhysicalDeviceExtensions(VkPhysicalDevice pDevice);
	
	/*The Vulkan Device Set up*/
	bool OnCreateLogicalDevice();
	void CreateDeviceAndInfo();
	void CreateDeviceQueueInfo();
	void CreatePhysicalDeviceFeatures();

	bool OnCreateSurface(GLFWwindow* window);

	bool OnCreateSwapChain(int width, int height);
	void CreateSwapChainSupportDetails();
	void SelectSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
	void SelectSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);
	void SelectSwapExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChainAndInfo(uint32_t imageCount);

	bool OnCreateImageViews();
	void CreateImageView(size_t imageNum);

	bool OnCreateGraphicsPipeline();
	static std::vector<char> ReadFile(const std::string& fileName);
	VkShaderModule CreateShaderModuleAndInfo(const std::vector<char>& shaderCode);
	void CreateVertexShaderStageInfo();
	void CreateFragShaderStageInfo();
	void CreateVertexInputInfo();																	// We will be telling Vulkan how our vertex data will be passed to our vertex shader
	void CreateInputAssemblyInfo();																	// Describes what geometry will be drawn from vertices & if primitive restart should be enabled

	void CreateViewport();
	void CreateScissor();
	void CreateViewportStateInfo();
	void CreateRasterizerStateInfo();
	void CreateMultisamplingStateInfo();
	void CreateColorBlendAttachment();
	void CreateColorBlendingInfo();
	void CreateDynamicStateInfo();

	void CreatePipelineLayoutAndInfo();

	bool OnCreateRenderPassAndInfo();

	void CreatePipleineAndInfo();

	bool OnCreateFramebuffers();

	bool OnCreateCommandPool();
	bool OnCreateCommandBuffers();
	void InitCommandBufferAndRenderPass();

	bool OnCreateSemaphores();

public:

	void RenderFrame();

private:


	// Vulkan Member Variables

	VkInstance instance;
	VkApplicationInfo applicationInfo = {};
	VkInstanceCreateInfo instanceInfo = {};
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	const std::vector<const char*> pDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	
	VkDevice device;
	VkDeviceCreateInfo deviceInfo = {};
	VkDeviceQueueCreateInfo deviceQueueInfo = {};
	VkPhysicalDeviceFeatures physicalDeviceFeatures;

	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	SwapChainSupportDetails swapChainSupportDetails;
	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D extent2D;
	VkSwapchainCreateInfoKHR swapChainInfo = {};

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkImageViewCreateInfo imageViewInfo = {};

	VkShaderModule vertexShaderModule;
	VkShaderModule fragShaderModule;
	VkShaderModuleCreateInfo shaderModuleInfo = {};
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};

	VkViewport viewport = {};
	VkRect2D scissor = {};
	VkPipelineViewportStateCreateInfo viewportStateInfo = {};
	VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
	VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};

	VkPipelineLayout pipelineLayout;
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

	VkRenderPass renderPass;
	VkRenderPassCreateInfo renderPassInfo = {};

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	VkPipeline graphicsPipeline;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;


};


#endif