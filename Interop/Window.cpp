#include "Window.h"

bool Window::SDLInitialized = false;

Window::Window(int32 width, int32 height, const char *title)
{
	if (!SDLInitialized)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) throw;
		SDLInitialized = true;
	}

	SDLWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!SDLWindow) throw;

	SDLSurface = SDL_GetWindowSurface(SDLWindow);
	if (!SDLSurface) throw;
}
Window::~Window()
{
	SDL_DestroyWindow(SDLWindow);
}

void Window::Lock()
{
	SDL_LockSurface(SDLSurface);
}
void Window::Unlock()
{
	SDL_UnlockSurface(SDLSurface);
}
void Window::Flip()
{
	SDL_UpdateWindowSurface(SDLWindow);
}