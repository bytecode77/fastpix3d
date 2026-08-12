#include "Graphics.h"
#include "../Math/Math_.h"

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

	if (alpha >= 255.0f / 256.0f)
	{
		FillSpan(&_Pixels[x + y * _Width], length, color);
	}
	else
	{
		FillSpan(&_Pixels[x + y * _Width], length, color, alpha);
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

	if (alpha >= 255.0f / 256.0f)
	{
		for (int32 i = 0; i < length; i++)
		{
			FillPixel(frameBuffer, color);
			frameBuffer += _Width;
		}
	}
	else
	{
		int32 srcR = color.R;
		int32 srcG = color.G;
		int32 srcB = color.B;
		int32 destAlpha;
		ComputeSrcDestAlpha(alpha, srcR, srcG, srcB, destAlpha);

		for (int32 i = 0; i < length; i++)
		{
			FillPixel(frameBuffer, srcR, srcG, srcB, destAlpha);
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

	cornerRadius = Math::Min(cornerRadius, Math::Min(width, height) >> 1);
	int32 cornerRadiusSquared = cornerRadius * cornerRadius;

	DrawHorizontalLine(x + cornerRadius, y, width - cornerRadius - cornerRadius, color, alpha);
	DrawHorizontalLine(x + cornerRadius, y + height - 1, width - cornerRadius - cornerRadius, color, alpha);
	DrawVerticalLine(x, y + cornerRadius, height - cornerRadius - cornerRadius, color, alpha);
	DrawVerticalLine(x + width - 1, y + cornerRadius, height - cornerRadius - cornerRadius, color, alpha);

	int32 previousLength = 0;

	for (int32 i = 1; i < cornerRadius; i++)
	{
		int32 yOffset = cornerRadius - i;
		int32 length = (int32)Math::Floor(Math::Sqrt((float)(cornerRadiusSquared - yOffset * yOffset)));
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
	FillRectangle<false>(x, y, width, height, color, 1);
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const
{
	if (alpha >= 255.0f / 256.0f)
	{
		FillRectangle<false>(x, y, width, height, color, alpha);
	}
	else
	{
		FillRectangle<true>(x, y, width, height, color, alpha);
	}
}
void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha, int32 cornerRadius) const
{
	if (alpha >= 255.0f / 256.0f)
	{
		FillRectangle<false>(x, y, width, height, color, alpha, cornerRadius);
	}
	else
	{
		FillRectangle<true>(x, y, width, height, color, alpha, cornerRadius);
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

	int32 pyFrom = Math::Max(0, -y);
	int32 pyTo = Math::Min(font.Height, _Height - y);
	bool yInside = y >= 0 && y + font.Height <= _Height;

	int32 r = color.R;
	int32 g = color.G;
	int32 b = color.B;
	vushort16 colorBroadcast = vuint8(color.RGB).Low8;

	while (*text && x < _Width)
	{
		char c = *text++;

		if (font.HasChar(c))
		{
			int32 charIndex = c - font.StartChar;
			int32 charBufferOffset = font.Glyphs[charIndex].Offset;
			int32 charWidth = font.MeasureChar(c);

			if (charWidth > 0 && x > -charWidth)
			{
				if (x >= 0 && x + charWidth <= _Width && yInside)
				{
					// Character is fully inside the screen.
					Color *frameBuffer = &_Pixels[x + y * _Width];
					const byte *fontBuffer = &font.Buffer[charBufferOffset];
					int32 stride = _Width - charWidth;

					for (int32 py = 0; py < font.Height; py++)
					{
						int32 count = charWidth;

						while (count >= 8)
						{
							vuint8 srcAlpha = vuint8(_mm256_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)fontBuffer)));
							vbyte32 alpha = VectorMath::Shuffle((vbyte32)srcAlpha, BroadcastByteToInt32Mask);
							vbyte32 destAlpha = ~alpha;

							vuint8 pixels = vuint8((uint32*)frameBuffer);

							vuint8::Write(
								(uint32*)frameBuffer,
								(vuint8)VectorMath::Pack(
									(colorBroadcast * alpha.Low8 + pixels.Low8 * destAlpha.Low8) >> 8,
									(colorBroadcast * alpha.High8 + pixels.High8 * destAlpha.High8) >> 8
								)
							);

							fontBuffer += 8;
							frameBuffer += 8;
							count -= 8;
						}

						while (count--)
						{
							int32 srcAlpha = *fontBuffer++;
							int32 destAlpha = 255 - srcAlpha;

							frameBuffer->R = (frameBuffer->R * destAlpha + r * srcAlpha) >> 8;
							frameBuffer->G = (frameBuffer->G * destAlpha + g * srcAlpha) >> 8;
							frameBuffer->B = (frameBuffer->B * destAlpha + b * srcAlpha) >> 8;
							frameBuffer++;
						}

						frameBuffer += stride;
					}
				}
				else
				{
					// Character overlaps the screen boundaries.
					int32 pxFrom = Math::Max(0, -x);
					int32 pxTo = Math::Min(charWidth, _Width - x);

					if (pxFrom < pxTo && pyFrom < pyTo)
					{
						int32 visibleWidth = pxTo - pxFrom;

						Color *frameBuffer = &_Pixels[x + pxFrom + (y + pyFrom) * _Width];
						const byte *fontBuffer = &font.Buffer[charBufferOffset + pyFrom * charWidth + pxFrom];
						int32 frameBufferStride = _Width - visibleWidth;
						int32 fontBufferStride = charWidth - visibleWidth;

						for (int32 py = pyFrom; py < pyTo; py++)
						{
							int32 count = visibleWidth;

							while (count >= 8)
							{
								vuint8 srcAlpha = vuint8(_mm256_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)fontBuffer)));
								vbyte32 alpha = VectorMath::Shuffle((vbyte32)srcAlpha, BroadcastByteToInt32Mask);
								vbyte32 destAlpha = ~alpha;

								vuint8 pixels = vuint8((uint32*)frameBuffer);

								vuint8::Write(
									(uint32*)frameBuffer,
									(vuint8)VectorMath::Pack(
										(colorBroadcast * alpha.Low8 + pixels.Low8 * destAlpha.Low8) >> 8,
										(colorBroadcast * alpha.High8 + pixels.High8 * destAlpha.High8) >> 8
									)
								);

								fontBuffer += 8;
								frameBuffer += 8;
								count -= 8;
							}

							while (count--)
							{
								int32 srcAlpha = *fontBuffer++;
								int32 destAlpha = 255 - srcAlpha;

								frameBuffer->R = (frameBuffer->R * destAlpha + r * srcAlpha) >> 8;
								frameBuffer->G = (frameBuffer->G * destAlpha + g * srcAlpha) >> 8;
								frameBuffer->B = (frameBuffer->B * destAlpha + b * srcAlpha) >> 8;
								frameBuffer++;
							}

							frameBuffer += frameBufferStride;
							fontBuffer += fontBufferStride;
						}
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

	int32 width = 0;
	bool hasText = false;

	while (*text)
	{
		char c = *text++;

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

template<bool hasTransparency>
__forceinline void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha) const
{
	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	int32 xFrom = Math::Clamp(x, 0, _Width);
	int32 yFrom = Math::Clamp(y, 0, _Height);
	int32 xTo = Math::Clamp(x + width, 0, _Width);
	int32 yTo = Math::Clamp(y + height, 0, _Height);

	int32 srcR = color.R;
	int32 srcG = color.G;
	int32 srcB = color.B;
	int32 destAlpha = 0;
	if constexpr (hasTransparency)
	{
		ComputeSrcDestAlpha(alpha, srcR, srcG, srcB, destAlpha);
	}

	Color *frameBuffer = &_Pixels[xFrom + yFrom * _Width];

	for (int32 py = yFrom; py < yTo; py++)
	{
		if constexpr (hasTransparency)
		{
			FillSpan(frameBuffer, xTo - xFrom, srcR, srcG, srcB, destAlpha);
		}
		else
		{
			FillSpan(frameBuffer, xTo - xFrom, color);
		}
		frameBuffer += _Width;
	}
}
template<bool hasTransparency>
__forceinline void Graphics::FillRectangle(int32 x, int32 y, int32 width, int32 height, const Color &color, float alpha, int32 cornerRadius) const
{
	if (cornerRadius <= 0)
	{
		FillRectangle<hasTransparency>(x, y, width, height, color, alpha);
		return;
	}

	if (alpha <= 0 || width == 0 || height == 0)
	{
		return;
	}

	cornerRadius = Math::Min(cornerRadius, Math::Min(width, height) >> 1);
	int32 cornerRadiusSquared = cornerRadius * cornerRadius;

	int32 xFrom = Math::Clamp(x, 0, _Width);
	int32 yFrom = Math::Clamp(y, 0, _Height);
	int32 xTo = Math::Clamp(x + width, 0, _Width);
	int32 yTo = Math::Clamp(y + height, 0, _Height);

	int32 srcR = color.R;
	int32 srcG = color.G;
	int32 srcB = color.B;
	int32 destAlpha = 0;
	if constexpr (hasTransparency)
	{
		ComputeSrcDestAlpha(alpha, srcR, srcG, srcB, destAlpha);
	}

	Color *frameBuffer = &_Pixels[yFrom * _Width];

	for (int32 py = yFrom; py < yTo; py++)
	{
		int32 lineFrom;
		int32 lineTo;

		if (py < y + cornerRadius)
		{
			int32 yOffset = cornerRadius - (py - y);
			int32 length = (int32)Math::Floor(Math::Sqrt((float)(cornerRadiusSquared - yOffset * yOffset)));

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


		if constexpr (hasTransparency)
		{
			FillSpan(&frameBuffer[lineFrom], lineTo - lineFrom, srcR, srcG, srcB, destAlpha);
		}
		else
		{
			FillSpan(&frameBuffer[lineFrom], lineTo - lineFrom, color);
		}

		frameBuffer += _Width;
	}
}

__forceinline void Graphics::ComputeSrcDestAlpha(float alpha, int32 &srcR, int32 &srcG, int32 &srcB, int32 &destAlpha)
{
	int32 srcAlpha = Math::Clamp((int32)(alpha * 256), 0, 256);
	destAlpha = 256 - srcAlpha;

	srcR *= srcAlpha;
	srcG *= srcAlpha;
	srcB *= srcAlpha;
}
__forceinline void Graphics::FillPixel(Color *dest, const Color &color)
{
	*dest = color;
}
__forceinline void Graphics::FillPixel(Color *dest, int32 srcR, int32 srcG, int32 srcB, int32 destAlpha)
{
	dest->R = (dest->R * destAlpha + srcR) >> 8;
	dest->B = (dest->B * destAlpha + srcB) >> 8;
	dest->G = (dest->G * destAlpha + srcG) >> 8;
}
__forceinline void Graphics::FillSpan(Color *dest, int32 count, const Color &color)
{
	if (count >= 8)
	{
		vuint8 colorBroadcast = vuint8(color.RGB);

		do
		{
			vuint8::Write((uint32*)dest, colorBroadcast);
			dest += 8;
			count -= 8;
		}
		while (count >= 8);
	}

	while (count--)
	{
		FillPixel(dest++, color);
	}
}
__forceinline void Graphics::FillSpan(Color *dest, int32 count, const Color &color, float alpha)
{
	int32 srcR = color.R;
	int32 srcG = color.G;
	int32 srcB = color.B;
	int32 destAlpha;
	ComputeSrcDestAlpha(alpha, srcR, srcG, srcB, destAlpha);

	FillSpan(dest, count, srcR, srcG, srcB, destAlpha);
}
__forceinline void Graphics::FillSpan(Color *dest, int32 count, int32 srcR, int32 srcG, int32 srcB, int32 destAlpha)
{
	if (count >= 8)
	{
		vushort16 destAlphaBroadcast = vushort16(destAlpha);

		vushort16 src = vushort16(
			srcB, srcG, srcR, 0,
			srcB, srcG, srcR, 0,
			srcB, srcG, srcR, 0,
			srcB, srcG, srcR, 0
		);

		do
		{
			vuint8 rgb = vuint8((uint32*)dest);

			vuint8::Write(
				(uint32*)dest,
				(vuint8)VectorMath::Pack(
					(rgb.Low8 * destAlphaBroadcast + src) >> 8,
					(rgb.High8 * destAlphaBroadcast + src) >> 8
				)
			);

			dest += 8;
			count -= 8;
		}
		while (count >= 8);
	}

	while (count--)
	{
		FillPixel(dest++, srcR, srcG, srcB, destAlpha);
	}
}