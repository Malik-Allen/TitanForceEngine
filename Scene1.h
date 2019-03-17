#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "SkyBox.h"
#include "Camera.h"
#include "GameObject.h"
union SDL_Event;
class Camera;
class Shader;
class GameObject;
class Mesh;
class Texture;

using namespace MATH;

class Scene1 : public Scene {
private:
	Camera *camera;
	SkyBox *skyBox; 

	GameObject *ball;
	Shader *shaderPtr;
	Mesh *meshPtr;
	Texture *texturePtr;
	
public:

	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
};


#endif // !SCENE1_H