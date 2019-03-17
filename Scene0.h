#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include <vector>
#include "SkyBox.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	Camera *camera;
	SkyBox *skyBox;
	Vec3 lightSource;

	Mesh *meshPtr;
	Shader *shaderPtr;

	GameObject *sun;
	GameObject *mercury;
	GameObject *earth;
	GameObject *moon;
	GameObject *venus;
	GameObject *mars;
	GameObject *jupiter;
	GameObject *saturn;
	GameObject *uranus;
	GameObject *neptune;
	GameObject *pluto;

	Texture *sunTexture;
	Texture *mercuryTexture;
	Texture *earthTexture;
	Texture *moonTexture;
	Texture *venusTexture;
	Texture *marsTexture;
	Texture *jupiterTexture;
	Texture *saturnTexture;
	Texture *uranusTexture;
	Texture *neptuneTexture;
	Texture *plutoTexture;

	


public:

	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H