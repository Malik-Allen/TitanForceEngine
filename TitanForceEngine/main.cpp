#include "Engine/Core/Engine.h"

int main ( int args, char* argv[] )
{

	Engine::Get ()->Init ( 120, 1980, 1080 );

	Engine::Get ()->Run ();

	return 0;
}