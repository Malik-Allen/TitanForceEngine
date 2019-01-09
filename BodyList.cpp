#include "BodyList.h"

BodyList::BodyList()
{
	head = nullptr;
}


BodyList::~BodyList()
{
}

//Takes a dereferenced Body as a parameter
void BodyList::AddBody(Body& newBody) {

	node* newNode;
	node* nodePtr;

	newNode = new node;

	newNode->object = &newBody;
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

//Calls the SetSprite() function of bodies in list
//Returns false if any of the sprites do not exist
bool BodyList::CheckSprites() {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {
		if (!nodePtr->object->SetSprite())
			return false;
		nodePtr = nodePtr->nextNode;
	}

	return true;
}

void BodyList::Update(float timeStep) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {

		nodePtr->object->Update(timeStep);

		nodePtr = nodePtr->nextNode;
	}
}

void BodyList::Render(SDL_Surface* screenSurface, Matrix4 projectionMatrix) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {
		Vec3 screenCoords = projectionMatrix * nodePtr->object->m_position;

		nodePtr->object->m_spriteShape.h = nodePtr->object->m_sprite->h;
		nodePtr->object->m_spriteShape.w = nodePtr->object->m_sprite->w;

		nodePtr->object->m_spriteShape.x = screenCoords.x; ///implicit type conversions BAD - probably causes a compiler warning
		nodePtr->object->m_spriteShape.y = screenCoords.y; ///implicit type conversions BAD - probably causes a compiler warning

		SDL_BlitSurface(nodePtr->object->m_sprite, nullptr, screenSurface, &nodePtr->object->m_spriteShape);

		nodePtr = nodePtr->nextNode;
	}
}
//Returns the mass of the object, given the object has been named
float BodyList::MassCheck(const char* objectName) {

	node *nodePtr;
	nodePtr = head;

	while (nodePtr != nullptr) {

		if (objectName == nodePtr->object->m_objectName)
			return nodePtr->object->m_mass;

		nodePtr = nodePtr->nextNode;
	}

	return 0.0f;

}
