#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <optional>
#include <glm.hpp>

#include "..//Debug/Debug.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHTS = 3;

namespace test {
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

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct Vertex {

		glm::vec2 position;
		glm::vec3 colour;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		// The Attribute Description decribes how to extract a vertex attribute chunk of veretx data originating from a binding description
		// The number of attributes expected for our current state is 4: position, normal, texCoords & colour: https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description

		// Returns the Vertex Attributes for this vertex
		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributesDesctiption = {};

			attributesDesctiption[0].binding = 0;
			attributesDesctiption[0].location = 0;
			attributesDesctiption[0].format = VK_FORMAT_R32G32_SFLOAT;	// Float, vec3, vec2, etc.. have different formats
			attributesDesctiption[0].offset = offsetof(Vertex, position);


			attributesDesctiption[1].binding = 0;
			attributesDesctiption[1].location = 1;
			attributesDesctiption[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributesDesctiption[1].offset = offsetof(Vertex, colour);

			return attributesDesctiption;
		}

	};

	const std::vector<Vertex> vertices = {
		{glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)},
		{glm::vec2(0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
		{glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)},
		{glm::vec2(-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	/*Vulkan Instance will contain all set up data and functionality associated with Vulkan setup*/
	/*In order to abstract the unnecessary data accessbility inside the window class*/
	/*GLFW should have been initialized before using this class*/
	class VulkanInstance {

	public:
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = delete;

		VulkanInstance();
		~VulkanInstance();


		bool OnCreate(int width, int height, GLFWwindow* window);
		void OnDestroy();

		VkDevice GetDevice() { return device; }

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

		bool OnCreateSemaphoresAndFences();

		bool RecreateSwapChain();

		void CleanUpSwapChain();

		void CreateVertexBuffer();
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		// Helper function to create a new buffer
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		// Helper function to copy the contents of one buffer to another
		void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size);

		void CreateIndexBuffer();

		void CreateDescriptorSetLayout();

		void CreateUniformBuffers();

		void UpdateUniformBuffer(uint32_t currentImage);

		void CreateDescriptorPool();

		void CreateDescriptorSets();

	public:

		void RenderFrame();

		void Wait();

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
		float queuePriority;

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

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

	public:

		bool framebufferResized;

	private:

		GLFWwindow* window;





	};

}

#endif