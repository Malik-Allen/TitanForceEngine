#include <glew.h>
#include <iostream>
#include "Window.h"
#include "Debug.h"
#include "Scene0.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"

Scene0::Scene0():
	camera(nullptr), earth(nullptr),meshPtr(nullptr),shaderPtr(nullptr),earthTexture(nullptr), 
	moonTexture(nullptr), moon(nullptr), skyBox(nullptr), sun(nullptr), sunTexture(nullptr),
	mercury(nullptr), mercuryTexture(nullptr), venus(nullptr), venusTexture(nullptr),
	mars(nullptr), marsTexture(nullptr), jupiter(nullptr), jupiterTexture(nullptr) ,
	saturn(nullptr), saturnTexture(nullptr), uranus(nullptr), uranusTexture(nullptr), 
	neptune(nullptr), neptuneTexture(nullptr), pluto(nullptr), plutoTexture(nullptr){}

Scene0::~Scene0() {}

bool Scene0::OnCreate() {
	camera = new Camera();
	skyBox = new SkyBox();

	if (skyBox->OnCreate())
		skyBox->LoadSkyBox("posX1.jpg", "posY1.jpg", "posZ1.jpg", "negX1.jpg", "negY1.jpg", "negZ1.jpg");

	lightSource = Vec3(0.0f, -25.0f, 1.0f);

	if (ObjLoader::loadOBJ("sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("textureVert.glsl", "textureFrag.glsl");

	if (meshPtr == nullptr || shaderPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	sunTexture = new Texture();
	mercuryTexture = new Texture();
	earthTexture = new Texture();
	moonTexture = new Texture();
	venusTexture = new Texture();
	marsTexture = new Texture();
	jupiterTexture = new Texture();
	saturnTexture = new Texture();
	uranusTexture = new Texture();
	neptuneTexture = new Texture();
	plutoTexture = new Texture();

	
	if (sunTexture->LoadImage("sunTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (mercuryTexture->LoadImage("mercuryTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (venusTexture->LoadImage("mercuryTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (earthTexture->LoadImage("earthTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (moonTexture->LoadImage("moonTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (marsTexture->LoadImage("marsTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (jupiterTexture->LoadImage("jupiterTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (saturnTexture->LoadImage("saturnTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (uranusTexture->LoadImage("uranusTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (neptuneTexture->LoadImage("neptuneTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (plutoTexture->LoadImage("plutoTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}


	sun = new GameObject(meshPtr, shaderPtr, sunTexture);
	earth = new GameObject(meshPtr, shaderPtr, earthTexture);
	moon = new GameObject(meshPtr, shaderPtr, moonTexture);
	mercury = new GameObject(meshPtr, shaderPtr, mercuryTexture);
	venus = new GameObject(meshPtr, shaderPtr, venusTexture);
	mars = new GameObject(meshPtr, shaderPtr, marsTexture);
	jupiter = new GameObject(meshPtr, shaderPtr, jupiterTexture);
	saturn = new GameObject(meshPtr, shaderPtr, saturnTexture);
	uranus = new GameObject(meshPtr, shaderPtr, uranusTexture);
	neptune = new GameObject(meshPtr, shaderPtr, neptuneTexture);
	pluto = new GameObject(meshPtr, shaderPtr, plutoTexture);

	if (earth == nullptr || moon == nullptr || sun == nullptr ||
		mercury == nullptr || venus == nullptr || mars == nullptr || 
		jupiter == nullptr || saturn == nullptr || uranus == nullptr || 
		neptune == nullptr || pluto == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	

	return true;
}

void Scene0::OnDestroy() {
	if(camera) delete camera, camera = nullptr;
	if(meshPtr) delete meshPtr, meshPtr = nullptr;
	if(earthTexture) delete earthTexture, earthTexture = nullptr;
	if (moonTexture) delete moonTexture, moonTexture = nullptr;
	if(shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if(earth) delete earth, earth = nullptr;
	if(moon) delete moon, moon = nullptr;
	if (skyBox) delete skyBox, skyBox = nullptr;
	if (sun) delete sun, sun = nullptr;
	if (sunTexture) delete sunTexture, sunTexture = nullptr;
	if (mercury) delete mercury, mercury = nullptr;
	if (mercuryTexture) delete mercuryTexture, mercuryTexture = nullptr;
	if (venus) delete venus, venus = nullptr;
	if (venusTexture) delete venusTexture, venusTexture = nullptr;
	if (mars) delete mars, mars = nullptr;
	if (marsTexture) delete marsTexture, marsTexture = nullptr;
	if (jupiter) delete jupiter, jupiter = nullptr;
	if (jupiterTexture) delete jupiterTexture, jupiterTexture = nullptr;
	if (saturn) delete saturn, saturn = nullptr;
	if (saturnTexture) delete saturnTexture, saturnTexture = nullptr;
	if (uranus) delete uranus, uranus = nullptr;
	if (uranusTexture) delete uranusTexture, uranusTexture = nullptr;
	if (neptune) delete neptune, neptune = nullptr;
	if (neptuneTexture) delete neptuneTexture, neptuneTexture = nullptr;
	if (pluto) delete pluto, pluto = nullptr;
	if (plutoTexture) delete plutoTexture, plutoTexture = nullptr;
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {

	switch (sdlEvent.type) {

		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {

			case SDLK_j:
				earth->rb->ApplyTorque(5.0f);
				Vec3 force = Vec3(1.0f, 0.0f, 0.0f);
				earth->rb->ApplyForce(force);
				break;
			}
			break;

		case SDL_KEYUP:

			switch (sdlEvent.key.keysym.sym) {
			case SDLK_h:

				break;
			}
			break;

			
		
	}

}

void Scene0::Update(const float deltaTime_) {

	earth->Update(deltaTime_);
	moon->Update(deltaTime_);
	sun->Update(deltaTime_);
	mercury->Update(deltaTime_);
	venus->Update(deltaTime_);
	mars->Update(deltaTime_);
	static float moonRotation = 0.0f;
	static float earthRotation = 0.0f;
	static float mercRotation = 0.0f;
	static float venusRotation = 0.0f;
	static float marsRotation = 0.0f;
	static float jupiterRotation = 0.0f;
	static float saturnRotation = 0.0f;
	static float uranusRotation = 0.0f;
	static float neptuneRotation = 0.0f;
	static float plutoRotation = 0.0f;

	mercRotation += 0.25f;
	moonRotation += 1.0f;
	earthRotation += 0.2f;
	venusRotation += 0.225f;
	marsRotation += 0.18f;
	jupiterRotation += 0.1f;
	saturnRotation += 0.075f;
	uranusRotation += 0.06f;
	neptuneRotation += 0.03f;
	plutoRotation += 0.028f;

	pluto->setModelMatrix(MMath::rotate(plutoRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(290.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(0.2f)));
	neptune->setModelMatrix(MMath::rotate(neptuneRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(230.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(2.2f)));
	uranus->setModelMatrix(MMath::rotate(uranusRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(175.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(2.5f)));
	saturn->setModelMatrix(MMath::rotate(saturnRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(118.5f, 0.0f, 0.0f)) * MMath::scale(Vec3(3.7f)));
	jupiter->setModelMatrix(MMath::rotate(jupiterRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(60.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(5.0f)));
	mars->setModelMatrix(MMath::rotate(marsRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(32.5f, 0.0f, 0.0f)) * MMath::scale(Vec3(1.0f)));
	earth->setModelMatrix(MMath::rotate(earthRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(25.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(2.0f, 2.0f, 2.0f)));
	moon->setModelMatrix((earth->getModelMatrix() * (MMath::rotate(moonRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate( Vec3(2.0f, 0.0f, 0.0f)))) * MMath::scale(Vec3(0.25f, 0.25f, 0.25f)));
	venus->setModelMatrix(MMath::rotate(venusRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(17.5f, 0.0f, 0.0f)) * MMath::scale(Vec3(1.8f, 1.8f, 1.8f)));
	mercury->setModelMatrix(MMath::rotate(mercRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::translate(Vec3(13.0f, 0.0f, 0.0f)) * MMath::scale(Vec3(0.3f, 0.3f, 0.3f)));
	sun->setModelMatrix(MMath::rotate(earthRotation, Vec3(0.0f, 1.0f, 0.0f)) * MMath::scale(Vec3(10.0f, 10.0f, 10.0f)));
	
}

void Scene0::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint program = skyBox->shaderPtr->getProgram();
	glUseProgram(program);

	glUniformMatrix4fv(skyBox->shaderPtr->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(skyBox->shaderPtr->getUniformID("viewMatrix"), 1, GL_FALSE, Matrix4());

	glDepthMask(GL_FALSE);
	skyBox->Render();
	glDepthMask(GL_TRUE);

	/// Draw your scene here
	program = earth->getShader()->getProgram();
	glUseProgram(program);

	sun->Render();
	mercury->Render();
	earth->Render();
	moon->Render();
	venus->Render();
	mars->Render();
	jupiter->Render();
	saturn->Render();
	uranus->Render();
	neptune->Render();
	pluto->Render();

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(earth->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(earth->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(earth->getShader()->getUniformID("lightPos"), 1, lightSource);

	glUseProgram(0);
}
