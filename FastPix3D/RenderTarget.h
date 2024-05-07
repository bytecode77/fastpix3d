#pragma once
#include "FastPix3D.h"
#include "Window.h"
#include "Texture.h"

class RenderTarget
{
private:
	int32 _Width;
	int32 _Height;
	void *_Buffer;

public:
	property_get(int32, Width)
	{
		return _Width;
	}
	property_get(int32, Height)
	{
		return _Height;
	}
	property_get(void*, Buffer)
	{
		return _Buffer;
	}

	RenderTarget() :
		_Width(0),
		_Height(0),
		_Buffer(nullptr)
	{
	}
	explicit RenderTarget(int32 width, int32 height, void *buffer) :
		_Width(width),
		_Height(height),
		_Buffer(buffer)
	{
	}
	explicit RenderTarget(const Window &window) :
		_Width(window.Width),
		_Height(window.Height),
		_Buffer(window.Pixels)
	{
	}
	explicit RenderTarget(const Texture &texture) :
		_Width(texture.Width),
		_Height(texture.Height),
		_Buffer(texture.MipMaps[0])
	{
	}

	template<class T>
	static RenderTarget Create(int32 width, int32 height)
	{
		return RenderTarget(width, height, new T[width * height]);
	}

	template<class T>
	T* GetBuffer() const
	{
		return (T*)_Buffer;
	}
	template<class T>
	T* GetBuffer(int32 offset) const
	{
		return &((T*)_Buffer)[offset];
	}
};