#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <memory>
#include <string>

#include "../Graphics/Renderer.h"
#include "../Graphics/Window.h"

#include "Devices/Instance.h"
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
#include "Buffers/Buffer.h"
#include "Descriptors/DescriptorPool.h"

const int MAX_FRAMES_IN_FLIGHT = 3;

namespace Vulkan {

	class VulkanRenderer : public Renderer {

	public:

		virtual void OnCreate(const char* applicationName,
			const char* engineName,
			int version,
			bool enableValidationLayers,
			Window* window_) override;

		virtual void OnDestroy() override;

		virtual void Render() override;
		virtual void Wait() override;
		

		static VulkanRenderer* GetInstance();
		bool IsRendering() const { return Renderer::isRendering; }

		LogicalDevice* GetLogicalDevice() const { return logicalDevice.get(); }
		PhysicalDevice* GetPhysicalDevice() const { return physicalDevice.get(); }
		CommandPool* GetCommandPool() const { return commandPool; }
		GraphicsPipeline* GetGraphicsPipeline() const { return graphicsPipeline; }
		DescriptorPool* GetGraphicsDescriptorPool() const { return graphicsDescriptorPool; }
		
		void BindVertexBuffers(const std::vector<Vulkan::Buffer*>& vertexBuffers);
		void BindIndexBuffer(Buffer* indexBuffer);

		// void CreateShader()

	private:

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator = (VulkanRenderer&&) = delete;

		VulkanRenderer();

		~VulkanRenderer();

		static std::unique_ptr<VulkanRenderer> graphicsInstance;
		friend std::default_delete<VulkanRenderer>;


		std::unique_ptr<Vulkan::Instance> instance;

		Window* window;	// Only one for now

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

		Vulkan::DescriptorPool* graphicsDescriptorPool;

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

		void CreateDescriptorPool();
	};

}


#endif // !VULKANRENDERER_H
