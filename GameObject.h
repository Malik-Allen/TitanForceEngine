#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL.h>
#include <SDL_image.h>
#include "Debug.h"
#include "Body.h"


/// Current GameObject Abilities Include:
/// Upon instantiation a GameObject is given a name a body definition
/// Load Textures inside of a SDL_Rect
/// The removal of said Body -- Feb 11, 2019


class GameObject
{
public:
	const char *gameObjectName;
	SDL_Texture *texture;
	SDL_Surface *sprite;
	const char *spriteFileName;
	SDL_Rect spriteRect;

	Body *body;

	GameObject(const char *objectName, BodyDefinition *bodyDef);
	~GameObject();

	GameObject& operator=(const GameObject& newGameObject);

	void FindSprite(const char *spriteFileName_);
	bool LoadTexture(SDL_Renderer *renderer);

	void AddBody(BodyDefinition *bodyDef);
	void RemoveBody();
};

#endif

