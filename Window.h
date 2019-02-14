#ifndef WINDOW_H
#define WINDOW_H

class Window {
private:
	struct SDL_Window* window;
	struct SDL_Surface* screenSurface;
	int width, height;

public:
	struct SDL_Renderer * renderer;

	Window(int newWidth, int newHeight);
	~Window();
	bool OnCreate();
	void OnDestroy();
	SDL_Window* GetSDL_Window();
};

#endif

