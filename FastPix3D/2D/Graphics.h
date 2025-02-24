#pragma once
#include "../FastPix3D.h"
#include "../Interop/Window.h"
#include "../Math/Color.h"
#include "Font.h"

class FASTPIX3D_API Graphics
{
private:
	const int32 _Width;
	const int32 _Height;
	Color *_Pixels;
	const Font *_Font;

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
	property_get(const ::Font*, Font)
	{
		return _Font;
	}
	property_set(const ::Font*, Font)
	{
		_Font = value;
	}

	explicit Graphics(const ::Window &window);

	void FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const;
	void FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const;
	void DrawString(int32 x, int32 y, const char *text) const;
	void DrawString(int32 x, int32 y, const char *text, const Color &color) const;
	int32 MeasureString(const char *text) const;
};