#ifndef SURFACE_H
#define SURFACE_H

#include <vulkan.h>
#include <GLFW/glfw3.h>

namespace Vulkan {

	// Connnection to Window
	class Surface {

	public:

		Surface(const Surface&) = delete;
		Surface& operator=(const Surface&) = delete;
		Surface(Surface&&) = delete;
		Surface& operator = (Surface&&) = delete;

		Surface(GLFWwindow* window, const VkInstance& instance);
		~Surface();

		// Creates link to GLFW window and VkSurfaceKHR
		void OnCreate(GLFWwindow* window, const VkInstance& instance);
		void OnDestroy();

		VkSurfaceKHR GetVKSurface() const { return vkSurface; }


	private:

		

		VkSurfaceKHR vkSurface;
		VkInstance vkInstance;

	};


}


#endif // !SURFACE_H
