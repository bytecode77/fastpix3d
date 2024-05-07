#pragma once
#include "FastPix3D.h"
#include "Color.h"
#include "Window.h"
#include "Font.h"

class Graphics
{
private:
	const Window &Window;
	const Font *Font;

public:
	explicit Graphics(const ::Window &window);

	void SetFont(const ::Font &font);

	void FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const;
	void FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float opacity) const;
	void DrawString(int32 x, int32 y, const char *text) const;
	void DrawString(int32 x, int32 y, const char *text, const Color &color) const;
	int32 MeasureString(const char *text) const;
};