#ifndef RENDERER_H
#define RENDERER_H

#include "../AppCore/Scene.h"

class Window;
class IScene;
class Model;
class CameraComponent;

// Virtual class, representing a renderer and its expected basic functionality
class IRenderer
{

public:

	IRenderer() :
		m_window( nullptr ),
		m_camera( nullptr )
	{}

	virtual ~IRenderer() {}

	// Initializes Renderer
	virtual bool OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) = 0;
	virtual void OnDestroy() = 0;

	virtual void RenderScene( IScene* scene ) = 0;

protected:

	Window*				m_window;
	CameraComponent*	m_camera;

	virtual void BeginScene( IScene* scene ) = 0;
	virtual void EndScene() = 0;

	virtual void Begin() = 0;
	virtual void Present() = 0;
	virtual void End() = 0;
	
	virtual void SubmitModel( Model* model ) = 0;


	// TODO:
	// Replace this model array with a queue of commands, these command will be created inside of the BeginScene functionality
	std::vector<Model*> m_models;
};



#endif // !RENDERER_H

