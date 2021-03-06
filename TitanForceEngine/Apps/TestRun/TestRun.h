#ifndef TESTRUN_H
#define TESTRUN_H

#include "../../Engine/AppCore/App.h"

class TestRun : public IApp
{

public:

	TestRun();
	~TestRun();

	virtual bool OnCreate() override final;
	virtual void OnDestroy() override final;

	virtual void Update( const float deltaTime ) override final;


private:


};


#endif // !TESTRUN_H

