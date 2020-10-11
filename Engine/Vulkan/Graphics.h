#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <string>

#include "Devices/Instance.h"
#include "Devices/Window.h"
#include "Devices/Surface.h"
#include "Devices/PhysicalDevice.h"
#include "Devices/LogicalDevice.h"
#include "RenderPass/SwapChain.h"
#include "RenderPass/RenderPass.h"
#include "Pipeline/Shader.h"
#include "Pipeline/GraphicsPipeline.h"
#include "RenderPass/FrameBuffers.h"
#include "Commands/CommandPool.h"
#include "Commands/CommandBuffer.h"

const int MAX_FRAMES_IN_FLIGHT = 3;

namespace Vulkan {

	class Graphics {

	public:

		void OnCreate(const std::string& applicationName,
			const std::string& engineName,
			int version,
			bool enableValidationLayers,
			Vulkan::Window* window_,
			const uint32_t windowWidth, const uint32_t windowHeight);

		void OnDestroy();

		static Graphics* GetInstance();

		void Render();
		void WaitIdle();

		LogicalDevice* GetLogicalDevice() const { return logicalDevice.get(); }
		PhysicalDevice* GetPhysicalDevice() const { return physicalDevice.get(); }
		CommandPool* GetCommandPool() const { return commandPool; }


		// void CreateShader()

	private:

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		Graphics(Graphics&&) = delete;
		Graphics& operator = (Graphics&&) = delete;

		Graphics();

		~Graphics();

		static std::unique_ptr<Graphics> graphicsInstance;
		friend std::default_delete<Graphics>;


		std::unique_ptr<Vulkan::Instance> instance;

		Vulkan::Window* window;	// Only one for now

		Vulkan::Surface* surface;	// We can settle for one surface for now

		std::unique_ptr<Vulkan::PhysicalDevice> physicalDevice;

		std::unique_ptr<Vulkan::LogicalDevice> logicalDevice;

		Vulkan::SwapChain* swapChain;

		Vulkan::RenderPass* renderPass;

		Vulkan::Shader* vertexShader;
		Vulkan::Shader* fragmentShader;

		Vulkan::GraphicsPipeline* graphicsPipeline;

		Vulkan::FrameBuffers* frameBuffers;	

		Vulkan::CommandPool* commandPool; // One for now

		std::vector<Vulkan::CommandBuffer*> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame;
		bool framebufferResized;

		const uint32_t initWidth;
		const uint32_t initHeight;
		const std::string name;

		void CreateVulkanInstance(const std::string& appName, const std::string& engineName, int version, bool enableValidationLayers);
		void CreateSurface();
		void CreatePhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFrameBuffers();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSemaphoresAndFences();
		
		void RecreateSwapChain();
		void CleanUpSwapChain();
	};

}


#endif // !GRAPHICS_H
