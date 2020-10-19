#include "Window.h"

#include <cstring>
#include <set>
#include <cstdint>
#include <algorithm>

#include "../Debug/Debug.h"





void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

Window::Window() :
	window(nullptr),
	width(1280),
	height(720)
{}

Window::~Window() {}

bool Window::OnCreate(const std::string& name, const int width_, const int height_) {

	if (!glfwInit()) {
		Debug::FatalError("Failed to init GLFW!", __FILE__, __LINE__);
		return false;
	}

	this->width = width_;
	this->height = height_;

	SetPre_Attributes();

	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	SetPost_Attributes();

	// return true;

	// AssignCallbacks();


	if (!window) {
		Debug::FatalError("Failed to create GLFW Window", __FILE__, __LINE__);
		return false;
	}

	// **----------
		// This will be potentially where I introduce a level of compiler directives which will choose the graphics library, and properly instantiate them
			// In this case we are using a vulkan instance for the graphics api so we must initialize it

	// glfwSetWindowUserPointer(window, vulkanInstance);

	return true;
}

void Window::OnDestroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
}


void Window::Render() {
	/*vulkanInstance->RenderFrame();
	vulkanInstance->Wait();*/
}

void Window::SetPre_Attributes() {
	// Window hints can be use before the window has been created to give the window properties and functionality. 
	// Once a window has been created a new window hint will not affect it/ cannot be applied after the fact
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void Window::SetPost_Attributes() {}

void Window::AssignCallbacks() {
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}


void FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
	// auto app = reinterpret_cast<VulkanInstance*>(glfwGetWindowUserPointer(window));
	// app->framebufferResized = true;
}



