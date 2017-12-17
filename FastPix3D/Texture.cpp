#include "FastPix3D.h"

Texture::Texture(string path)
{
	SDL_Surface *bitmap = SDLUtility::LoadSurface(path);

	FileName = path;
	Width = bitmap->w;
	Height = bitmap->h;

	int32 width2 = GetLogarithmicCeiling(Width), height2 = GetLogarithmicCeiling(Height);
	Buffer = new int32[width2 * height2];
	if (Width == width2 && Height == height2)
	{
		memcpy(Buffer, bitmap->pixels, Width * Height * 4);
	}
	else
	{
		int32 *pixels = (int32*)bitmap->pixels;
		for (int32 y = 0, *buffer = Buffer; y < height2; y++)
		{
			for (int32 x = 0; x < width2; x++)
			{
				*buffer++ = pixels[x * Width / width2 + y * Height / height2 * Width];
			}
		}
		Width = width2;
		Height = height2;
	}
	WidthExponent = GetExponent(Width);
	SDL_FreeSurface(bitmap);
}
Texture::~Texture()
{
	delete[] Buffer;
}

int32 Texture::getWidth()
{
	return Width;
}
int32 Texture::getHeight()
{
	return Height;
}
int32* Texture::getBuffer()
{
	return Buffer;
}
string Texture::getFileName()
{
	return FileName;
}

int32 Texture::GetLogarithmicCeiling(int32 number)
{
	if (number & number - 1)
	{
		for (int32 i = 31; i >= 0; i--)
		{
			if (number & 1 << i) return 1 << (i + 1);
		}
	}
	return number;
}
int32 Texture::GetExponent(int32 number)
{
	for (int32 i = 31; i >= 0; i--)
	{
		if (1 << i == number) return i;
	}
	return 0;
}