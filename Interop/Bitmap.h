#pragma once
#include "FastPix3D.h"

class Bitmap
{
private:
	int32 _Width;
	int32 _Height;
	int32 *_Pixels;

public:
	property_get(int32, Width)
	{
		return _Width;
	}
	property_get(int32, Height)
	{
		return _Height;
	}
	property_get(int32*, Pixels)
	{
		return _Pixels;
	}

	explicit Bitmap(int32 width, int32 height);
	~Bitmap();

	static Bitmap* Load(const char *path);
};