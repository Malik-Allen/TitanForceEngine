#include "Engine/Core/TitanForceEngine.h"
#include "Game/Core/GameAlpha.h"

#include "Engine/Graphics/Renderer.h"

#include "Engine/Debug/Debug.h"


int main(int args, char* argv[]) {

	const int width = 1280;
	const int height = 720;

	TitanForceEngine::GetInstance()->SetGameInterface(new GameAlpha());

	if (!TitanForceEngine::GetInstance()->InitEngine("TitanForceEngine", width, height, RendererType::Vulkan)) {
		Debug::FatalError("Failed to create engine!", __FILE__, __LINE__);
		return 0;
	}

	TitanForceEngine::GetInstance()->Run();

	

	system("pause");
}