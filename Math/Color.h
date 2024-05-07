#pragma once
#include "FastPix3D.h"
#include "Vector3f.h"

struct Color
{
	byte R;
	byte G;
	byte B;

	property_get(int32, RGB)
	{
		return R << 16 | G << 8 | B;
	}

	Color() :
		R(0),
		G(0),
		B(0)
	{
	}
	explicit Color(byte r, byte g, byte b) :
		R(r),
		G(g),
		B(b)
	{
	}
	explicit Color(int32 rgb) :
		R((rgb >> 16) & 0xff),
		G((rgb >> 8) & 0xff),
		B(rgb & 0xff)
	{
	}

	Color operator +(const Color &other) const
	{
		return Color(R + other.R, G + other.G, B + other.B);
	}
	Color operator *(float scalar) const
	{
		return Color((byte)(R * scalar), (byte)(G * scalar), (byte)(B * scalar));
	}
	Color operator /(float scalar) const
	{
		return Color((byte)(R / scalar), (byte)(G / scalar), (byte)(B / scalar));
	}
	Color& operator +=(const Color &other)
	{
		R += other.R;
		G += other.G;
		B += other.B;
		return *this;
	}
	Color& operator *=(float scalar)
	{
		R = (byte)(R * scalar);
		G = (byte)(G * scalar);
		B = (byte)(B * scalar);
		return *this;
	}
	Color& operator /=(float scalar)
	{
		R = (byte)(R / scalar);
		G = (byte)(G / scalar);
		B = (byte)(B / scalar);
		return *this;
	}
	bool operator ==(const Color &other) const
	{
		return R == other.R && G == other.G && B == other.B;
	}
	bool operator !=(const Color &other) const
	{
		return R != other.R || G != other.G || B != other.B;
	}
	operator Vector3f() const
	{
		return Vector3f(R, G, B);
	}
};