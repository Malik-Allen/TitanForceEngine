#ifndef RENDERER_H
#define RENDERER_H

enum class RendererType
{
	Vulkan = 0
};

class Window;

// Virtual class, representing a renderer and its expected basic functionality
class Renderer {

public:

	Renderer() : isRendering(false) {}
	virtual ~Renderer() {};
	
	virtual void OnCreate(const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window) = 0;

	virtual void OnDestroy() = 0;

	virtual void Render() = 0;
	virtual void Wait() = 0;

protected:

	bool isRendering;

};



#endif // !RENDERER_H

