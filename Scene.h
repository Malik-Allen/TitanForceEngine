#ifndef SCENE_H
#define SCENE_H

#include "Debug.h"

/// Scene Header Capabilities
/// This header is to be inherited by Scene classes inorder to recieve overridables
/// Dec 13, 2018 

union SDL_Event;

class Scene {
	// All Child Scenes should be instantiated with a:
		// Window, Projection Matrix(Matrix4), Renderer
	// Scene Constructors need to be overriden with the SDL_Window and Renderer ptrs as paramters

public:
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float time) = 0;
	virtual void Render() = 0;
	virtual void InputManagement(const SDL_Event &inputEvent) = 0;

};

#endif