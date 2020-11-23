#include "Engine/Core/Engine.h"

#include "Apps/TestRun/TestRun.h"

int main( int args, char* argv[] )
{

	Engine::Get()->Init( "Titan Force Engine", 120, 1280, 720 );

	Engine::Get()->LoadApplication( new TestRun() );

	Engine::Get()->Run();

	return 0;
}