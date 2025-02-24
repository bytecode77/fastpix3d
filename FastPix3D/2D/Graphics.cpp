#include "Graphics.h"
#include "../Math/Color.h"
#include "../Math/Math_.h"
#include "../Math/VectorMath.h"

Graphics::Graphics(const ::Window &window) :
	_Width(window.Width),
	_Height(window.Height),
	_Pixels(window.Pixels),
	_Font(nullptr)
{
}

void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const
{
	FillRectangle(x, y, width, height, color, 1);
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const
{
	int32 xFrom = Math::Clamp(x, 0, _Width - 1);
	int32 yFrom = Math::Clamp(y, 0, _Width - 1);
	int32 xTo = Math::Clamp(x + width, 0, _Width - 1);
	int32 yTo = Math::Clamp(y + height, 0, _Width - 1);

	int32 alphaA = Math::Clamp((int32)(alpha * 256), 0, 256);
	int32 alphaB = 256 - alphaA;

	for (int32 py = yFrom; py < yTo; py++)
	{
		Color *frameBuffer = &_Pixels[xFrom + py * _Width];

		for (int32 px = xFrom; px < xTo; px++)
		{
			frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
			frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
			frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
			frameBuffer++;
		}
	}
}
void Graphics::DrawString(int32 x, int32 y, const char *text) const
{
	DrawString(x, y, text, Color(255, 255, 255));
}
void Graphics::DrawString(int32 x, int32 y, const char *text, const Color &color) const
{
	if (!_Font) throw;

	int32 textLength = lstrlenA(text);

	for (int32 i = 0; i < textLength; i++)
	{
		char character = text[i];
		int32 characterX = (text[i] & 15) * _Font->Width;
		int32 characterY = (text[i] >> 4) * _Font->Height;
		vint2 characterDimension = _Font->CharacterDimensions[character];
		int32 characterWidth = characterDimension.Y - characterDimension.X;

		for (int32 py = 0; py < _Font->Height; py++)
		{
			byte *bitmapPtr = &_Font->Buffer[characterDimension.X + characterX + (py + characterY) * (_Font->Width << 4)];
			Color *frameBuffer = &_Pixels[x + (y + py) * _Width];

			for (int32 px = 0; px < characterWidth; px++)
			{
				int32 alphaA = *bitmapPtr++;
				int32 alphaB = 255 - alphaA;

				frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
				frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
				frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
				frameBuffer++;
			}
		}

		x += characterWidth + _Font->CharacterSpacing;
	}
}
int32 Graphics::MeasureString(const char *text) const
{
	if (!_Font) throw;

	int32 textLength = lstrlenA(text);
	int32 width = 0;

	for (int32 i = 0; i < textLength; i++)
	{
		char character = text[i];
		width += _Font->CharacterDimensions[character].Y - _Font->CharacterDimensions[character].X;

		if (i < textLength - 1)
		{
			width += _Font->CharacterSpacing;
		}
	}

	return width;
}