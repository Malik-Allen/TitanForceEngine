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

	void AddBody(Body& newBody);
	///void DeleteBody(); //Comes with naming system according to char
	void Update(float timeStep);
	bool CheckSprites();
	void Render(SDL_Surface* screenSurface, Matrix4 projectionMatrix);

	BodyList();
	~BodyList();


};

#endif

