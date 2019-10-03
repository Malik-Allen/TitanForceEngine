#ifndef WINDOW_H
#define WINDOW_H

#include "..\Vulkan\VulkanInstance.h"

// TO BE IMPLEMENTED: Graphics Wrapper Class

class Window {

public:
	// The Window class will not be using the copy and move operations
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator= (Window&&) = delete;

	Window();
	~Window();

	bool OnCreate(std::string name, const int width, const int height);	// Starting point!
	void OnDestroy();

	GLFWwindow* GetWindow() const;
	int GetWidth() const;
	int GetHeight() const;

	void Render();
	VkDevice GetDevice() { return vulkanInstance->GetDevice(); }
	

private:

	GLFWwindow* window;

	int width;
	int height;

	// The window will exist seperate/abstracted from the Vulkan Instance and any other Graphics API only exchanging necessary data
	VulkanInstance* vulkanInstance;
	
};



#endif