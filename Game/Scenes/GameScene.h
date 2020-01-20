#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../..//Engine/Core/TitanForceEngine.h"

class GameScene : public SceneInterface
{
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void Render();
};
#endif // !GAMESCENE_H


