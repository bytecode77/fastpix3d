#pragma once
#include "FastPix3D.h"
#include "Interop/Window.h"
#include "Mesh/Texture.h"

class FASTPIX3D_API RenderTarget
{
private:
	int32 _Width;
	int32 _Height;
	int32 _WidthExponent;
	void *_Buffer;

public:
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
	readonly_property(void*, Buffer)
	{
		return _Buffer;
	}

	RenderTarget() :
		_Width(0),
		_Height(0),
		_WidthExponent(0),
		_Buffer(nullptr)
	{
	}
	explicit RenderTarget(int32 width, int32 height, void *buffer) :
		_Width(width),
		_Height(height),
		_WidthExponent(Math::GetExponent(width)),
		_Buffer(buffer)
	{
		AssertAlignment();
	}
	explicit RenderTarget(const Window &window) :
		_Width(window.Width),
		_Height(window.Height),
		_WidthExponent(Math::GetExponent(window.Width)),
		_Buffer(window.Pixels)
	{
		AssertAlignment();
	}
	explicit RenderTarget(const Texture &texture) :
		_Width(texture.Width),
		_Height(texture.Height),
		_WidthExponent(texture.WidthExponent),
		_Buffer(texture.Mip0)
	{
		AssertAlignment();
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

private:
	void AssertAlignment()
	{
		// Pixels must be aligned to 32 bytes to be used with AVX2.
		if ((int64)_Buffer & 31) throw std::runtime_error("RenderTarget buffer must be aligned to 32 bytes.");
	}
};