#ifndef GAMEOBJLIST_H
#define GAMEOBJLIST_H

#include "MMath.h"
#include "GameObject.h"

/// Current GameObjList Abilities Include:
/// The handling of rendering across all GameObjects inside of the list
/// The removal of an item from this list will result in it not being rendered
/// Calls to the Body inside of GameObjects to do a Physics Update 
/// Calls to the Load Texture function inside of all GameObjects in the list in order to load all the sprites at once -- Feb 13, 2019

// Future Iteration on GameObjList Includes:
// Clear List function -- Feb 13, 2019

using namespace MATH;

class GameObjList
{
	struct node {
		GameObject *gameObject;
		node *nextNode;
	};

private:
	node *head;
	node *previousNode;

public:
	GameObjList();
	~GameObjList();

	void AddGameObj(GameObject& newGameObj);
	void RemoveGameObj(const char *gameObjName);
	bool LoadTextures(SDL_Renderer *sdlRenderer);
	void Render(SDL_Renderer *sdlRenderer, SDL_Surface *screenSurface, Matrix4 projectionMatrix);
	void Update(float timeStep);
};

#endif

