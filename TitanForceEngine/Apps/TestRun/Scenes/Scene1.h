#pragma once

#include "../../../Engine/AppCore/Scene.h"

class Scene1 : public IScene
{
public:

	Scene1();
	~Scene1();

	virtual bool OnCreate() override final;
	virtual void OnDestroy() override final;
	virtual void Update( const float deltaTime ) override final;


};
