#include "Font.h"
#include "../Interop/Bitmap.h"
#include "../Math/Math_.h"
#include "../Math/Color.h"

Font::Font(const char *path, int32 columnCount, int32 rowCount, int32 startChar) : Font(path, columnCount, rowCount, startChar, 2)
{
}
Font::Font(const char *path, int32 columnCount, int32 rowCount, int32 startChar, int32 characterSpacing)
{
	if (!path) throw std::invalid_argument("path cannot be null.");
	if (columnCount <= 0) throw std::invalid_argument("Column count must be a positive number.");
	if (rowCount <= 0) throw std::invalid_argument("Row count must be a positive number.");

	Bitmap *bitmap = Bitmap::FromFile(path);
	if (bitmap->Width > 4096 || bitmap->Height > 4096) throw std::runtime_error("Font bitmap exceeds the maximum size of 4096x4096.");
	if (bitmap->Width % columnCount != 0 || bitmap->Height % rowCount != 0) throw std::invalid_argument("Bitmap size must be a multiple of columnCount and rowCount.");

	int32 width = bitmap->Width / columnCount;
	Height = bitmap->Height / rowCount;
	StartChar = startChar;
	CharCount = rowCount * columnCount;
	_CharacterSpacing = characterSpacing;

	if (StartChar < 0 || StartChar + CharCount > 255) throw std::out_of_range("Font character range exceeds the range of valid ASCII values.");

	CharacterOffsets = new int32[CharCount];
	CharacterWidths = new int32[CharCount];

	int32 leftEdges[256];
	int32 totalWidth = 0;

	for (int32 i = 0; i < CharCount; i++)
	{
		int32 characterX = i % columnCount * width;
		int32 characterY = i / columnCount * Height;

		// Search for left and right edge of character, store only pixels that fall into the character's bounding box.
		bool foundEdge = false;

		for (int32 x = 0; x < width; x++)
		{
			for (int32 y = 0; y < Height; y++)
			{
				if (bitmap->Pixels[characterX + x + (characterY + y) * bitmap->Width].RGB & 0xffffff)
				{
					leftEdges[i] = x;
					foundEdge = true;
					break;
				}
			}
			if (foundEdge) break;
		}

		if (foundEdge)
		{
			foundEdge = false;
			int32 rightEdge = 0;

			for (int32 x = width - 1; x >= 0; x--)
			{
				for (int32 y = 0; y < Height; y++)
				{
					if (bitmap->Pixels[characterX + x + (characterY + y) * bitmap->Width].RGB & 0xffffff)
					{
						rightEdge = x;
						foundEdge = true;
						break;
					}
				}
				if (foundEdge) break;
			}

			CharacterWidths[i] = rightEdge - leftEdges[i] + 1;
		}
		else
		{
			// Whitespace character.
			leftEdges[i] = 0;
			CharacterWidths[i] = Math::Max(_CharacterSpacing * 3, 2);
		}

		CharacterOffsets[i] = i == 0 ? 0 : CharacterOffsets[i - 1] + CharacterWidths[i - 1] * Height;
		totalWidth += CharacterWidths[i];
	}

	// After computing all character widths, create a buffer that contains all characters in a single row.
	Buffer = new byte[totalWidth * Height];
	byte *ptr = Buffer;

	for (int32 i = 0; i < CharCount; i++)
	{
		int32 characterX = i % columnCount * width + leftEdges[i];
		int32 characterY = i / columnCount * Height;

		for (int32 y = 0; y < Height; y++)
		{
			for (int32 x = 0; x < CharacterWidths[i]; x++)
			{
				Color color = bitmap->Pixels[characterX + x + (characterY + y) * bitmap->Width];
				*ptr++ = (color.R + color.G + color.B) / 3;
			}
		}
	}

	delete bitmap;
}
Font::~Font()
{
	delete[] Buffer;
	delete[] CharacterOffsets;
	delete[] CharacterWidths;
}