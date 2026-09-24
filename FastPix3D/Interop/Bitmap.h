#pragma once
#include "../FastPix3D.h"
#include "../Math/Color.h"

struct IWICImagingFactory;
struct IWICBitmapDecoder;

class FASTPIX3D_API Bitmap
{
private:
	static INIT_ONCE InitOnce;
	static IWICImagingFactory *ImagingFactory;
	int32 _Width;
	int32 _Height;
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
	Bitmap(const Bitmap &other);
	~Bitmap();

	static Bitmap* FromFile(const char *path);
	static Bitmap* FromMemory(const void *buffer, int32 size);

private:
	static void EnsureInitialized();
	static BOOL CALLBACK Initialize(PINIT_ONCE initOnce, PVOID parameter, PVOID *context);
	static Bitmap* FromStream(IStream *stream);

public:
	Bitmap& operator =(const Bitmap &other);
};