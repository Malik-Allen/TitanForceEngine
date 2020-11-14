#ifndef APP_H
#define APP_H

#include "Renderer.h"

class Window;

class App
{
	friend class Engine;


	App( const App& ) = delete;
	App& operator=( const App& ) = delete;
	App( App&& ) = delete;
	App& operator = ( App&& ) = delete;

public:

	explicit App( const char* appName, const GraphicsAPI& graphicsAPI ) :
		m_appName( appName ),
		m_graphicsAPI(graphicsAPI),
		m_renderer( nullptr )
	{}

	virtual ~App()
	{
		if ( m_renderer )
		{
			delete m_renderer;
			m_renderer = nullptr;
		}
	}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Update( const float deltaTime ) = 0;

	// Returns the name of this application
	const char* GetAppName() const { return m_appName; }

	const GraphicsAPI& GetGraphicsAPI() const { return m_graphicsAPI; }

private:

	const char*			m_appName;
	GraphicsAPI			m_graphicsAPI;


	bool CreateRenderer(
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window );

protected:

	Renderer*			m_renderer;

};



#endif // !APP_H

