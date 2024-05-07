#pragma once
#include "FastPix3D.h"

enum class TextureType
{
	ColorMap,
	SphereMap
};

class Texture
{
private:
	static std::vector<Texture*> AllTextures;
	const char *_Path;
	TextureType _Type;
	int32 _Width;
	int32 _Height;
	int32 _MipLevels;
	int32 **_MipMaps;
	bool _HasTransparencyKey;

public:
	property_getset(TextureType, Type)
	{
		return _Type;
	}
	property_set(TextureType, Type)
	{
		_Type = value;
	}
	property_get(int32, Width)
	{
		return _Width;
	}
	property_get(int32, Height)
	{
		return _Height;
	}
	property_get(int32, MipLevels)
	{
		return _MipLevels;
	}
	property_get(int32**, MipMaps)
	{
		return _MipMaps;
	}
	property_get(bool, HasTransparencyKey)
	{
		return _HasTransparencyKey;
	}

	explicit Texture(int32 width, int32 height);
	~Texture();

	static Texture* Load(const char *path);

private:
	static Texture* GetExistingTexture(const char *path);
	static void ResizeImage(int32 *src, int32 srcWidth, int32 srcHeight, int32 *dest, int32 destWidth, int32 destHeight, bool bilinearFilter);
	static bool DetectTransparencyKey(int32 *image, int32 width, int32 height);
	void GenerateMipMaps();
};