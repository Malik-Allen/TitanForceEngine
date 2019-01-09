#include "PrototypeMenuNavigation.h"

PrototypeMenuNavigation::PrototypeMenuNavigation(SDL_Window* sdlWindow)
{
	window1 = sdlWindow;
	currentInput = new InputEvent;

	hit->SetType(dynamicBody);
}


PrototypeMenuNavigation::~PrototypeMenuNavigation()
{
}

bool PrototypeMenuNavigation::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window1, &w, &h);
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(-10.0f, 30.0f, 0.0f, 30.0f, 0.0f, 1.0f);

	hit->SetMass(200.0f);
	hit->m_position.Load(10.0f, 15.0f, 0.0f);
	hit->m_velocity.Load(1.0f, 1.0f, 0.0f);

	hit->FindSprite("Brick-BrownWhite.png");

	world1->AddBody(*hit);

	hit->GiveName("Hit-Sama");

	if (!world1->CheckSprites()) {
		printf("Missing Sprite!");
		return false;
	}

	tempVec.Load(0.0f, 0.0f, 0.0f);
	return true;
}

void PrototypeMenuNavigation::OnDestroy(){}

void PrototypeMenuNavigation::Update(const float time) {
	InputManagement(currentInput);
	world1->Update(time);

	hit->ApplyForce(tempVec);

	printf("PosX: %f\t PosY: %f\n", hit->m_position.x, hit->m_position.y);
}

void PrototypeMenuNavigation::Render() {
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window1);
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

	world1->Render(screenSurface, projectionMatrix);

	SDL_UpdateWindowSurface(window1);
}

void PrototypeMenuNavigation::InputManagement(InputEvent* inputEvent) {

	if (*inputEvent == KEY_PRESS_0) {
		hit->FindSprite("Grass.png");
		world1->CheckSprites();
	}
	if (*inputEvent == KEY_PRESS_1) {
		hit->FindSprite("Brick-BrownWhite.png");
		world1->CheckSprites();
	}
}
