#include "GameObjList.h"



GameObjList::GameObjList(): head(nullptr), previousNode(nullptr)
{
}


GameObjList::~GameObjList()
{
}

// Add a GameObject to this list of GameObjects
// The order in which you add GameObjs to the list dictates what is draw in the front
void GameObjList::AddGameObj(GameObject& newGameObj) {

	node *newNode;
	node *nodePtr;

	newNode = new node;

	newNode->gameObject = &newGameObj;
	newNode->nextNode = nullptr;

	if (!head)
		head = newNode;
	else {
		nodePtr = head;

		while (nodePtr->nextNode != nullptr) {
			nodePtr = nodePtr->nextNode;
		}

		nodePtr->nextNode = newNode;
	}
}

// Removes specified GameObject from this list
void GameObjList::RemoveGameObj(const char *gameObjName) {

	node *nodePtr;
	
	if (!head) {
		Debug::Warning("GameObj List is already empty", __FILE__, __LINE__);
		return;
	}

	if (head->gameObject->gameObjectName == gameObjName) {

		nodePtr = head->nextNode;
		head->gameObject->~GameObject(); /// Call to GameObject Deconstructor
		delete head;
		head = nodePtr;
	}
	else {
		nodePtr = head;

		while (nodePtr != nullptr && 
			nodePtr->nextNode->gameObject->gameObjectName != gameObjName) {

			previousNode = nodePtr;
			nodePtr = nodePtr->nextNode;
		}

		if (nodePtr) {
			previousNode->nextNode = nodePtr->nextNode;
			nodePtr->gameObject->~GameObject();
			delete nodePtr;
		}
	}

	Debug::Warning("GameObject was not deleted from the list or gameObjName does not exist", __FILE__, __LINE__);
}

// Returns true when all the textures in this list have been loaded
bool GameObjList::LoadTextures(SDL_Renderer *sdlRenderer) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {
		if (!nodePtr->gameObject->LoadTexture(sdlRenderer))
			return false;

		nodePtr = nodePtr->nextNode;
	}

	return true;
}

// Renders a copy of the sprite on screen, this should called in update
void GameObjList::Render(SDL_Renderer *sdlRenderer, SDL_Surface *screenSurface, Matrix4 projectionMatrix) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {
		Vec3 screenCoords = projectionMatrix * nodePtr->gameObject->body->m_position;

		nodePtr->gameObject->spriteRect.h = nodePtr->gameObject->sprite->h;
		nodePtr->gameObject->spriteRect.w = nodePtr->gameObject->sprite->w;

		nodePtr->gameObject->spriteRect.x = screenCoords.x; /// Implicit type conversion!
		nodePtr->gameObject->spriteRect.y = screenCoords.y; /// Implicit type conversion!
		double angle = nodePtr->gameObject->body->m_angle;	/// Implicit type conversion!

		SDL_RenderCopyEx(sdlRenderer, nodePtr->gameObject->texture, nullptr, &nodePtr->gameObject->spriteRect, angle, nullptr, SDL_FLIP_NONE);

		//SDL_RenderCopy(sdlRenderer, nodePtr->gameObject->texture, nullptr, &nodePtr->gameObject->spriteRect);

		nodePtr = nodePtr->nextNode;
	}
}

// Calls the update functionality of Game Objects in this list
void GameObjList::Update(float timeStep) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {
		nodePtr->gameObject->body->Update(timeStep);
		nodePtr = nodePtr->nextNode;
	}
}



