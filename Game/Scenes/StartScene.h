#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "..//..//Engine/Core/TitanForceEngine.h"

class StartScene : public SceneInterface
{
	StartScene();
	virtual ~StartScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void Render();
};
#endif // !STARTSCENE_H


