#include "Graphics.h"
#include "../Math/Color.h"
#include "../Math/Math_.h"
#include "../Math/VectorMath.h"

Graphics::Graphics(const ::Window &window) :
	_Width(window.Width),
	_Height(window.Height),
	_Pixels(window.Pixels)
{
}

void Graphics::DrawHorizontalLine(int32 x, int32 y, int32 length, const Color &color) const
{
	DrawHorizontalLine(x, y, length, color, 1);
}
void Graphics::DrawHorizontalLine(int32 x, int32 y, int32 length, const Color &color, float alpha) const
{
	if (alpha <= 0 || length == 0)
	{
		return;
	}

	if (length < 0)
	{
		x += length;
		length = -length;
	}

	if (x + length < 0 || x >= _Width || y < 0 || y >= _Height)
	{
		return;
	}

	if (x < 0)
	{
		length += x;
		x = 0;
	}

	if (x + length > _Width)
	{
		length = _Width - x;
	}

	Color *frameBuffer = &_Pixels[x + y * _Width];

	if (alpha >= 1)
	{
		for (int32 i = 0; i < length; i++)
		{
			*frameBuffer++ = color;
		}
	}
	else
	{
		int32 alphaA = Math::Clamp((int32)(alpha * 256), 0, 256);
		int32 alphaB = 256 - alphaA;

		for (int32 i = 0; i < length; i++)
		{
			frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
			frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
			frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
			frameBuffer++;
		}
	}
}
void Graphics::DrawVerticalLine(int32 x, int32 y, int32 length, const Color &color) const
{
	DrawVerticalLine(x, y, length, color, 1);
}
void Graphics::DrawVerticalLine(int32 x, int32 y, int32 length, const Color &color, float alpha) const
{
	if (alpha <= 0 || length == 0)
	{
		return;
	}

	if (length < 0)
	{
		y += length;
		length = -length;
	}

	if (x < 0 || x >= _Width || y + length < 0 || y >= _Height)
	{
		return;
	}

	if (y < 0)
	{
		length += y;
		y = 0;
	}

	if (y + length > _Height)
	{
		length = _Height - y;
	}

	Color *frameBuffer = &_Pixels[x + y * _Width];

	if (alpha >= 1)
	{
		for (int32 i = 0; i < length; i++)
		{
			*frameBuffer = color;
			frameBuffer += _Width;
		}
	}
	else
	{
		int32 alphaA = Math::Clamp((int32)(alpha * 256), 0, 256);
		int32 alphaB = 256 - alphaA;

		for (int32 i = 0; i < length; i++)
		{
			frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
			frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
			frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
			frameBuffer += _Width;
		}
	}
}
void Graphics::DrawRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const
{
	DrawRectangle(x, y, width, height, color, 1);
}
void Graphics::DrawRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const
{
	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	DrawHorizontalLine(x, y, width, color, alpha);
	DrawHorizontalLine(x, y + height - 1, width, color, alpha);
	DrawVerticalLine(x, y + 1, height - 2, color, alpha);
	DrawVerticalLine(x + width - 1, y + 1, height - 2, color, alpha);
}
void Graphics::DrawRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha, int32 cornerRadius) const
{
	if (cornerRadius <= 0)
	{
		DrawRectangle(x, y, width, height, color, alpha);
		return;
	}

	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	if (width == 1)
	{
		DrawVerticalLine(x, y, height, color, alpha);
		return;
	}

	if (height == 1)
	{
		DrawHorizontalLine(x, y, width, color, alpha);
		return;
	}

	cornerRadius = Math::Min(cornerRadius, Math::Min(width, height) / 2);
	int32 cornerRadiusSquared = cornerRadius * cornerRadius;

	DrawHorizontalLine(x + cornerRadius, y, width - cornerRadius - cornerRadius, color, alpha);
	DrawHorizontalLine(x + cornerRadius, y + height - 1, width - cornerRadius - cornerRadius, color, alpha);
	DrawVerticalLine(x, y + cornerRadius, height - cornerRadius - cornerRadius, color, alpha);
	DrawVerticalLine(x + width - 1, y + cornerRadius, height - cornerRadius - cornerRadius, color, alpha);

	int32 previousLength = 0;

	for (int32 i = 1; i < cornerRadius; i++)
	{
		int32 yOffset = cornerRadius - i;
		int32 length = (int32)Math::Floor(Math::Sqrt(cornerRadiusSquared - yOffset * yOffset));
		int32 segmentLength = length - previousLength;

		if (segmentLength > 0)
		{
			DrawHorizontalLine(x + cornerRadius - length, y + i, segmentLength, color, alpha);
			DrawHorizontalLine(x + width - cornerRadius + previousLength, y + i, segmentLength, color, alpha);
			DrawHorizontalLine(x + cornerRadius - length, y + height - i - 1, segmentLength, color, alpha);
			DrawHorizontalLine(x + width - cornerRadius + previousLength, y + height - i - 1, segmentLength, color, alpha);
		}
		else
		{
			DrawHorizontalLine(x + cornerRadius - length, y + i, 1, color, alpha);
			DrawHorizontalLine(x + width - cornerRadius + length - 1, y + i, 1, color, alpha);
			DrawHorizontalLine(x + cornerRadius - length, y + height - i - 1, 1, color, alpha);
			DrawHorizontalLine(x + width - cornerRadius + length - 1, y + height - i - 1, 1, color, alpha);
		}

		previousLength = length;
	}
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color) const
{
	FillRectangle(x, y, width, height, color, 1);
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const
{
	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	int32 xFrom = Math::Clamp(x, 0, _Width);
	int32 yFrom = Math::Clamp(y, 0, _Height);
	int32 xTo = Math::Clamp(x + width, 0, _Width);
	int32 yTo = Math::Clamp(y + height, 0, _Height);

	Color *frameBuffer = &_Pixels[xFrom + yFrom * _Width];
	int32 stride = _Width - xTo + xFrom;

	if (alpha >= 1)
	{
		for (int32 py = yFrom; py < yTo; py++)
		{
			for (int32 px = xFrom; px < xTo; px++)
			{
				*frameBuffer++ = color;
			}

			frameBuffer += stride;
		}
	}
	else
	{
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

			frameBuffer += stride;
		}
	}
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha, int32 cornerRadius) const
{
	if (cornerRadius <= 0)
	{
		FillRectangle(x, y, width, height, color, alpha);
		return;
	}

	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	cornerRadius = Math::Min(cornerRadius, Math::Min(width, height) / 2);
	int32 cornerRadiusSquared = cornerRadius * cornerRadius;

	int32 xFrom = Math::Clamp(x, 0, _Width);
	int32 yFrom = Math::Clamp(y, 0, _Height);
	int32 xTo = Math::Clamp(x + width, 0, _Width);
	int32 yTo = Math::Clamp(y + height, 0, _Height);
	int32 alphaA = Math::Clamp((int32)(alpha * 256), 0, 256);
	int32 alphaB = 256 - alphaA;

	for (int32 py = yFrom; py < yTo; py++)
	{
		int32 lineFrom;
		int32 lineTo;

		if (py < y + cornerRadius)
		{
			int32 yOffset = cornerRadius - (py - y);
			int32 length = (int32)Math::Floor(Math::Sqrt(cornerRadiusSquared - yOffset * yOffset));

			lineFrom = x + cornerRadius - length;
			lineTo = x + width - cornerRadius + length;
		}
		else if (py >= y + height - cornerRadius)
		{
			int32 yOffset = cornerRadius - (y + height - 1 - py);
			int32 length = (int32)Math::Floor(Math::Sqrt((float)(cornerRadiusSquared - yOffset * yOffset)));

			lineFrom = x + cornerRadius - length;
			lineTo = x + width - cornerRadius + length;
		}
		else
		{
			lineFrom = xFrom;
			lineTo = xTo;
		}

		lineFrom = Math::Clamp(lineFrom, xFrom, xTo);
		lineTo = Math::Clamp(lineTo, xFrom, xTo);

		if (lineFrom < lineTo)
		{
			Color *frameBuffer = &_Pixels[lineFrom + py * _Width];

			if (alpha >= 1)
			{
				for (int32 px = lineFrom; px < lineTo; px++)
				{
					*frameBuffer++ = color;
				}
			}
			else
			{
				for (int32 px = lineFrom; px < lineTo; px++)
				{
					frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
					frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
					frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
					frameBuffer++;
				}
			}
		}
	}
}
void Graphics::DrawString(int32 x, int32 y, const Font &font, const char *text) const
{
	DrawString(x, y, font, text, Color(255, 255, 255));
}
void Graphics::DrawString(int32 x, int32 y, const Font &font, const char *text, const Color &color) const
{
	if (!text) throw std::invalid_argument("text cannot be null.");

	if (x >= _Width || y < -font.Height || y >= _Height)
	{
		return;
	}

	int32 length = lstrlenA(text);

	for (int32 i = 0; i < length; i++)
	{
		if (x >= _Width)
		{
			break;
		}

		char c = text[i];

		if (font.HasChar(c))
		{
			int32 charIndex = c - font.StartChar;
			int32 charBufferOffset = font.CharacterOffsets[charIndex];
			int32 charWidth = font.MeasureChar(c);

			if (charWidth > 0 && x >= -charWidth)
			{
				Color *frameBuffer = &_Pixels[x + y * _Width];
				int32 stride = Width - charWidth;

				byte *fontBuffer = &font.Buffer[charBufferOffset];

				if (x >= 0 && x + charWidth < _Width && y >= 0 && y + font.Height < _Height)
				{
					// Character is fully inside the screen.
					for (int32 py = 0; py < font.Height; py++)
					{
						for (int32 px = 0; px < charWidth; px++)
						{
							int32 alphaA = *fontBuffer++;
							int32 alphaB = 255 - alphaA;

							frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
							frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
							frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
							frameBuffer++;
						}

						frameBuffer += stride;
					}
				}
				else
				{
					// Character overlaps the screen boundaries.
					for (int32 py = 0; py < font.Height; py++)
					{
						for (int32 px = 0; px < charWidth; px++)
						{
							if (x + px >= 0 && x + px < _Width && y + py >= 0 && y + py < _Height)
							{
								int32 alphaA = *fontBuffer;
								int32 alphaB = 255 - alphaA;

								frameBuffer->B = (frameBuffer->B * alphaB + color.B * alphaA) >> 8;
								frameBuffer->G = (frameBuffer->G * alphaB + color.G * alphaA) >> 8;
								frameBuffer->R = (frameBuffer->R * alphaB + color.R * alphaA) >> 8;
							}

							fontBuffer++;
							frameBuffer++;
						}

						frameBuffer += stride;
					}
				}
			}

			x += charWidth + font.CharacterSpacing;
		}
	}
}
int32 Graphics::MeasureString(const Font &font, const char *text) const
{
	if (!text) throw std::invalid_argument("text cannot be null.");

	int32 length = lstrlenA(text);
	int32 width = 0;
	bool hasText = false;

	for (int32 i = 0; i < length; i++)
	{
		char c = text[i];

		if (font.HasChar(c))
		{
			width += font.MeasureChar(c) + font.CharacterSpacing;

			if (!hasText)
			{
				hasText = true;
				width -= font.CharacterSpacing;
			}
		}
	}

	return width;
}