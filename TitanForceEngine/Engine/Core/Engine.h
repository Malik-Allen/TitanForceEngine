#ifndef ENGINE_H
#define ENGINE_H

#include "EngineClock.h"

#include <memory>

class App;
class Window;

// Singleton Engine Class
class Engine
{

public:

	// Initialize Engine at passed fps, inits window at passed window width and height
	bool Init( 
		const char* engineName, 
		const unsigned int fps, 
		const int windowWidth, 
		const int windowHeight 
	);

	// Loads passed application into Engine, application will be executed when Engine's Run is called
	bool LoadApplication(App* app);

	// Runs current application
	void Run();

	// Returns true if engine is running
	bool IsRunning() const;

	// Returns true if current app is running
	bool IsAppRunning() const;

	// Exits Engine
	void Exit();

	// Exists App
	void ExitApp();

	// Get Instance of Engine
	static Engine* Get();

	Window* GetWindow() const { return m_window; }

private:

	// The Engine class should not be copied or moved hence removing the functionality
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator = (Engine&&) = delete;

	Engine();
	~Engine();

	static std::unique_ptr<Engine> g_engineInstance;
	friend std::default_delete<Engine>;

	void OnDestroy();
	void Update(const float deltaTime);

	const char*			m_engineName;

	EngineClock*		m_engineClock;

	bool				m_isRunning;
	bool				m_isAppRunning;

	unsigned int		m_fps;

	App*				m_app;

	Window*				m_window;
	


};

#endif