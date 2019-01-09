#ifndef BODYLIST_H
#define BODYLIST_H

#include "Body.h"
#include "MMath.h"

using namespace MATH;

class BodyList
{
	struct node {
		Body *object;
		node *nextNode;
	};
private:
	node* head;
public:

	virtual void AddBody(Body& newBody);
	///void DeleteBody(); //Comes with naming system according to char
	virtual void Update(float timeStep);

	virtual float MassCheck(const char* objectName);
	virtual bool CheckSprites();
	virtual void Render(SDL_Surface* screenSurface, Matrix4 projectionMatrix);

	BodyList();
	~BodyList();


};

#endif

