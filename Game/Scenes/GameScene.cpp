#include "GameScene.h"

#include "../../Engine/Debug/Debug.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

bool GameScene::OnCreate() {
	Debug::Info("Created Game Scene", __FILE__, __LINE__);
	return true;
}

void GameScene::Update(const float deltaTime) {}

void GameScene::Render() {}