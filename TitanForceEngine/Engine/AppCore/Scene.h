#ifndef SCENE_H
#define SCENE_H

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

class IScene
{


public:

	IScene() :
		m_world( new ECS::World() )
	{}

	virtual ~IScene() 
	{ 
		delete m_world; 
		m_world = nullptr; 
	}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update( const float deltaTime ) = 0;

	ECS::World*		m_world;

};

#endif // !SCENE_H
