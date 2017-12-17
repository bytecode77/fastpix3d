#include "FastPix3D.h"

Font::Font(string path)
{
	SDL_Surface *bitmap = SDLUtility::LoadSurface(path);
	if (bitmap->w % 16 > 0 || bitmap->h % 16 > 0 || bitmap->w > 4096) throw;

	Width = bitmap->w >> 4;
	Height = bitmap->h >> 4;
	CharacterSpacing = 2;
	Buffer = new byte[bitmap->w * bitmap->h];
	CharacterDimensions = new Point[256];

	byte *bitmapPtr = Buffer;
	for (int32 i = 0, *buffer = (int32*)bitmap->pixels; i < bitmap->w * bitmap->h; i++)
	{
		int32 r = (*buffer & 0xff0000) >> 16;
		int32 g = (*buffer & 0xff00) >> 8;
		int32 b = *buffer & 0xff;
		buffer++;
		*bitmapPtr++ = (r + g + b) / 3;
	}
	SDL_FreeSurface(bitmap);

	for (int32 i = 0; i < 256; i++)
	{
		CharacterDimensions[i] = Point(0, CharacterSpacing * 3);
		int32 characterX = (i & 15) * Width, characterY = (i >> 4) * Height;

		bool found = false;
		for (int32 x = 0; x < Width; x++)
		{
			for (int32 y = 0; y < Height; y++)
			{
				if (Buffer[(x + characterX) + (y + characterY) * (Width << 4)])
				{
					CharacterDimensions[i].X = x;
					found = true;
					break;
				}
			}
			if (found) break;
		}

		found = false;
		for (int32 x = Width - 1; x >= 0; x--)
		{
			for (int32 y = 0; y < Height; y++)
			{
				if (Buffer[(x + characterX) + (y + characterY) * (Width << 4)])
				{
					CharacterDimensions[i].Y = x + 1;
					found = true;
					break;
				}
			}
			if (found) break;
		}
	}
}
Font::~Font()
{
	delete[] Buffer, CharacterDimensions;
}

int32 Font::getCharacterSpacing()
{
	return CharacterSpacing;
}

void Font::setCharacterSpacing(int32 characterSpacing)
{
	CharacterSpacing = characterSpacing;
}