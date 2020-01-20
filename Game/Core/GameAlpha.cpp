#include "GameAlpha.h"

GameAlpha::GameAlpha() :
	GameInterface(),
	sceneInterface(nullptr),
	currentSceneNum(0)
{}

GameAlpha::~GameAlpha() {}

bool GameAlpha::OnCreate() {
	Debug::Info("Created Game!", __FILE__, __LINE__);
	return true;
}

void GameAlpha::Update(const float deltaTime) {}

void GameAlpha::Render() {}

void GameAlpha::BuildScene() {}