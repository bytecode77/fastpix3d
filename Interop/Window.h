#pragma once
#include "FastPix3D.h"

class Window
{
private:
	static bool SDLInitialized;
	SDL_Window *SDLWindow;
	SDL_Surface *SDLSurface;

public:
	property_get(int32, Width)
	{
		return SDLSurface->w;
	}
	property_get(int32, Height)
	{
		return SDLSurface->h;
	}
	property_get(int32*, Pixels)
	{
		return (int32*)SDLSurface->pixels;
	}
	property_getset(const char*, Title)
	{
		return SDL_GetWindowTitle(SDLWindow);
	}
	property_set(const char*, Title)
	{
		SDL_SetWindowTitle(SDLWindow, value);
	}

	explicit Window(int32 width, int32 height, const char *title);
	~Window();

	void Lock();
	void Unlock();
	void Flip();

	friend class Input;
};