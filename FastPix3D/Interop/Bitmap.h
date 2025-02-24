#pragma once
#include "../FastPix3D.h"
#include "../Math/Color.h"

struct SDL_Surface;

class FASTPIX3D_API Bitmap
{
private:
	const int32 _Width;
	const int32 _Height;
	Color *_Pixels;

public:
	readonly_property(int32, Width)
	{
		return _Width;
	}
	readonly_property(int32, Height)
	{
		return _Height;
	}
	readonly_property(Color*, Pixels)
	{
		return _Pixels;
	}

	explicit Bitmap(int32 width, int32 height);
	~Bitmap();

	static Bitmap* FromFile(const char *path);
	static Bitmap* FromMemory(const void *buffer, int32 size);

private:
	static Bitmap* FromSurface(SDL_Surface *surface);
};