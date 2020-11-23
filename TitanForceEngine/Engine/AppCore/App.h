#ifndef APP_H
#define APP_H

#include "../Graphics/Graphics.h"
#include "../RenderCore/Renderer.h"
#include "Scene.h"

#include <string>

class Window;

class IApp
{
	friend class Engine;

	IApp( const IApp& ) = delete;
	IApp& operator=( const IApp& ) = delete;
	IApp( IApp&& ) = delete;
	IApp& operator = ( IApp&& ) = delete;

public:

	explicit IApp( const std::string& appName );

	virtual ~IApp();

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Update( const float deltaTime ) = 0;

	// Returns the name of this application
	const std::string& GetAppName() const;


private:

	std::string			m_appName;

	// TODO:
	// RendererManager* will handle this creating its renderer through, the same location the below function
		// CreateRenderer is being called in the LoadApplicationFunction in the Engine Class
		// Using the GRAPHICS_API macros ofcourse

	// Creates the render using the graphicsAPI of this application
	bool CreateRenderer(
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window 
	);

protected:

	// TODO:
	// RenderManager* will replace renderer reference
	// SceneManager* will replace scene reference

	IScene*				m_scene;
	IRenderer*			m_renderer;

};



#endif // !APP_H

