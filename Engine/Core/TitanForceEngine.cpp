#include "TitanForceEngine.h"

#include "../Vulkan/Graphics.h"

#include "../Debug/Debug.h"

std::unique_ptr<TitanForceEngine> TitanForceEngine::engineInstance(nullptr);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

TitanForceEngine::TitanForceEngine():
	engineTimer(nullptr),
	engineProfiler(nullptr),
	window(nullptr),
	gameInterface(nullptr),
	isRunning(false),
	isGameRunning(false),
	fps(120),
	currentSceneNum(0)
	{}

TitanForceEngine::~TitanForceEngine() {}

bool TitanForceEngine::InitEngine(const std::string& name, const int initWindowWidth, const int initWindowHeight) {
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	window = new Vulkan::Window();
	if (!window->OnCreate(name, initWindowWidth, initWindowHeight)) {
		Debug::FatalError("Failed to create window!", __FILE__, __LINE__);
		return false;
	}

	Vulkan::Graphics::GetInstance()->OnCreate("Prototype", name, 1, true, window, initWindowWidth, initWindowHeight);
	
	engineTimer = new EngineTimer();
	if (engineTimer == nullptr) {
		Debug::FatalError("Failed to create engine timer!", __FILE__, __LINE__);
		return false;
	}

	// Compiler directive to say whether or not initialize engine with profiler attached
	engineProfiler = new Profiler();
	if (!engineProfiler->OnCreate()) {
		Debug::FatalError("Failed to create engine profiler!", __FILE__, __LINE__);
		return false;
	}

	


	// SetUpInput();

	if (gameInterface) {
		if (!gameInterface->OnCreate()) {
			Debug::FatalError("Failed to load game!", __FILE__, __LINE__);
			return false;
		}
	}
	else {
		Debug::Info("Engine running without game.", __FILE__, __LINE__);
	}

	Debug::Info("All Core Engine Systems Have be successfully initialized!", __FILE__, __LINE__);

	TitanForceEngine::SetFPS(fps);

	return isRunning = true;
}

void TitanForceEngine::Run() {

	// BEGIN_PROFILE("Hello");

	while(isRunning) {
		engineTimer->UpdateFrameTicks();
		// Update(engineTimer->GetDeltaTime());
		// Render();
		Sleep(engineTimer->GetSleepTime(engineTimer->GetFPS()));
	}

	if (!isRunning) {
		OnDestroy();
	}

	// END_PROFILE("GoodBye");
}

bool TitanForceEngine::IsRunning() const { return isRunning; }

bool TitanForceEngine::IsGameRunning() const { return isGameRunning; }

void TitanForceEngine::Exit() { isRunning = false; }

void TitanForceEngine::ExitGame() { isGameRunning = false; }

TitanForceEngine* TitanForceEngine::GetInstance() {
	if (engineInstance == nullptr) {
		engineInstance.reset(new TitanForceEngine);
		return engineInstance.get();
	}
	return engineInstance.get();
}

void TitanForceEngine::SetFPS(const unsigned int fps_) { engineTimer->SetFPS(fps_); }

void TitanForceEngine::SetGameInterface(GameInterface* game) { gameInterface = game; }

void TitanForceEngine::OnDestroy() {

	if (window) {
		window->OnDestroy();
		delete window;
		window = nullptr;
	}

	if (engineTimer) {
		delete engineTimer;
		engineTimer = nullptr;
	}

	if (engineProfiler) {
		delete engineProfiler;
		engineProfiler = nullptr;
	}

	if (engineInstance != nullptr) {
		engineInstance = nullptr;
	}

	Debug::Info("Engine Stopped Running Successfully", __FILE__, __LINE__);
}

void TitanForceEngine::Update(const float deltaTime)
{
	if (gameInterface)
		gameInterface->Update(deltaTime);

	HandleEvents();
}

void TitanForceEngine::Render() {
	// window->Render();
}

void TitanForceEngine::SetUpInput() {
	// glfwSetKeyCallback(window->GetWindow(), KeyCallback);
	// glfwSetInputMode(window->GetWindow(), GLFW_STICKY_KEYS, 1);
}

void TitanForceEngine::HandleEvents() {
	/*if (window)
		glfwPollEvents();*/

}

void KeyCallback(GLFWwindow *window, int key, int code, int action, int mods) {
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE) {
		switch (action) {

		case GLFW_RELEASE:
			TitanForceEngine::GetInstance()->Exit();
			break;

		default:
			break;
		}
	}
}

int TitanForceEngine::GetCurrentSceneNum() const { return currentSceneNum; }

void TitanForceEngine::SetCurrentSceneNum(int sceneNum) { currentSceneNum = sceneNum; }
