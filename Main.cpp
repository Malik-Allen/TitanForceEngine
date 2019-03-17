#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

#include <string>
#include "GameSceneManager.h"
#include "Debug.h"

  
int main(int argc, char* args[]) {

	std::string name = { "Graphics Game Engine" };
	Debug::DebugInit(name + "_Log");
	Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);
	
	GameSceneManager* gsm = new GameSceneManager();
	if (gsm->Init(name, 1280, 720) ==  true) {
		gsm->Run();
	} 
	delete gsm;
	
	_CrtDumpMemoryLeaks();
	exit(0);

}