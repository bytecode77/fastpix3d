#include "Font.h"
#include "Bitmap.h"

Font::Font(const char *path)
{
	Bitmap *bitmap = Bitmap::Load(path);
	if (bitmap->Width % 16 > 0 || bitmap->Height % 16 > 0 || bitmap->Width > 4096) throw;

	Width = bitmap->Width / 16;
	Height = bitmap->Height / 16;
	_CharacterSpacing = 2;
	Buffer = new byte[bitmap->Width * bitmap->Height];
	CharacterDimensions = new Vector2i[256];

	byte *ptr = Buffer;
	for (int32 i = 0, *buffer = (int32*)bitmap->Pixels; i < bitmap->Width * bitmap->Height; i++)
	{
		int32 r = (*buffer & 0xff0000) >> 16;
		int32 g = (*buffer & 0xff00) >> 8;
		int32 b = *buffer & 0xff;
		buffer++;
		*ptr++ = (r + g + b) / 3;
	}

	delete bitmap;

	// Compute character dimensions (Vector2i.X is the leftmost pixel, Vector2i.Y is the rightmost pixel).
	for (int32 i = 0; i < 256; i++)
	{
		CharacterDimensions[i] = Vector2i(0, _CharacterSpacing * 3);
		int32 characterX = i % 16 * Width;
		int32 characterY = i / 16 * Height;

		bool found = false;
		for (int32 x = 0; x < Width; x++)
		{
			for (int32 y = 0; y < Height; y++)
			{
				if (Buffer[x + characterX + (y + characterY) * Width * 16])
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
				if (Buffer[x + characterX + (y + characterY) * Width * 16])
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
	delete[] Buffer;
	delete[] CharacterDimensions;
}