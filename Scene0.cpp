#include "Scene0.h"
#include <SDL.h>
Scene0::Scene0(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	m_jetski->SetType(dynamicBody);
	m_boat->SetType(dynamicBody);
	
};

Scene0::~Scene0() {
}

bool Scene0::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(0.0f, 14.0f, 0.0f, 7.0f, 0.0f, 1.0f);

	m_boat->m_position.Load(0.0f, 1.5f, 0.0f);
	m_boat->SetMass(400.0f);

	m_jetski->m_position.Load(0.0f, 1.5f, 0.0f);
	m_jetski->SetMass(200.0f);

	worldObjects->AddBody(*m_jetski);
	worldObjects->AddBody(*m_boat);
	worldObjects->AddBody(*m_tree);

	m_jetski->FindSprite("Pile_of_Coins.png");
	m_boat->FindSprite("ball.bmp");
	m_tree->FindSprite("ball.bmp");

	if (!worldObjects->CheckSprites()) {
		return false;
	}

	m_force.Load(3000.0f, 0.0f, 0.0f);
	m_force2.Load(3000.0f, 0.0f, 0.0f);
	return true;
}

void Scene0::OnDestroy() {}

void Scene0::Update(const float time) {
	
	timePassed += time;
	m_jetski->ApplyForce(m_force);
	m_boat->ApplyForce(m_force2);
	worldObjects->Update(time);
	
	if (m_jetski->m_position.x <= 3.0f) {
		m_jetski->m_velocity.RotateZ(30.0f);
	}
	else {
		m_force.Load(0.0f, 0.0f, 0.0f);
	}

	if (m_boat->m_position.x <= 3.0f) {
		m_boat->m_velocity.RotateZ(30.0f);
	}
	else {
		m_force2.Load(0.0f, 0.0f, 0.0f);
	}

	//if (timePassed >= 1.0f)
		//m_force.Load(0.0f, 0.0f, 0.0f);

}

void Scene0::Render() {

	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

	worldObjects->Render(screenSurface, projectionMatrix);
	
	SDL_UpdateWindowSurface(window);
}
