#include "GameObject.h"



GameObject::GameObject(const char* objectName, BodyDefinition *bodyDef):gameObjectName(nullptr),
																		texture(nullptr),
																		sprite(nullptr),
																		spriteFileName(nullptr),
																		body(nullptr)
{
	gameObjectName = objectName;
	AddBody(bodyDef);
}

GameObject& GameObject::operator=(const GameObject& newGameObj) {
	gameObjectName = newGameObj.gameObjectName;
	texture = newGameObj.texture;
	sprite = newGameObj.sprite;
	spriteFileName = newGameObj.spriteFileName;
	body = newGameObj.body;

	return *this;
}

GameObject::~GameObject()
{
	if (body)
		body->~Body();
}

// Takes an image's file name
void GameObject::FindSprite(const char *spriteFileName_) {
	spriteFileName = spriteFileName_;
}

// Loads Texture from Surface 
// Surface is definied using FindSprite() 
// Returns false when the sprite could not be loaded
// Returns true when texture has been successfully loaded
bool GameObject::LoadTexture(SDL_Renderer *sdlRenderer) {
	sprite = IMG_Load(spriteFileName);

	if (sprite == nullptr) {
		Debug::Error("GameObject could not find the sprite file. ", __FILE__, __LINE__);
		return false;
	}

	texture = SDL_CreateTextureFromSurface(sdlRenderer, sprite);
	SDL_FreeSurface(sprite);
	return true;
}

// Adds a Body to this GameObject of type BodyDefinition
void GameObject::AddBody(BodyDefinition *bodyDefinition) {
	body = new Body(bodyDefinition);
}

// Removes the Body componenet from this GameObject
void GameObject::RemoveBody() {
	if (body) {
		body->~Body();
		delete body;
		body = nullptr;
	}
}
