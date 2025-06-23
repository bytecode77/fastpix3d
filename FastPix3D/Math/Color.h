#pragma once
#include "../FastPix3D.h"
#include "vint3.h"
#include "vfloat3.h"
#include "vfloat4.h"

#undef RGB

struct FASTPIX3D_API Color
{
	union
	{
		struct
		{
			byte B;
			byte G;
			byte R;
		};
		int32 RGB;
	};

	__forceinline Color() :
		RGB(0)
	{
	}
	__forceinline Color(const Color &value) :
		RGB(value.RGB)
	{
	}
	__forceinline explicit Color(const vint3 &value) :
		R(value.X),
		G(value.Y),
		B(value.Z)
	{
	}
	__forceinline explicit Color(const vfloat3 &value) :
		R((byte)value.X),
		G((byte)value.Y),
		B((byte)value.Z)
	{
	}
	__forceinline explicit Color(byte r, byte g, byte b) :
		R(r),
		G(g),
		B(b)
	{
	}
	__forceinline explicit Color(int32 rgb) :
		RGB(rgb & 0xffffff)
	{
	}

	__forceinline Color& operator =(const Color &other)
	{
		RGB = other.RGB;
		return *this;
	}
	__forceinline Color operator +(const Color &other) const
	{
		return Color(R + other.R, G + other.G, B + other.B);
	}
	__forceinline Color operator *(float scalar) const
	{
		return Color((byte)(R * scalar), (byte)(G * scalar), (byte)(B * scalar));
	}
	__forceinline Color operator *(const vfloat3 &other) const
	{
		return Color((byte)(R * other.X), (byte)(G * other.Y), (byte)(B * other.Z));
	}
	__forceinline Color operator *(const vfloat4 &other) const
	{
		return Color((byte)(R * other[0]), (byte)(G * other[1]), (byte)(B * other[2]));
	}
	__forceinline Color operator /(float scalar) const
	{
		scalar = 1 / scalar;
		return Color((byte)(R * scalar), (byte)(G * scalar), (byte)(B * scalar));
	}
	__forceinline Color& operator +=(const Color &other)
	{
		R += other.R;
		G += other.G;
		B += other.B;
		return *this;
	}
	__forceinline Color& operator *=(float scalar)
	{
		R = (byte)(R * scalar);
		G = (byte)(G * scalar);
		B = (byte)(B * scalar);
		return *this;
	}
	__forceinline Color& operator *=(const vfloat3 &other)
	{
		R = (byte)(R * other.X);
		G = (byte)(G * other.Y);
		B = (byte)(B * other.Z);
		return *this;
	}
	__forceinline Color& operator *=(const vfloat4 &other)
	{
		R = (byte)(R * other[0]);
		G = (byte)(G * other[1]);
		B = (byte)(B * other[2]);
		return *this;
	}
	__forceinline Color& operator /=(float scalar)
	{
		scalar = 1 / scalar;
		R = (byte)(R * scalar);
		G = (byte)(G * scalar);
		B = (byte)(B * scalar);
		return *this;
	}
	__forceinline bool operator ==(const Color &other) const
	{
		return (RGB & 0xffffff) == (other.RGB & 0xffffff);
	}
	__forceinline bool operator !=(const Color &other) const
	{
		return (RGB & 0xffffff) != (other.RGB & 0xffffff);
	}
	__forceinline operator vint3() const
	{
		return vint3(R, G, B);
	}
	__forceinline operator vfloat3() const
	{
		return vfloat3(R, G, B);
	}
};