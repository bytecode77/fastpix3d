#include "Graphics.h"
#include "Math_.h"

Graphics::Graphics(const ::Window &window) :
	Window(window),
	Font(nullptr)
{
}

void Graphics::SetFont(const ::Font &font)
{
	Font = &font;
}

void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const
{
	FillRectangle(x, y, width, height, color, 1);
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float opacity) const
{
	int32 xFrom = Math::Clamp(x, 0, Window.Width - 1);
	int32 yFrom = Math::Clamp(y, 0, Window.Width - 1);
	int32 xTo = Math::Clamp(x + width, 0, Window.Width - 1);
	int32 yTo = Math::Clamp(y + height, 0, Window.Width - 1);

	int32 opacityA = Math::Clamp((int32)(opacity * 256), 0, 256);
	int32 opacityB = 256 - opacityA;

	for (int32 py = yFrom; py < yTo; py++)
	{
		int32 *frameBuffer = &Window.Pixels[xFrom + py * Window.Width];

		for (int32 px = xFrom; px < xTo; px++)
		{
			int32 r = (*frameBuffer & 0xff0000) >> 16;
			int32 g = (*frameBuffer & 0xff00) >> 8;
			int32 b = *frameBuffer & 0xff;

			*frameBuffer++ =
				(r * opacityB + color.R * opacityA) >> 8 << 16 |
				(g * opacityB + color.G * opacityA) >> 8 << 8 |
				(b * opacityB + color.B * opacityA) >> 8;
		}
	}
}
void Graphics::DrawString(int32 x, int32 y, const char *text) const
{
	DrawString(x, y, text, Color(255, 255, 255));
}
void Graphics::DrawString(int32 x, int32 y, const char *text, const Color &color) const
{
	if (!Font) throw;

	int32 textLength = lstrlenA(text);

	for (int32 i = 0; i < textLength; i++)
	{
		char character = text[i];
		int32 characterX = (text[i] & 15) * Font->Width;
		int32 characterY = (text[i] >> 4) * Font->Height;
		Vector2i characterDimension = Font->CharacterDimensions[character];
		int32 characterWidth = characterDimension.Y - characterDimension.X;

		for (int32 py = 0; py < Font->Height; py++)
		{
			byte *bitmapPtr = &Font->Buffer[characterDimension.X + characterX + (py + characterY) * (Font->Width << 4)];
			int32 *frameBuffer = &Window.Pixels[x + (y + py) * Window.Width];

			for (int32 px = 0; px < characterWidth; px++)
			{
				int32 r = (*frameBuffer & 0xff0000) >> 16;
				int32 g = (*frameBuffer & 0xff00) >> 8;
				int32 b = *frameBuffer & 0xff;

				int32 opacityA = *bitmapPtr++;
				int32 opacityB = 255 - opacityA;

				*frameBuffer++ =
					(r * opacityB + color.R * opacityA) >> 8 << 16 |
					(g * opacityB + color.G * opacityA) >> 8 << 8 |
					(b * opacityB + color.B * opacityA) >> 8;
			}
		}

		x += characterWidth + Font->CharacterSpacing;
	}
}
int32 Graphics::MeasureString(const char *text) const
{
	if (!Font) throw;

	int32 textLength = lstrlenA(text);
	int32 width = 0;

	for (int32 i = 0; i < textLength; i++)
	{
		char character = text[i];
		width += Font->CharacterDimensions[character].Y - Font->CharacterDimensions[character].X;

		if (i < textLength - 1)
		{
			width += Font->CharacterSpacing;
		}
	}

	return width;
}