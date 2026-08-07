#include "Bitmap.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

Bitmap::Bitmap(int32 width, int32 height) :
	_Width(width),
	_Height(height),
	_Pixels(new Color[width * height])
{
}
Bitmap::~Bitmap()
{
	delete[] _Pixels;
}

Bitmap* Bitmap::FromFile(const char *path)
{
	if (!path) throw std::invalid_argument("path cannot be null.");

	SDL_Surface *surface = IMG_Load(path);
	if (!surface) throw std::runtime_error("Failed to load image.");

	Bitmap *bitmap = FromSurface(surface);
	SDL_DestroySurface(surface);

	return bitmap;
}
Bitmap* Bitmap::FromMemory(const void *buffer, int32 size)
{
	SDL_IOStream *stream = SDL_IOFromConstMem(buffer, size);
	if (!stream) throw std::runtime_error("Failed to load image.");

	SDL_Surface *surface = IMG_Load_IO(stream, true);
	if (!surface) throw std::runtime_error("Failed to load image.");

	Bitmap *bitmap = FromSurface(surface);
	SDL_DestroySurface(surface);

	return bitmap;
}
Bitmap* Bitmap::FromSurface(SDL_Surface *surface)
{
	Bitmap* bitmap = new Bitmap(surface->w, surface->h);
	SDL_Surface* dest = SDL_CreateSurfaceFrom(bitmap->_Width, bitmap->_Height, SDL_PIXELFORMAT_XRGB8888, bitmap->_Pixels, bitmap->_Width * 4);

	if (!dest)
	{
		delete bitmap;
		throw std::runtime_error("Failed to load image.");
	}

	if (!SDL_BlitSurface(surface, nullptr, dest, nullptr))
	{
		SDL_DestroySurface(dest);
		delete bitmap;
		throw std::runtime_error("Failed to load image.");
	}

	SDL_DestroySurface(dest);

	// Remove alpha channel to avoid spilling of alpha values into the RGB channels.
	int32 *ptr = (int32*)bitmap->Pixels;
	int32 count = bitmap->_Width * bitmap->_Height;

	for (int32 i = 0; i < count; i++)
	{
		*ptr++ &= 0xffffff;
	}

	return bitmap;
}