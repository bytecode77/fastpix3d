#include "FastPix3D.h"

void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, Color color)
{
	FillRectangle(x, y, width, height, color, 1);
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, Color color, float opacity)
{
	x = min(max(x, 0), Device::Width - 1);
	y = min(max(y, 0), Device::Height - 1);
	opacity = min(max(opacity, 0), 1);
	width += x;
	height += y;
	int32 opacityA = int32(opacity * 256);
	int32 opacityB = 256 - opacityA;

	for (int32 y2 = y; y2 < height; y2++)
	{
		for (int32 x2 = x, *backBuffer = &Device::BackBuffer[y2 * Device::Width + x2]; x2 < width; x2++)
		{
			int32 r = (*backBuffer & 0xff0000) >> 16;
			int32 g = (*backBuffer & 0xff00) >> 8;
			int32 b = *backBuffer & 0xff;
			*backBuffer++ = (r * opacityB + color.R * opacityA) >> 8 << 16 | (g * opacityB + color.B * opacityA) >> 8 << 8 | (b * opacityB + color.B * opacityA) >> 8;
		}
	}
}
void Graphics::DrawString(int32 x, int32 y, string text, Font *font)
{
	DrawString(x, y, text, font, Color(255, 255, 255));
}
void Graphics::DrawString(int32 x, int32 y, string text, Font *font, Color color)
{
	int32 textLength = int32(text.length());
	int32 *backBuffer = Device::BackBuffer;
	byte *bitmapPtr = font->Buffer;

	for (int32 i = 0, x2 = x; i < textLength; i++)
	{
		char character = text[i];
		int32 characterX = (text[i] & 15) * font->Width, characterY = (text[i] >> 4) * font->Height;
		Point characterDimension = font->CharacterDimensions[character];
		int32 characterWidth = characterDimension.Y - characterDimension.X;

		for (int32 py = 0; py < font->Height; py++)
		{
			for (int32 px = 0, *backBufferPtr = &backBuffer[x2 + (y + py) * Device::Width]; px < characterWidth; px++)
			{
				int32 r = (*backBufferPtr & 0xff0000) >> 16;
				int32 g = (*backBufferPtr & 0xff00) >> 8;
				int32 b = *backBufferPtr & 0xff;
				int32 opacityA = bitmapPtr[(px + characterDimension.X + characterX) + (py + characterY) * (font->Width << 4)];
				int32 opacityB = 255 - opacityA;
				*backBufferPtr++ = (r * opacityB + color.R * opacityA) >> 8 << 16 | int32(g * opacityB + color.G * opacityA) >> 8 << 8 | (b * opacityB + color.B * opacityA) >> 8;
			}
		}
		x2 += characterWidth + font->CharacterSpacing;
	}
}