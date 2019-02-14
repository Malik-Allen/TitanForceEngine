#include "Window.h"
#include <SDL.h>
#include <iostream> 


Window::Window(int newWidth, int newHeight) {
	screenSurface = nullptr;
	window = nullptr;
	renderer = nullptr;
	width = newWidth;
	height = newHeight;
}

// Initiates the screeSurface, renderer and creates the window, returns true on sucess
bool Window::OnCreate() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Titan Force Engine 0.1.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	screenSurface = SDL_GetWindowSurface(window);
	if (screenSurface == nullptr) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if (renderer == nullptr) {
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Window::OnDestroy() {
	/// Kill the surface first
	if (screenSurface) {
		SDL_FreeSurface(screenSurface);
	}

	/// Now kill the window
	if (window) {
		SDL_DestroyWindow(window);
	}

	/// Kill the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}

	///Exit the SDL subsystems
	SDL_Quit();

}

Window::~Window() {
	OnDestroy();
}

SDL_Window* Window::GetSDL_Window() {
	return window;
}
