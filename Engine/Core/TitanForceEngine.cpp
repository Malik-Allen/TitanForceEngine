#include "TitanForceEngine.h"

std::unique_ptr<TitanForceEngine> TitanForceEngine::engineInstance(nullptr);

TitanForceEngine::TitanForceEngine():
	window(nullptr),
	timer(nullptr),
	isRunning(false),
	isGameRunning(false),
	fps(60)
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

	timer = new Timer();
	if (timer == nullptr) {
		throw std::runtime_error("Failed to init engine timer!");
		return false;
	}

	Debug::Info("Everythging is fine!", __FILE__, __LINE__);

	return isRunning = true;
}

void TitanForceEngine::Run() {
	timer->Start();

	isRunning = true;
	float currentTime = 0.0f;
	while (isRunning) {
		timer->UpdateFrameTicks();
		Update(timer->GetDeltaTime());
		Render();
		HandleEvents();
		currentTime += timer->GetDeltaTime();
		//printf("Current Time: %f\n", currentTime);
		Sleep(timer->GetSleepTime(timer->GetFPS()));
	}

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

void TitanForceEngine::OnDestroy() {
	if (window) {
		window->OnDestroy();
		delete window;
		window = nullptr;
	}

	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (engineInstance != nullptr) {
		engineInstance = nullptr;
	}
}

void TitanForceEngine::Update(const float deltaTime){}

void TitanForceEngine::Render(){}

void TitanForceEngine::HandleEvents() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_EventType::SDL_QUIT:
			isRunning = false;
			break;
		}
	}

}
