#include "Texture.h"
#include "Math/Math_.h"

std::vector<Texture*> Texture::AllTextures;

Texture::Texture(int32 width, int32 height)
{
	if (width != Math::GetLogarithmicCeiling(width)) throw;
	if (height != Math::GetLogarithmicCeiling(height)) throw;

	_Path = nullptr;
	_FileChecksum = 0;
	_Type = TextureType::ColorMap;
	_Width = width;
	_Height = height;

	int32 bufferSize;
	GetMipmapParameters(_Width, _Height, _MipLevels, bufferSize);
	_Buffer = new Color[bufferSize];

	// All mipmaps are allocated in a single buffer to increase cache coherence.
	Color *mipMap = _Buffer;
	for (int32 y = 0; y <= _MipLevels; y++)
	{
		for (int32 x = 0; x <= _MipLevels; x++)
		{
			_MipMaps[y][x] = mipMap;
			mipMap += (_Width >> x) * (_Height >> y);
		}
	}

	_HasTransparencyKey = false;
}
Texture::~Texture()
{
	if (_Path) delete[] _Path;
	delete[] _Buffer;

	AllTextures.erase(std::remove(AllTextures.begin(), AllTextures.end(), this), AllTextures.end());
}

Texture* Texture::FromFile(const char *path)
{
	Texture *existingTexture = GetExistingTexture(path, 0);
	if (existingTexture)
	{
		// Do not load the same texture multiple times.
		// This can happen a lot, if the mesh has many surfaces with the same texture!

		return existingTexture;
	}

	Bitmap *bitmap = Bitmap::FromFile(path);
	Texture *texture = FromBitmap(bitmap);
	delete bitmap;

	char *pathCopy = new char[lstrlenA(path) + 1];
	lstrcpyA(pathCopy, path);
	texture->_Path = pathCopy;

	return texture;
}
Texture* Texture::FromMemory(const void *buffer, int32 size)
{
	uint32 checksum = Math::ComputeCRC32(buffer, size);

	Texture *existingTexture = GetExistingTexture(nullptr, checksum);
	if (existingTexture)
	{
		return existingTexture;
	}

	Bitmap *bitmap = Bitmap::FromMemory(buffer, size);
	Texture *texture = FromBitmap(bitmap);
	delete bitmap;

	texture->_FileChecksum = checksum;

	return texture;
}
Texture* Texture::FromBitmap(const Bitmap *bitmap)
{
	Texture *texture = new Texture(
		Math::GetLogarithmicCeiling(bitmap->Width),
		Math::GetLogarithmicCeiling(bitmap->Height)
	);

	texture->_HasTransparencyKey = DetectTransparencyKey(bitmap->Pixels, bitmap->Width, bitmap->Height);

	if (texture->_Width == bitmap->Width && texture->_Height == bitmap->Height)
	{
		memcpy(texture->_Buffer, bitmap->Pixels, texture->_Width * texture->_Height * 4);
	}
	else
	{
		// Resize texture to power of 2 width and height.
		ResizeImage(bitmap->Pixels, bitmap->Width, bitmap->Height, (Color*)texture->_Buffer, texture->_Width, texture->_Height, !texture->_HasTransparencyKey);
	}

	// Change transparency key ff00ff to 000000 and change 000000 to 010101.
	if (texture->_HasTransparencyKey)
	{
		int32 size = texture->_Width * texture->_Height;
		Color *ptr = texture->_Buffer;

		for (int32 i = 0; i < size; i++, ptr++)
		{
			if (ptr->RGB == 0xff00ff)
			{
				ptr->RGB = 0;
			}
			else if (ptr->RGB == 0)
			{
				ptr->RGB = 0x010101;
			}
		}
	}

	texture->GenerateMipMaps();

	AllTextures.push_back(texture);
	return texture;
}

Texture* Texture::GetExistingTexture(const char *path, uint32 checksum)
{
	for (Texture *texture : AllTextures)
	{
		if (path && texture->_Path && !lstrcmpiA(texture->_Path, path) ||
			checksum && texture->_FileChecksum == checksum)
		{
			// Texture was already loaded.
			return texture;
		}
	}

	return nullptr;
}
void Texture::ResizeImage(const Color *src, int32 srcWidth, int32 srcHeight, Color *dest, int32 destWidth, int32 destHeight, bool bilinearFilter)
{
	if (bilinearFilter)
	{
		for (int32 y = 0; y < destHeight; y++)
		{
			for (int32 x = 0; x < destWidth; x++)
			{
				float u = x * srcWidth / (float)destWidth - .5f;
				float v = y * srcHeight / (float)destHeight - .5f;

				int32 u0 = (int32)Math::Floor(u);
				int32 v0 = (int32)Math::Floor(v);
				int32 u1 = (u0 + 1) % srcWidth;
				int32 v1 = (v0 + 1) % srcHeight;

				int32 du0 = (int32)((u - u0) * 255);
				int32 dv0 = (int32)((v - v0) * 255);
				int32 du1 = 255 - du0;
				int32 dv1 = 255 - dv0;

				u0 = Math::Max(u0 % srcWidth, 0);
				v0 = Math::Max(v0 % srcHeight, 0);

				const Color *rgb00 = &src[u0 + v0 * srcWidth];
				const Color *rgb01 = &src[u0 + v1 * srcWidth];
				const Color *rgb10 = &src[u1 + v0 * srcWidth];
				const Color *rgb11 = &src[u1 + v1 * srcWidth];

				dest->B = ((rgb00->B * du1 + rgb10->B * du0) * dv1 + (rgb01->B * du1 + rgb11->B * du0) * dv0) >> 16;
				dest->G = ((rgb00->G * du1 + rgb10->G * du0) * dv1 + (rgb01->G * du1 + rgb11->G * du0) * dv0) >> 16;
				dest->R = ((rgb00->R * du1 + rgb10->R * du0) * dv1 + (rgb01->R * du1 + rgb11->R * du0) * dv0) >> 16;
				dest++;
			}
		}
	}
	else
	{
		for (int32 y = 0; y < destHeight; y++)
		{
			int32 row = y * srcHeight / destHeight * srcWidth;
			for (int32 x = 0; x < destWidth; x++)
			{
				*dest++ = src[x * srcWidth / destWidth + row];
			}
		}
	}
}
bool Texture::DetectTransparencyKey(const Color *image, int32 width, int32 height)
{
	int32 size = width * height;
	for (int32 i = 0; i < size; i++, image++)
	{
		if (image->RGB == 0xff00ff)
		{
			return true;
		}
	}

	return false;
}
void Texture::GetMipmapParameters(int32 width, int32 height, int32 &mipLevels, int32 &bufferSize)
{
	mipLevels = 0;
	int32 bufferSizeX = width;
	int32 bufferSizeY = height;

	for (int32 i = 1; i < 8; i++)
	{
		width >>= 1;
		height >>= 1;

		if (width == 0 || height == 0)
		{
			// A texture may have less than 8 mipmaps.
			break;
		}

		mipLevels++;
		bufferSizeX += width;
		bufferSizeY += height;
	}

	bufferSize = bufferSizeX * bufferSizeY;
}
void Texture::GenerateMipMaps()
{
	// Generate anisotropic mimpaps of up to 8 levels per dimension.

	for (int32 mipY = 0; mipY <= _MipLevels; mipY++)
	{
		if (mipY > 0)
		{
			int32 mipWidth = _Width;
			int32 mipHeight = _Height >> mipY;

			Color *mipMap = _MipMaps[mipY][0];
			Color *previousMipMap = _MipMaps[mipY - 1][0];

			for (int32 y = 0; y < mipHeight; y++)
			{
				for (int32 x = 0; x < mipWidth; x++)
				{
					// Average two pixels from previous mipmap.
					Color* rgb00 = &previousMipMap[x + (y << 1) * mipWidth];
					Color* rgb01 = &previousMipMap[x + ((y << 1) + 1) * mipWidth];

					if (rgb00->RGB == 0 && rgb01->RGB == 0)
					{
						// Both texels are transparent.
						mipMap->RGB = 0;
					}
					else if (rgb00->RGB != 0 && rgb01->RGB != 0)
					{
						// Both texels are opaque.
						mipMap->B = (rgb00->B + rgb01->B) >> 1;
						mipMap->G = (rgb00->G + rgb01->G) >> 1;
						mipMap->R = (rgb00->R + rgb01->R) >> 1;
					}
					else if (rgb00->RGB != 0)
					{
						mipMap->RGB = rgb00->RGB;
					}
					else
					{
						mipMap->RGB = rgb01->RGB;
					}

					mipMap++;
				}
			}
		}

		for (int32 mipX = 1; mipX <= _MipLevels; mipX++)
		{
			int32 mipWidth = _Width >> mipX;
			int32 mipHeight = _Height >> mipY;

			Color *mipMap = _MipMaps[mipY][mipX];
			Color *previousMipMap = _MipMaps[mipY][mipX - 1];

			for (int32 y = 0; y < mipHeight; y++)
			{
				for (int32 x = 0; x < mipWidth; x++)
				{
					// Average two pixels from previous mipmap.
					Color* rgb00 = &previousMipMap[(x << 1) + y * (mipWidth << 1)];
					Color* rgb10 = &previousMipMap[(x << 1) + 1 + y * (mipWidth << 1)];

					if (rgb00->RGB == 0 && rgb10->RGB == 0)
					{
						// Both texels are transparent.
						mipMap->RGB = 0;
					}
					else if (rgb00->RGB != 0 && rgb10->RGB != 0)
					{
						// Both texels are opaque.
						mipMap->B = (rgb00->B + rgb10->B) >> 1;
						mipMap->G = (rgb00->G + rgb10->G) >> 1;
						mipMap->R = (rgb00->R + rgb10->R) >> 1;
					}
					else if (rgb00->RGB != 0)
					{
						mipMap->RGB = rgb00->RGB;
					}
					else
					{
						mipMap->RGB = rgb10->RGB;
					}

					mipMap++;
				}
			}
		}
	}
}