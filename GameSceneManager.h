#ifndef GAMESCENEMANAGER_H
#define GAMESCENEMANAGER_H

#include <string>
class GameSceneManager  {
public:
	
	GameSceneManager();
	~GameSceneManager();
	void Run();
	bool Init(std::string name_, int width_, int height_);
	void HandleEvents();
	
	
private:
	enum SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE2,
		SCENE3
	};
	class Scene* currentScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	Scene* BuildScene(SCENE_NUMBER scene_);
};


#endif // !GAMESCENEMANAGER_H