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

bool Window::OnCreate(std::string name, const int newWidth, const int newHeight) {

	if (!glfwInit()) {
		return false;
	}

	this->width = newWidth;
	this->height = newHeight;

	// Window hints can be use before the window has been created to give the window properties and functionality. 
	// Once a window has been created a new window hint will not affect it/ cannot be applied after the fact
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (!window)
		return false;

	vulkanInstance = new VulkanInstance();		// In this case we are using a vulkan instance for the graphics api so we must initialize it

	if (vulkanInstance == nullptr) {
		return false;
	}

	if (!vulkanInstance->OnCreate(width, height, window))
		return false;

	return true;

}

void Window::OnDestroy() {
	vulkanInstance->OnDestroy();
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;
}

GLFWwindow* Window::GetWindow() const { return window; }

int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }


void Window::Render() {
	vulkanInstance->RenderFrame();
}