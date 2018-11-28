#include "BodyList.h"

BodyList::BodyList()
{
	head = nullptr;
}


BodyList::~BodyList()
{
}

void BodyList::AddBody(Body& newBody) { //Must pass a dereferenced Body

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
