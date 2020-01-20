#ifndef GAMEALPHA_H
#define GAMEALPHA_H

#include "..//..//Engine/Core/TitanForceEngine.h"

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

	void BuildScene();


};
#endif // !GAMEALPHA_H


