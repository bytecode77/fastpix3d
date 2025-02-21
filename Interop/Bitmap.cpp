#include "Bitmap.h"
#include "SDL/SDL_image.h"

Bitmap::Bitmap(int32 width, int32 height) :
	_Width(width),
	_Height(height),
	_Pixels(new int32[width * height])
{
}
Bitmap::~Bitmap()
{
	delete[] Pixels;
}

Bitmap* Bitmap::Load(const char *path)
{
	SDL_Surface *surface = IMG_Load(path);
	if (!surface) throw;

	SDL_Surface *surface2 = SDL_CreateSurface(surface->w, surface->h, SDL_PIXELFORMAT_XRGB8888);
	if (!surface2) throw;

	SDL_BlitSurface(surface, NULL, surface2, NULL);
	SDL_DestroySurface(surface);

	Bitmap *bitmap = new Bitmap(surface2->w, surface2->h);
	memcpy(bitmap->_Pixels, surface2->pixels, bitmap->_Width * bitmap->_Height * 4);
	SDL_DestroySurface(surface2);

	return bitmap;
}