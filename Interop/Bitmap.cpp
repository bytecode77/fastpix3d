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
	char *extension = &PathFindExtensionA(path)[1];
	SDL_Surface *surface = nullptr;
	SDL_RWops *file = SDL_RWFromFile(path, "rb");

	if (!lstrcmpiA(extension, "bmp"))
	{
		surface = SDL_LoadBMP_RW(file, 1);
	}
	else if (!lstrcmpiA(extension, "png"))
	{
		surface = IMG_LoadPNG_RW(file);
	}
	else if (!lstrcmpiA(extension, "jpg") || !lstrcmpiA(extension, "jpeg"))
	{
		surface = IMG_LoadJPG_RW(file);
	}
	else if (!lstrcmpiA(extension, "tga"))
	{
		surface = IMG_LoadTGA_RW(file);
	}
	else if (!lstrcmpiA(extension, "tif") || !lstrcmpiA(extension, "tiff"))
	{
		surface = IMG_LoadTIF_RW(file);
	}

	if (!surface) throw;

	SDL_Surface *surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, 0xff0000, 0x00ff00, 0x0000ff, 0);
	SDL_BlitSurface(surface, NULL, surface2, NULL);
	SDL_FreeSurface(surface);

	Bitmap *bitmap = new Bitmap(surface2->w, surface2->h);
	memcpy(bitmap->_Pixels, surface2->pixels, bitmap->_Width * bitmap->_Height * 4);

	SDL_FreeSurface(surface2);
	return bitmap;
}