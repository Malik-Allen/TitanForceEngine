#include <iostream>
#include "GameManager.h"
#include "MMath.h"

int main(int argc, char* argv[]) {

	GameManager *ptr = new GameManager();
	bool status = ptr->OnCreate();
	if (status == true) {
		ptr->Run();
	}
	else if (ptr == false) {
		std::cerr << "Fatal error occured. Cannot start this program" << std::endl;
	}
	return 0;
}