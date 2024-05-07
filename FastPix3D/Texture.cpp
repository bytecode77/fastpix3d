#include "Texture.h"
#include "Bitmap.h"
#include "Math_.h"

std::vector<Texture*> Texture::AllTextures;

Texture::Texture(int32 width, int32 height)
{
	if (width != Math::GetLogarithmicCeiling(width)) throw;
	if (height != Math::GetLogarithmicCeiling(height)) throw;

	_Path = nullptr;
	_Type = TextureType::ColorMap;
	_Width = width;
	_Height = height;
	_MipMaps = new int32 * [8]();
	_MipMaps[0] = new int32[_Width * _Height];

	_HasTransparencyKey = false;
	_MipLevels = 0;
}
Texture::~Texture()
{
	for (int32 i = 0; i < 8; i++)
	{
		if (_MipMaps[i])
		{
			delete[] _MipMaps[i];
		}
	}

	delete[] _Path;
	delete[] _MipMaps;

	AllTextures.erase(std::remove(AllTextures.begin(), AllTextures.end(), this), AllTextures.end());
}

Texture* Texture::Load(const char *path)
{
	Texture *existingTexture = GetExistingTexture(path);
	if (existingTexture)
	{
		// Some meshes have over 1000 surfaces and we don't want to load 1000 textures!
		// This would produce cache misses and slow down rendering significantly.

		return existingTexture;
	}

	Bitmap *bitmap = Bitmap::Load(path);

	Texture *texture = new Texture(
		Math::GetLogarithmicCeiling(bitmap->Width),
		Math::GetLogarithmicCeiling(bitmap->Height)
	);

	texture->_HasTransparencyKey = DetectTransparencyKey(bitmap->Pixels, bitmap->Width, bitmap->Height);

	if (texture->_Width == bitmap->Width && texture->_Height == bitmap->Height)
	{
		memcpy(texture->_MipMaps[0], bitmap->Pixels, texture->_Width * texture->_Height * 4);
	}
	else
	{
		// Resize texture to power of 2 width and height.
		ResizeImage(bitmap->Pixels, bitmap->Width, bitmap->Height, texture->_MipMaps[0], texture->_Width, texture->_Height, !texture->_HasTransparencyKey);
	}

	delete bitmap;

	char *pathCopy = new char[lstrlenA(path) + 1];
	lstrcpyA(pathCopy, path);
	texture->_Path = pathCopy;

	texture->GenerateMipMaps();

	AllTextures.push_back(texture);
	return texture;
}

Texture* Texture::GetExistingTexture(const char *path)
{
	for (Texture *texture : AllTextures)
	{
		if (!lstrcmpiA(texture->_Path, path))
		{
			// Texture was already loaded.
			return texture;
		}
	}

	return nullptr;
}
void Texture::ResizeImage(int32 *src, int32 srcWidth, int32 srcHeight, int32 *dest, int32 destWidth, int32 destHeight, bool bilinearFilter)
{
	for (int32 y = 0; y < destHeight; y++)
	{
		for (int32 x = 0; x < destWidth; x++)
		{
			if (bilinearFilter)
			{
				float u = x * srcWidth / (float)destWidth - .5f;
				float v = y * srcHeight / (float)destHeight - .5f;

				int32 x1 = (int32)Math::Floor(u);
				int32 y1 = (int32)Math::Floor(v);

				int32 u1 = (int32)((u - x1) * 255);
				int32 v1 = (int32)((v - y1) * 255);
				int32 u2 = 255 - u1;
				int32 v2 = 255 - v1;

				int32 x2 = (x1 + 1) % srcWidth;
				int32 y2 = (y1 + 1) % srcHeight;
				x1 = Math::Max(x1 % srcWidth, 0);
				y1 = Math::Max(y1 % srcHeight, 0);

				int32 rgb00 = src[x1 + y1 * srcWidth];
				int32 rgb01 = src[x1 + y2 * srcWidth];
				int32 rgb10 = src[x2 + y1 * srcWidth];
				int32 rgb11 = src[x2 + y2 * srcWidth];

				int32 r00 = (rgb00 & 0xff0000) >> 16;
				int32 g00 = (rgb00 & 0xff00) >> 8;
				int32 b00 = rgb00 & 0xff;
				int32 r01 = (rgb01 & 0xff0000) >> 16;
				int32 g01 = (rgb01 & 0xff00) >> 8;
				int32 b01 = rgb01 & 0xff;
				int32 r10 = (rgb10 & 0xff0000) >> 16;
				int32 g10 = (rgb10 & 0xff00) >> 8;
				int32 b10 = rgb10 & 0xff;
				int32 r11 = (rgb11 & 0xff0000) >> 16;
				int32 g11 = (rgb11 & 0xff00) >> 8;
				int32 b11 = rgb11 & 0xff;

				int32 r = ((r00 * u2 + r10 * u1) * v2 + (r01 * u2 + r11 * u1) * v1) >> 16;
				int32 g = ((g00 * u2 + g10 * u1) * v2 + (g01 * u2 + g11 * u1) * v1) >> 16;
				int32 b = ((b00 * u2 + b10 * u1) * v2 + (b01 * u2 + b11 * u1) * v1) >> 16;

				*dest++ = r << 16 | g << 8 | b;
			}
			else
			{
				*dest++ = src[x * srcWidth / destWidth + y * srcHeight / destHeight * srcWidth];
			}
		}
	}
}
bool Texture::DetectTransparencyKey(int32 *image, int32 width, int32 height)
{
	for (int32 y = 0; y < width * height; y++)
	{
		if (*image++ == 0xff00ff)
		{
			return true;
		}
	}

	return false;
}
void Texture::GenerateMipMaps()
{
	int32 mipWidth = _Width;
	int32 mipHeight = _Height;

	// Generate up to 7 mipmaps.
	for (int32 i = 1; i < 8; i++)
	{
		mipWidth >>= 1;
		mipHeight >>= 1;

		if (mipWidth == 0 || mipHeight == 0)
		{
			// A texture may have less than 7 mipmaps.
			break;
		}

		if (_MipMaps[i])
		{
			// Mipmaps were already generated.
			throw;
		}

		_MipLevels = i;
		_MipMaps[i] = new int32[mipWidth * mipHeight];
		int32 *mipMap = _MipMaps[i];
		int32 *previousMipMap = _MipMaps[i - 1];

		for (int32 y = 0; y < mipHeight; y++)
		{
			for (int32 x = 0; x < mipWidth; x++)
			{
				// Average 2x2 pixels from previous mipmap.
				int32 rgb00 = previousMipMap[(x << 1) + (y << 1) * (mipWidth << 1)];
				int32 rgb01 = previousMipMap[(x << 1) + ((y << 1) + 1) * (mipWidth << 1)];
				int32 rgb10 = previousMipMap[(x << 1) + 1 + (y << 1) * (mipWidth << 1)];
				int32 rgb11 = previousMipMap[(x << 1) + 1 + ((y << 1) + 1) * (mipWidth << 1)];

				if (rgb00 == 0xff00ff && rgb01 == 0xff00ff && rgb10 == 0xff00ff && rgb11 == 0xff00ff)
				{
					// All texels are transparent.
					*mipMap++ = 0xff00ff;
				}
				else
				{
					// Compute average of non-transparent texels.
					int32 r = 0;
					int32 g = 0;
					int32 b = 0;
					int32 count = 0;

					if (rgb00 != 0xff00ff)
					{
						r += (rgb00 >> 16) & 0xff;
						g += (rgb00 >> 8) & 0xff;
						b += rgb00 & 0xff;
						count++;
					}

					if (rgb01 != 0xff00ff)
					{
						r += (rgb01 >> 16) & 0xff;
						g += (rgb01 >> 8) & 0xff;
						b += rgb01 & 0xff;
						count++;
					}

					if (rgb10 != 0xff00ff)
					{
						r += (rgb10 >> 16) & 0xff;
						g += (rgb10 >> 8) & 0xff;
						b += rgb10 & 0xff;
						count++;
					}

					if (rgb11 != 0xff00ff)
					{
						r += (rgb11 >> 16) & 0xff;
						g += (rgb11 >> 8) & 0xff;
						b += rgb11 & 0xff;
						count++;
					}

					*mipMap++ = (r / count) << 16 | (g / count) << 8 | (b / count);
				}
			}
		}
	}
}