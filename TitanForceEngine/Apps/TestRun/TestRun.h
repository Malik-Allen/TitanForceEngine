#ifndef TESTRUN_H
#define TESTRUN_H

#include "../../Engine/Core/App.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/ECS.h"

class TestRun : public App
{

public:

	TestRun();
	~TestRun();

	virtual bool OnCreate() override final;
	virtual void OnDestroy() override final;

	virtual void Update( const float deltaTime ) override final;


private:

	ECS::World*		m_world;

};


#endif // !TESTRUN_H

