#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

class Window {

public:

	Window();
	~Window();

	// Creates Window with the passed name and dimensions
	bool OnCreate(const std::string& name, const int width_, const int height_);
	void OnDestroy();

	GLFWwindow* GetGLFW_Window() const { return m_glfwWindow; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

private:

	GLFWwindow*		m_glfwWindow;

	int				m_width;
	int				m_height;

	void SetPre_Attributes();
	void SetPost_Attributes();


};


#endif