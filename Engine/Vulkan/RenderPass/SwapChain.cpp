#include "SwapChain.h"

#include <algorithm>

#include "../../Debug/Debug.h"

#include "../Devices/PhysicalDevice.h"

namespace Vulkan {

	SwapChain::SwapChain() :
		swapChain(VK_NULL_HANDLE),
		vkSurface(VK_NULL_HANDLE),
		vkDevice(VK_NULL_HANDLE),
		physicalDevice(nullptr)
	{}

	SwapChain::~SwapChain() {}


	// Creates swap chain support details based on the passed physical device, and creates swap chain image resoluations and image views for each image present in the swap chain
	void SwapChain::OnCreate(const VkSurfaceKHR& surface, const VkExtent2D& extent2D_, const VkDevice& device, class PhysicalDevice* physicalDevice_) {
		vkSurface = surface;
		vkDevice = device;
		physicalDevice = physicalDevice_;

		if (vkSurface == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null surface passed to swap chain!");
		}

		if (vkDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null logical device passed to swap chain!");
		}

		if (physicalDevice == nullptr) {
			throw std::runtime_error("Error! Null physical device passed to swap chain!");
		}


		// Getting our swap chain support info
		swapChainSupportDetails = QuerySwapChainSupport(physicalDevice->GetVkPhysicalDevice(), physicalDevice->GetVkSurface());


		// Determining swap chain formats
		SelectSwapSurfaceFormat(swapChainSupportDetails.formats);
		SelectSwapPresentMode(swapChainSupportDetails.presentModes);
		SelectSwapExtent(extent2D_, swapChainSupportDetails.capabilities);

		// NOTE: It is recommended that we use at least one more swap chain than the minimum because we want to avoid a situatuion where the driver has to wait for the minimum
		uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount + 1;
		
		if (swapChainSupportDetails.capabilities.minImageCount > 0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount) {
			imageCount = swapChainSupportDetails.capabilities.maxImageCount;
		}



		VkSwapchainCreateInfoKHR swapChainInfo = {};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface = vkSurface;

		// Swap chain image details
		swapChainInfo.minImageCount = imageCount;
		swapChainInfo.imageFormat = surfaceFormat.format;
		swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapChainInfo.imageExtent = extent2D;
		swapChainInfo.imageArrayLayers = 1;

		//NOTE: 'Image Usuage' specifies the operations you will use the image in the swap chain for. For 'Post-process' operations you will pass 'VK_IMAGE_USAGE_TRANSFER_DST_BIT' instead
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = physicalDevice->GetPhysicalDeviceQueueFamilies(physicalDevice->GetVkPhysicalDevice());
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;	// An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. This option offers the best performance.
			swapChainInfo.queueFamilyIndexCount = 2;
			swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;		// Images can be used across multiple queue families without explicit ownership transfers.
			swapChainInfo.queueFamilyIndexCount = 0;	//optional
			swapChainInfo.pQueueFamilyIndices = nullptr;	//optional
		}

		swapChainInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
		swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainInfo.presentMode = presentMode;
		swapChainInfo.clipped = VK_TRUE;
		swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(vkDevice, &swapChainInfo, nullptr, &swapChain) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create SwapChain!");
		}

		vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, swapChainImages.data());
		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent2D;

		CreateImageViews();

	}

	void SwapChain::OnDestroy() {

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			vkDestroyImageView(vkDevice, swapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(vkDevice, swapChain, nullptr);

	}

	// Checking if the passed physical device supports swap chains, returns the devices capapbilities
	SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkPhysicalDevice pDevice, VkSurfaceKHR surface) {

		SwapChainSupportDetails details;	

		// Getting device and surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &details.capabilities);

		// Getting Format count
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface, &formatCount, details.formats.data());
		}

		// Getting present count
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface, &presentModeCount, details.presentModes.data());
		}

		return details;

	}

	// Determines the Surface format ( colour depth )
	void SwapChain::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

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

	// Determines conditions for 'swapping' screens
	void SwapChain::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	
		bool foundMode = false;
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				presentMode = availablePresentMode;
				foundMode = true;
			}
		}
		if (foundMode == false)
			presentMode = VK_PRESENT_MODE_FIFO_KHR;

	}

	// Determines the resolutions for images in swap chain
	void SwapChain::SelectSwapExtent(const VkExtent2D& extent2D_, const VkSurfaceCapabilitiesKHR& capabilities) {
		
		if (capabilities.currentExtent.width != UINT32_MAX) {
			extent2D = capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { extent2D_.width, extent2D_.height };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			extent2D = actualExtent;
		}

	}

	// Creates a VkImageView for every image in this swap chain
	void SwapChain::CreateImageViews() {

		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++) {

			VkImageViewCreateInfo imageViewInfo = {};

			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image = swapChainImages[i];
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

			if (vkCreateImageView(vkDevice, &imageViewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create image views!");
			}
		}

	}


}