#include "Surface.h"
#include <iostream>

Vulkan::Surface::Surface(GLFWwindow* window, const VkInstance& instance) :
	vkSurface(VK_NULL_HANDLE),
	vkInstance(instance)
{
	OnCreate(window, instance);
}

Vulkan::Surface::~Surface() {}

// Creates link to GLFW window and VkSurfaceKHR
void Vulkan::Surface::OnCreate(GLFWwindow* window, const VkInstance& instance) {

	vkInstance = instance;
	if (vkInstance == VK_NULL_HANDLE) {
		throw std::runtime_error("Error! Null Instance passed to Surface!");
	}

	if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create Surface!");
	}

}

void Vulkan::Surface::OnDestroy() {

	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);

}
