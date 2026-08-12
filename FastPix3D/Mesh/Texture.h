#pragma once
#include "../FastPix3D.h"
#include "../Interop/Bitmap.h"
#include "../Math/Color.h"

enum class TextureType
{
	ColorMap,
	SphereMap
};

class FASTPIX3D_API Texture
{
private:
	static std::vector<Texture*> AllTextures;
	const char *_Path;
	uint32 _FileChecksum;
	TextureType _Type;
	int32 _Width;
	int32 _Height;
	int32 _WidthExponent;
	int32 _MipLevels;
	int32 _BufferSize;
	Color *_Buffer;
	Color *_MipMaps[8][8];
	bool _HasTransparencyKey;

public:
	property_get(TextureType, Type)
	{
		return _Type;
	}
	property_set(TextureType, Type)
	{
		_Type = value;
	}
	readonly_property(int32, Width)
	{
		return _Width;
	}
	readonly_property(int32, Height)
	{
		return _Height;
	}
	readonly_property(int32, WidthExponent)
	{
		return _WidthExponent;
	}
	readonly_property(int32, MipLevels)
	{
		return _MipLevels;
	}
	readonly_property(Color*, Mip0)
	{
		return _Buffer;
	}
	readonly_indexed_property2(Color*, MipMaps, int32 x, int32 y)
	{
		return _MipMaps[y][x];
	}
	readonly_property(bool, HasTransparencyKey)
	{
		return _HasTransparencyKey;
	}
	readonly_property(const char*, Path)
	{
		return _Path;
	}
	readonly_property(const char*, FileName)
	{
		return _Path ? strrchr(_Path, '\\') + 1 : nullptr;
	}

	explicit Texture(int32 width, int32 height);
	Texture(const Texture& other);
	~Texture();

	static Texture* FromFile(const char *path);
	static Texture* FromMemory(const void *buffer, int32 size);
	static Texture* FromBitmap(const Bitmap *bitmap);

private:
	static Texture* GetExistingTexture(const char *path);
	static Texture* GetExistingTexture(uint32 checksum);
	static void ResizeImage(const Color *src, int32 srcWidth, int32 srcHeight, Color *dest, int32 destWidth, int32 destHeight, bool bilinearFilter);
	static bool DetectTransparencyKey(const Color *image, int32 width, int32 height);
	static void GetMipmapParameters(int32 width, int32 height, int32 &mipLevels, int32 &bufferSize);
	void ComputeMipMapOffsets();
	void GenerateMipMaps();

public:
	Texture& operator =(const Texture &other);
};