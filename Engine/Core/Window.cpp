#include "Window.h"
#include <cstring>
#include <set>
#include <cstdint>
#include <algorithm>


Window::Window():
	window(nullptr),
	width(1280),
	height(720),
	vulkanInstance(nullptr)
	{}

Window::~Window() {}

bool Window::OnCreate(std::string name, const int width_, const int height_) {

	if (!glfwInit()) {
		Debug::FatalError("Failed to init GLFW!", __FILE__, __LINE__);
		return false;
	}

	this->width = width_;
	this->height = height_;

	SetPre_Attributes();
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	SetPost_Attributes();

	if (!window) {
		Debug::FatalError("Failed to create GLFW Window", __FILE__, __LINE__);
		return false;
	}
		
	// **----------
		// This will be potentially where I introduce a level of compiler directives which will choose the graphics library, and properly instantiate them
	vulkanInstance = new VulkanInstance();		// In this case we are using a vulkan instance for the graphics api so we must initialize it
	if (vulkanInstance == nullptr) {
		Debug::FatalError("Failed to init vulkan instance!", __FILE__, __LINE__);
		return false;
	}

	if (!vulkanInstance->OnCreate(width, height, window)) {
		Debug::FatalError("Failed to create vulakn instance", __FILE__, __LINE__);
		return false;
	}
		

	return true;
}

void Window::OnDestroy() {
	vulkanInstance->OnDestroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Window::GetWindow() const { return window; }

int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }


void Window::Render() {
	vulkanInstance->RenderFrame();
}

void Window::SetPre_Attributes() {
	// Window hints can be use before the window has been created to give the window properties and functionality. 
	// Once a window has been created a new window hint will not affect it/ cannot be applied after the fact
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void Window::SetPost_Attributes() {}
