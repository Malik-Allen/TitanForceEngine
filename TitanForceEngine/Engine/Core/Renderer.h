#ifndef RENDERER_H
#define RENDERER_H

enum class GraphicsAPI
{
	OpenGL,
	Vulkan,
	Metal
};

class Window;
class Mesh;
// Virtual class, representing a renderer and its expected basic functionality
class Renderer
{

public:

	Renderer() {}
	virtual ~Renderer() {}

	virtual void OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) = 0;

	virtual void OnDestroy() = 0;

	virtual void Render() = 0;
	virtual void Wait() = 0;

	virtual void AddMesh( Mesh* mesh ) = 0;


};



#endif // !RENDERER_H

