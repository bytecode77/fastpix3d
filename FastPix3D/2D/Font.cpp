#include "Font.h"
#include "../Interop/Bitmap.h"
#include "../Math/Color.h"

Font::Font(const char *path)
{
	Bitmap *bitmap = Bitmap::FromFile(path);
	if (bitmap->Width % 16 > 0 || bitmap->Height % 16 > 0 || bitmap->Width > 4096) throw;

	Width = bitmap->Width / 16;
	Height = bitmap->Height / 16;
	_CharacterSpacing = 2;
	Buffer = new byte[bitmap->Width * bitmap->Height];
	CharacterDimensions = new vint2[256];

	byte *ptr = Buffer;
	Color *buffer = bitmap->Pixels;
	for (int32 i = 0; i < bitmap->Width * bitmap->Height; i++)
	{
		*ptr++ = (buffer->R + buffer->G + buffer->B) / 3;
		buffer++;
	}

	delete bitmap;

	// Compute character dimensions (vint2.X is the leftmost pixel, vint2.Y is the rightmost pixel).
	for (int32 i = 0; i < 256; i++)
	{
		CharacterDimensions[i] = vint2(0, _CharacterSpacing * 3);
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