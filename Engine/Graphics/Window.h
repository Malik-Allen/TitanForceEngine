#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>




class Window {

public:

	Window();
	~Window();

	bool OnCreate(const std::string& name, const int width_, const int height_);
	void OnDestroy();

	GLFWwindow* GetWindow() const { return window; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	void Render();



private:

	GLFWwindow* window;

	int width;
	int height;

		

	void SetPre_Attributes();
	void SetPost_Attributes();

private:

	// Callbacks
	void AssignCallbacks();


};







#endif