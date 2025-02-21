#include "Window.h"

bool Window::SDLInitialized = false;

Window::Window(int32 width, int32 height, const char *title)
{
	if (!SDLInitialized)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) throw;
		SDLInitialized = true;
	}

	SDLWindow = SDL_CreateWindow(title, width, height, 0);
	if (!SDLWindow) throw;

	SDLSurface = SDL_GetWindowSurface(SDLWindow);
	if (!SDLSurface) throw;
}
Window::~Window()
{
	SDL_DestroyWindow(SDLWindow);
}

void Window::SetRelativeMouseMode(bool enabled)
{
	SDL_SetWindowRelativeMouseMode(SDLWindow, enabled);
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