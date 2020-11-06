#ifndef APP_H
#define APP_H

class App
{

	App(const App&) = delete;
	App& operator=(const App&) = delete;
	App(App&&) = delete;
	App& operator = (App&&) = delete;

public:

	explicit App(const char* appName) : m_appName(appName) {}
	virtual ~App() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Update(const float deltaTime) = 0;

	// Returns the name of this application
	const char* GetAppName() const { return m_appName; }

private:

	const char*			m_appName;

};



#endif // !APP_H

