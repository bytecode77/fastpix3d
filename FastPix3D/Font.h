#pragma once
#include "FastPix3D.h"
#include "Vector2i.h"

class Font
{
private:
	int32 Width;
	int32 Height;
	int32 CharacterSpacing;
	byte *Buffer;
	Vector2i *CharacterDimensions;

public:
	property_getset(int32, CharacterSpacing)
	{
		return CharacterSpacing;
	}
	property_set(int32, CharacterSpacing)
	{
		CharacterSpacing = value;
	}

	explicit Font(const char *path);
	~Font();

	friend class Graphics;
};