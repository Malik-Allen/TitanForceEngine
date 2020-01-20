#ifndef WINDOW_H
#define WINDOW_H

#include "..\Vulkan\VulkanInstance.h"

// TO BE IMPLEMENTED: Graphics Wrapper Class

class Window {

public:

	Window();
	~Window();

	bool OnCreate(std::string name, const int width, const int height);	
	void OnDestroy();

	GLFWwindow* GetWindow() const;
	int GetWidth() const;
	int GetHeight() const;

	void Render();

	VkDevice GetVkDevice() { return vulkanInstance->GetDevice(); }
	

private:

	GLFWwindow* window;

	int width;
	int height;

	VulkanInstance* vulkanInstance;

	void SetPre_Attributes();
	void SetPost_Attributes();
	
};



#endif