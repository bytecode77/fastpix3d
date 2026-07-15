#include "Window.h"
#include <SDL3/SDL.h>

int32 Window::WindowCount = 0;

int32 Window::get_Width() const
{
	return SDLSurface->w;
}
int32 Window::get_Height() const
{
	return SDLSurface->h;
}
Color* Window::get_Pixels() const
{
	return (Color*)SDLSurface->pixels;
}
const char* Window::get_Title() const
{
	return SDL_GetWindowTitle(SDLWindow);
}
void Window::set_Title(const char* value)
{
	SDL_SetWindowTitle(SDLWindow, value);
}

Window::Window(int32 width, int32 height, const char *title)
{
	if (WindowCount++ == 0)
	{
		if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error("Failed to initialize SDL.");
		_MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);
	}

	// Only support resolutions that are multiples of 8 to simplify the halfspace rasterization.
	if ((width & 7) || (height & 7)) throw std::runtime_error("Window width and height must be multiples of 8.");

	SDLWindow = SDL_CreateWindow(title ? title : "", width, height, 0);
	if (!SDLWindow) throw std::runtime_error("Failed to create window.");

	SDLSurface = SDL_GetWindowSurface(SDLWindow);
	if (!SDLSurface) throw std::runtime_error("Failed to create window.");
}
Window::~Window()
{
	SDL_DestroyWindow(SDLWindow);

	if (--WindowCount == 0)
	{
		SDL_Quit();
	}
}

void Window::SetRelativeMouseMode(bool enabled)
{
	SDL_SetWindowRelativeMouseMode(SDLWindow, enabled);
}
void Window::Lock()
{
	if (SDL_MUSTLOCK(SDLSurface))
	{
		SDL_LockSurface(SDLSurface);
	}
}
void Window::Unlock()
{
	if (SDL_MUSTLOCK(SDLSurface))
	{
		SDL_UnlockSurface(SDLSurface);
	}
}
void Window::Flip()
{
	SDL_UpdateWindowSurface(SDLWindow);
}