#include "TitanForceEngine.h"

std::unique_ptr<TitanForceEngine> TitanForceEngine::engineInstance(nullptr);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

TitanForceEngine::TitanForceEngine():
	window(nullptr),
	engineTimer(nullptr),
	engineProfiler(nullptr),
	isRunning(false),
	isGameRunning(false),
	fps(120)
	{}

TitanForceEngine::~TitanForceEngine(){}

bool TitanForceEngine::InitEngine(std::string name, const int initWindowWidth, const int initWindowHeight) {
	
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	window = new Window();
	if (!window->OnCreate(name, initWindowWidth, initWindowHeight)) {
		throw std::runtime_error("Failed to init engine window!");
		return false;
	}

	engineTimer = new EngineTimer();
	if (engineTimer == nullptr) {
		throw std::runtime_error("Failed to init engine timer!");
		return false;
	}

	engineProfiler = new Profiler();
	if (!engineProfiler->OnCreate()) {
		throw std::runtime_error("Failed to init engine profiler!");
		return false;
	}

	
	glfwSetKeyCallback(window->GetWindow(), KeyCallback);
	glfwSetInputMode(window->GetWindow(), GLFW_STICKY_KEYS, 1);

	Debug::Info("Everythging is fine!", __FILE__, __LINE__);

	TitanForceEngine::SetFPS(fps);

	return isRunning = true;
}

void TitanForceEngine::Run() {
	isRunning = true;

	// BEGIN_PROFILE("Hello");

	while(isRunning) {
		engineTimer->UpdateFrameTicks();
		Update(engineTimer->GetDeltaTime());
		Render();
		Sleep(engineTimer->GetSleepTime(engineTimer->GetFPS()));
		
	}

	vkDeviceWaitIdle(window->GetDevice());

	// END_PROFILE("GoodBye");
}

bool TitanForceEngine::IsRunning() const { return isRunning; }

bool TitanForceEngine::IsGameRunning() const { return isGameRunning; }

void TitanForceEngine::Stop() {
	isRunning = false;
	OnDestroy();
}

void TitanForceEngine::StopGame(){}

TitanForceEngine* TitanForceEngine::GetInstance() {
	if (engineInstance == nullptr) {
		engineInstance.reset(new TitanForceEngine);
		return engineInstance.get();
	}
	return engineInstance.get();
}

void TitanForceEngine::SetFPS(const unsigned int fps_) { engineTimer->SetFPS(fps_); }

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
}

void TitanForceEngine::Update(const float deltaTime){}

void TitanForceEngine::Render(){
	
	window->Render();

}

void TitanForceEngine::HandleEvents() {
	
	while (!glfwWindowShouldClose(window->GetWindow())) {
		glfwPollEvents();
	}
	/*GLFWkeyfun keyCallback();
	glfwSetKeyCallback(window->GetWindow, keyCallback());

	while (!glfwWindowShouldClose(window->GetWindow())) {
		glfwPollEvents();
	}*/

	/*SDL_Event sdlEvent;
	
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_EventType::SDL_QUIT:
			isRunning = false;
			break;
	
		}
	}
*/

}

void KeyCallback(GLFWwindow *window, int key, int code, int action, int mods) {
	std::cout << key << std::endl;

	if (key == GLFW_KEY_K) {

		switch (action) {
			case GLFW_PRESS:
				std::cout << "Key has been pressed!" << std::endl;
				break;

			case GLFW_REPEAT:
				std::cout << "Key is being held down!" << std::endl;
				break;

			case GLFW_RELEASE:
				std::cout << "Key has been released!" << std::endl;
				break;

			default:
				break;
		}
		
	}
	else if (key == GLFW_KEY_ESCAPE) {
		switch (action) {
		case GLFW_PRESS:
			TitanForceEngine::GetInstance()->Stop();
			std::cout << "Closing window..." << std::endl;
			break;

		default:
			break;
		}
	}
}
