#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vulkan.h>
#include <vector>


namespace Vulkan {

	// Storing information on the swapchain details
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	// Queues of images that are waiting to be presented to the screen
	class SwapChain {

		friend class FrameBuffers;

	public:

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		SwapChain(SwapChain&&) = delete;
		SwapChain& operator = (SwapChain&&) = delete;

		SwapChain();
		~SwapChain();


		// Creates swap chain support details based on the passed physical device, and creates swap chain image resoluations and image views for each image present in the swap chain
		void OnCreate(const VkSurfaceKHR& surface, const VkExtent2D& extent2D_, const VkDevice& device, class PhysicalDevice* physicalDevice_);
		void OnDestroy();

		// Checking if the passed physical device supports swap chains, returns the devices capapbilities
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice pDevice, VkSurfaceKHR surface);

		VkFormat GetImageFormat() const { return swapChainImageFormat; }
		VkExtent2D GetExtent() const { return swapChainExtent; }
		const std::vector<VkImage>& GetImages() const { return swapChainImages; }
		const VkSwapchainKHR& GetVkSwapChain() const { return swapChain; }
		

	private:

		VkSwapchainKHR swapChain;
		VkSurfaceKHR vkSurface;
		VkDevice vkDevice;
		PhysicalDevice* physicalDevice;


		SwapChainSupportDetails swapChainSupportDetails;
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR presentMode;
		VkExtent2D extent2D;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;



		
		void SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);	// Determines the Surface format ( colour depth )
		void SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);		// Determines conditions for 'swapping' screens
		void SelectSwapExtent(const VkExtent2D& extent2D_, const VkSurfaceCapabilitiesKHR& capabilities);	// Determines the resolutions for images in swap chain
		void CreateImageViews();	// Creates a VkImageView for every image in this swap chain
		
	};


}


#endif // !SWAPCHAIN_H
