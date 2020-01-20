#include "StartScene.h"

StartScene::StartScene() {}

StartScene::~StartScene() {}

bool StartScene::OnCreate() {
	Debug::Info("Created StartScene", __FILE__, __LINE__);
	return true;
}

void StartScene::Update(const float deltaTime) {}

void StartScene::Render() {}