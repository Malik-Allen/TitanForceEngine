#include "Engine/Core/Engine.h"

#include "Apps/TestRun/TestRun.h"

int main ( int args, char* argv[] )
{

	Engine::Get ()->Init ( 60, 1980, 1080 );

	Engine::Get()->LoadApplication( new TestRun() );

	Engine::Get ()->Run ();

	return 0;
}