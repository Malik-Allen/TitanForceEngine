#include "SandBox.h"

SandBox::SandBox(SDL_Window* sdlWindow, SDL_Renderer *sdlRenderer) : worldObjects(new GameObjList), actor(new BodyDefinition)
{
	window = sdlWindow;
	renderer = sdlRenderer;
}

SandBox::~SandBox()
{
}

bool SandBox::OnCreate() {

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	projectionMatrix = MMath::viewportNDC(w, h) * MMath::orthographic(-45.0f, 45.0f, -45.0f, 45.0f, 0.0f, 1.0f);

	dummy = new GameObject("TestDummy", actor);
	van = new GameObject("Vehicle", actor);

	starship = new GameObject("SpaceShip", actor);

	worldObjects->AddGameObj(*van);
	worldObjects->AddGameObj(*dummy);

	worldObjects->AddGameObj(*starship);
	
	starship->body->SetType(dynamicBody);
	starship->body->SetMass(1e6);
	starship->body->SetRotationalInertia(2.67e10);
	starship->body->m_position.Load(-45.0f, 45.0f, 0.0f);

	torque = 6.291e8;

	dummy->body->SetType(kinematicBody);
	dummy->body->SetMass(1.0f);
	dummy->body->m_position.Load(10.0f, 15.0f, 0.0f);

	van->body->m_position.Load(25.0f, 10.0f, 0.0f);

	dummy->FindSprite("Brick-BrownWhite.png");
	van->FindSprite("Grass.png");
	starship->FindSprite("Brick-BrownWhite.png");

	vector1.Load(5e7, 0.0f, 0.0f);
	vector2.Load(0.0f, -1.0f, 0.0f);

	if (!worldObjects->LoadTextures(renderer)) {
		return false;
	}

	return true;
}

void SandBox::OnDestroy(){
	if (window) delete window;
	if (worldObjects) delete worldObjects;
	if (actor) delete actor;
	if (dummy) delete dummy;
}

void SandBox::Update(const float time){
	
	timePassed += time;
	worldObjects->Update(time);

	starship->body->ApplyForce(vector1);

	if (timePassed >= 30.0f && timePassed < 51.0f)
		starship->body->ApplyTorque(6.2e10);
	else if (timePassed > 51.0f)
		vector1.Load(0.0f, 0.0f, 0.0f);


	//dummy->m_velocity.RotateZ(45);
	
	dummy->body->ApplyTorque(-10.0f);
	///printf("%f", dummy->body->m_angle);
	///printf("%f \n",dummy->body->m_position.x);
	
}

void SandBox::Render() {

	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

	worldObjects->Render(renderer, screenSurface, projectionMatrix);
}

void SandBox::InputManagement(const SDL_Event &sdlEvent) {

	switch (sdlEvent.type) {

		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {

			case SDLK_k:
				worldObjects->RemoveGameObj(dummy->gameObjectName);
				break;

			case SDLK_a:
				dummy->body->m_velocity.Load(-10.0f, 0.0f, 0.0f);
				break;

			case SDLK_s:
				dummy->body->m_velocity.Load(0.0f, -10.0f, 0.0f);
				break;

			case SDLK_d:
				dummy->body->m_velocity.Load(10.0f, 0.0f, 0.0f);
				break;

			case SDLK_w:
				dummy->body->m_velocity.Load(0.0f, 10.0f, 0.0f);
				break;

			case SDLK_o:
				dummy->body->m_type = kinematicBody;
				break;

			case SDLK_p:
				dummy->body->m_type = staticBody;
				break;

			case SDLK_l:
				dummy->body->m_type = dynamicBody;
				break;

			}

			break;

		case SDL_KEYUP:
			switch (sdlEvent.key.keysym.sym) {

			}

			break;
		
	}

	
}
