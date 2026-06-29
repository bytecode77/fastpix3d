#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API Font
{
private:
	int32 Height;
	int32 StartChar;
	int32 CharCount;
	int32 _CharacterSpacing;
	byte *Buffer;
	int32 *CharacterOffsets;
	int32 *CharacterWidths;

public:
	property_get(int32, CharacterSpacing)
	{
		return _CharacterSpacing;
	}
	property_set(int32, CharacterSpacing)
	{
		_CharacterSpacing = value;
	}

	explicit Font(const char *path, int32 columnCount, int32 rowCount, int32 startChar);
	explicit Font(const char *path, int32 columnCount, int32 rowCount, int32 startChar, int32 characterSpacing);
	~Font();

	__forceinline bool HasChar(char c) const
	{
		return c >= StartChar && c < StartChar + CharCount;
	}
	__forceinline int32 MeasureChar(char c) const
	{
		return HasChar(c) ? CharacterWidths[c - StartChar] : 0;
	}

	friend class Graphics;
};