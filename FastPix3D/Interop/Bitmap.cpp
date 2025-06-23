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
	SDL_Surface *surface = IMG_Load(path);
	if (!surface) throw;

	Bitmap *bitmap = FromSurface(surface);
	SDL_DestroySurface(surface);

	return bitmap;
}
Bitmap* Bitmap::FromMemory(const void *buffer, int32 size)
{
	SDL_IOStream *stream = SDL_IOFromConstMem(buffer, size);
	if (!stream) throw;

	SDL_Surface *surface = IMG_Load_IO(stream, true);
	if (!surface) throw;

	Bitmap *bitmap = FromSurface(surface);
	SDL_DestroySurface(surface);

	return bitmap;
}
Bitmap* Bitmap::FromSurface(SDL_Surface *surface)
{
	SDL_Surface *copy = SDL_CreateSurface(surface->w, surface->h, SDL_PIXELFORMAT_XRGB8888);
	if (!copy) throw;

	if (!SDL_BlitSurface(surface, NULL, copy, NULL)) throw;

	Bitmap *bitmap = new Bitmap(copy->w, copy->h);
	memcpy(bitmap->_Pixels, copy->pixels, bitmap->_Width * bitmap->_Height * 4);
	SDL_DestroySurface(copy);

	return bitmap;
}