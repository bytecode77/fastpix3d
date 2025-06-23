#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"

class FASTPIX3D_API Font
{
private:
	int32 Width;
	int32 Height;
	int32 _CharacterSpacing;
	byte *Buffer;
	vint2 *CharacterDimensions;

public:
	property_get(int32, CharacterSpacing)
	{
		return _CharacterSpacing;
	}
	property_set(int32, CharacterSpacing)
	{
		_CharacterSpacing = value;
	}

	explicit Font(const char *path);
	~Font();

	friend class Graphics;
};