#include "Graphics.h"

#include "../Rendering/3D/Mesh.h"

namespace Vulkan {

	std::unique_ptr<Graphics> Graphics::graphicsInstance(nullptr);

	Graphics* Graphics::GetInstance() {
		if (graphicsInstance == nullptr) {
			graphicsInstance.reset(new Graphics);
			return graphicsInstance.get();
		}
		return graphicsInstance.get();
	}


	Graphics::Graphics():
		instance(nullptr),
		window(nullptr),
		surface(nullptr),
		physicalDevice(nullptr),
		logicalDevice(nullptr),
		swapChain(nullptr),
		renderPass(nullptr),
		vertexShader(nullptr),
		fragmentShader(nullptr),
		graphicsPipeline(nullptr),
		frameBuffers(nullptr),
		commandPool(nullptr),
		currentFrame(0),
		framebufferResized(false),
		graphicsDescriptorPool(nullptr),
		isRendering(false),
		initWidth(1280),
		initHeight(720),
		name("")
	{}


	Graphics::~Graphics() {}


	void Graphics::OnCreate(const std::string& applicationName,
		const std::string& engineName,
		int version,
		bool enableValidationLayers,
		Vulkan::Window* window_,
		const uint32_t windowWidth, const uint32_t windowHeight) {


		CreateVulkanInstance(applicationName, engineName, version, enableValidationLayers);

		window = window_;

		CreateSurface();

		CreatePhysicalDevice();

		CreateLogicalDevice();
		
		CreateSwapChain();

		CreateRenderPass();

		CreateGraphicsPipeline();

		CreateFrameBuffers();

		CreateCommandPool();

		CreateDescriptorPool();		

		CreateCommandBuffers();

		CreateSemaphoresAndFences();

		isRendering = true;
		
	}


	void Graphics::OnDestroy() {

		isRendering = false;

		if (frameBuffers) {
			frameBuffers->OnDestroy();
			delete frameBuffers;
			frameBuffers = nullptr;
		}

		if (commandPool) {
			commandPool->OnDestroy();
			delete commandPool;
			commandPool = nullptr;
		}

		if (swapChain) {
			swapChain->OnDestroy();
			delete swapChain;
			swapChain = nullptr;
		}

		if (surface) {
			surface->OnDestroy();
			delete surface;
			surface = nullptr;
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(logicalDevice->GetVkDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice->GetVkDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(logicalDevice->GetVkDevice(), inFlightFences[i], nullptr);
		}

		if (renderPass) {
			renderPass->OnDestroy();
			delete renderPass;
			renderPass = nullptr;
		}

		if (vertexShader) {
			vertexShader->OnDestroy();
			delete vertexShader;
			vertexShader = nullptr;
		}

		if (fragmentShader) {
			fragmentShader->OnDestroy();
			delete fragmentShader;
			fragmentShader = nullptr;
		}

		if (graphicsPipeline) {
			graphicsPipeline->OnDestroy();
			delete graphicsPipeline;
			graphicsPipeline = nullptr;
		}

		for (auto commandBuffer : commandBuffers) {
			commandBuffer->OnDestroy();
		}

		if (graphicsDescriptorPool) {
			graphicsDescriptorPool->OnDestroy();
			delete graphicsDescriptorPool;
			graphicsDescriptorPool = nullptr;
		}


		logicalDevice->OnDestroy();
		instance->OnDestroy();
	}
	

	void Graphics::Render() {

		auto device = logicalDevice->GetVkDevice();

		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		// vkResetFences(device, 1, &inFlightFences[currentFrame]);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(device, swapChain->GetVkSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

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

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitFlags;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex]->GetVkCommandBuffer();

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(logicalDevice->GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain->GetVkSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr;	//Optional

		result = vkQueuePresentKHR(logicalDevice->GetPresentationQueue(), &presentInfo);

		// In case we recreate if optimal to do so
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	}


	void Graphics::WaitIdle() {
		vkQueueWaitIdle(logicalDevice->GetPresentationQueue());
	}



	void Graphics::BindVertexBuffers(const std::vector<Vulkan::Buffer*>& vertexBuffers) {
		
		for (int i = 0; i < commandBuffers.size(); ++i) {

			commandBuffers[i]->BindVertexBuffers(vertexBuffers);

		}

	}


	void Graphics::BindIndexBuffer(Vulkan::Buffer* indexBuffer) {
		for (int i = 0; i < commandBuffers.size(); ++i) {

			commandBuffers[i]->BindIndexBuffer(indexBuffer);

		}
	}


	void Graphics::CreateVulkanInstance(const std::string& appName, const std::string& engineName, int version, bool enableValidationLayers) {
		
		instance.reset(new Vulkan::Instance(appName, engineName, version, enableValidationLayers));
	
	}


	void Graphics::CreateSurface() {
	
		if(surface == nullptr)
			surface = new Vulkan::Surface(window->GetWindow(), instance.get()->GetVkInstance());
		else {
			surface->OnDestroy();
			surface->OnCreate(window->GetWindow(), instance.get()->GetVkInstance());
		}
	
	}
	

	void Graphics::CreatePhysicalDevice() {
	
		physicalDevice.reset(new Vulkan::PhysicalDevice(surface->GetVKSurface(), instance.get()->GetVkInstance()));

	}


	void Graphics::CreateLogicalDevice() {
	
		logicalDevice.reset(new Vulkan::LogicalDevice(physicalDevice.get(), instance.get()));

	}


	void Graphics::CreateSwapChain() {
	
		if (swapChain == nullptr) {
			swapChain = new Vulkan::SwapChain();
		}
		else {
			swapChain->OnDestroy();
		}
		
		swapChain->OnCreate(surface->GetVKSurface(), { static_cast<uint32_t>(window->GetWidth()), static_cast<uint32_t>(window->GetHeight()) }, 
			logicalDevice->GetVkDevice(), physicalDevice.get());
		
	}


	void Graphics::CreateRenderPass() {


		if(renderPass == nullptr)
			renderPass = new Vulkan::RenderPass(logicalDevice->GetVkDevice(), swapChain->GetImageFormat());
		else {
			
			renderPass->OnCreate(logicalDevice->GetVkDevice(), swapChain->GetImageFormat());
		}
	
	}

	// TODO:
		// Create Helper Function to create Shader Modules, then load them into a map, where the key is the shader's name and the value is the ShaderObj
		// Loop through the Shader Stages to add to the graphics pipeline
	void Graphics::CreateGraphicsPipeline() {
	
		if(vertexShader == nullptr)
			vertexShader = new Vulkan::Shader(logicalDevice->GetVkDevice(), VK_SHADER_STAGE_VERTEX_BIT, "vertex", "C:/TitanForceEngineLibrary/TitanForceEngine/TitanForceEngine/Engine/Shaders/vert.spv", "main");

		if(fragmentShader == nullptr)
			fragmentShader = new Vulkan::Shader(logicalDevice->GetVkDevice(), VK_SHADER_STAGE_FRAGMENT_BIT, "fragment", "C:/TitanForceEngineLibrary/TitanForceEngine/TitanForceEngine/Engine/Shaders/frag.spv", "main");


		std::vector<VkPipelineShaderStageCreateInfo> stages;
		stages.push_back(vertexShader->GetStageInfo());
		stages.push_back(fragmentShader->GetStageInfo());

		std::vector<VkVertexInputBindingDescription> bindingDescriptions;
		bindingDescriptions.push_back(Vertex::GetBindingDescription());

		auto attributesDescriptions = Vertex::GetAttributeDescriptions();

		graphicsPipeline = new Vulkan::GraphicsPipeline(logicalDevice->GetVkDevice(),
			renderPass->GetRenderPass(),
			stages,
			bindingDescriptions,
			attributesDescriptions,
			initWidth, initHeight,
			name);
	
	}


	void Graphics::CreateFrameBuffers() {
	
		if (frameBuffers == nullptr)
			frameBuffers = new Vulkan::FrameBuffers(logicalDevice->GetVkDevice(), swapChain, renderPass->GetRenderPass());
		else
			frameBuffers->OnCreate(logicalDevice->GetVkDevice(), swapChain, renderPass->GetRenderPass());
	}


	void Graphics::CreateCommandPool() {
	
		commandPool = new Vulkan::CommandPool(logicalDevice->GetVkDevice(), physicalDevice.get());
	
	}


	void Graphics::CreateCommandBuffers() {
	
		commandBuffers.resize(frameBuffers->GetVkFrameBuffers().size());

		for (int i = 0; i < commandBuffers.size(); i++) {

			if(commandBuffers[i] == nullptr)
				commandBuffers[i] = new Vulkan::CommandBuffer();

			commandBuffers[i]->OnCreate(logicalDevice->GetVkDevice(), commandPool->GetVkCommandPool(), renderPass->GetRenderPass());

			commandBuffers[i]->Begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.renderPass = renderPass->GetRenderPass();
			renderPassBeginInfo.framebuffer = frameBuffers->GetVkFrameBuffers()[i];		//Carefull now!
			renderPassBeginInfo.renderArea.offset = { 0, 0 };
			renderPassBeginInfo.renderArea.extent = swapChain->GetExtent();

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassBeginInfo.clearValueCount = 1;
			renderPassBeginInfo.pClearValues = &clearColor;

			commandBuffers[i]->BeginRenderPass(renderPassBeginInfo);

			commandBuffers[i]->BindPipeline(graphicsPipeline->GetVkPipeline());

			// commandBuffers[i]->BindVertexBuffers();

			// commandBuffers[i]->BindIndexBuffer();

			// commandBuffers[i]->BindDescriptorSets();

			// commandBuffers[i]->DrawIndexed();

			commandBuffers[i]->EndRenderPass();

			commandBuffers[i]->End();

		}
	
	}


	void Graphics::CreateSemaphoresAndFences() {
	
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(swapChain->GetImages().size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(logicalDevice->GetVkDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(logicalDevice->GetVkDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(logicalDevice->GetVkDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create semaphores!");
			}
		}
	
	}


	void Graphics::RecreateSwapChain() {

		int width = 0, height = 0;
		glfwGetFramebufferSize(window->GetWindow(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window->GetWindow(), &width, &height);
			glfwWaitEvents();
		}

		auto device = logicalDevice->GetVkDevice();
		vkDeviceWaitIdle(device);

		CleanUpSwapChain();

		CreateSwapChain();

		CreateRenderPass();

		CreateGraphicsPipeline();

		CreateFrameBuffers();

		// CreateUniformBuffers();

		CreateDescriptorPool();

		// CreateDescriptorSets();


		CreateCommandBuffers();

	}


	void Graphics::CleanUpSwapChain() {

		if (frameBuffers) {
			frameBuffers->OnDestroy();
		}

		if (swapChain) {
			swapChain->OnDestroy();
		}

		if (renderPass) {
			renderPass->OnDestroy();
		}

		if (graphicsPipeline) {
			graphicsPipeline->OnDestroy();
		}

		for (auto commandBuffer : commandBuffers) {
			commandBuffer->OnDestroy();
		}

		// Uniform buffers too

	}

	void Graphics::CreateDescriptorPool() {

		if (graphicsDescriptorPool == nullptr) {

			graphicsDescriptorPool = new Vulkan::DescriptorPool(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);

		}
		else {

			graphicsDescriptorPool->OnCreate(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);

		}

	}


}