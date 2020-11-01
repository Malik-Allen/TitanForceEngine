#ifndef GAMEALPHA_H
#define GAMEALPHA_H

#include "..//..//Engine/Core/TitanForceEngine.h"
#include "../../Engine/Core/Camera.h"

#include "../../Engine/Core/Object.h"

class GameAlpha : public GameInterface
{
public:
	GameAlpha();
	virtual ~GameAlpha();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void Render();

private:

	SceneInterface* sceneInterface;
	int currentSceneNum;

	Camera* m_camera;
	DemoObject* test;

	class ObjLoader* objLoader;

	void BuildScene();


};
#endif // !GAMEALPHA_H


