#include "FastPix3D.h"

SDL_Surface* SDLUtility::LoadSurface(string path)
{
	SDL_Surface *bitmap = NULL;
	string ext = Path::GetExtension(path);

	if (ext == "bmp") bitmap = SDL_LoadBMP(path.c_str());
	else if (ext == "jpg" || ext == "jpeg") bitmap = IMG_LoadJPG_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "png") bitmap = IMG_LoadPNG_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "gif") bitmap = IMG_LoadGIF_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "tga") bitmap = IMG_LoadTGA_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "tif" || ext == "tiff") bitmap = IMG_LoadTIF_RW(SDL_RWFromFile(&path[0], "rb"));

	if (bitmap == NULL) throw;

	SDL_Surface *image = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 32, 0x0000ff, 0x00ff00, 0xff0000, 0);
	SDL_BlitSurface(bitmap, NULL, image, NULL);
	SDL_FreeSurface(bitmap);
	return image;
}