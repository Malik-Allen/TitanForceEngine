#include "Scene1.h"



Scene1::Scene1(): camera(nullptr), skyBox(nullptr), ball(nullptr), shaderPtr(nullptr), texturePtr(nullptr), meshPtr(nullptr) {}


Scene1::~Scene1() {
}

bool Scene1::OnCreate() {
	camera = new Camera();
	skyBox = new SkyBox();

	if(skyBox->OnCreate())
		skyBox->LoadSkyBox("posX.jpg", "posY.jpg", "posZ.jpg", "negX.jpg", "negY.jpg", "negZ.jpg");


	if (ObjLoader::loadOBJ("sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	texturePtr = new Texture();

	if (texturePtr->LoadImage("sunTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	shaderPtr = new Shader("textureVert.glsl", "textureFrag.glsl");

	ball = new GameObject(meshPtr, shaderPtr, texturePtr);

	return true;
}

void Scene1::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (skyBox) delete skyBox, skyBox = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {}

void Scene1::Update(const float deltaTime_) {}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	GLuint program = skyBox->shaderPtr->getProgram();
	glUseProgram(program);

	glUniformMatrix4fv(skyBox->shaderPtr->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(skyBox->shaderPtr->getUniformID("viewMatrix"), 1, GL_FALSE, Matrix4());

	glDepthMask(GL_FALSE);
	skyBox->Render();
	glDepthMask(GL_TRUE);

	glUniformMatrix4fv(ball->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(ball->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	

	ball->Render();

	glUseProgram(0);
}
