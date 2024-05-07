#pragma once
#include "FastPix3D.h"

struct Vector2i
{
	int32 X;
	int32 Y;

	property_get(float, Length)
	{
		return sqrtf(X * X + Y * Y);
	}

	Vector2i() :
		X(0),
		Y(0)
	{
	}
	explicit Vector2i(int32 uniform) :
		X(uniform),
		Y(uniform)
	{
	}
	explicit Vector2i(int32 x, int32 y) :
		X(x),
		Y(y)
	{
	}

	int32 DotProduct(const Vector2i &other) const
	{
		return X * other.X + Y * other.Y;
	}

	Vector2i operator +(const Vector2i &other) const
	{
		return Vector2i(X + other.X, Y + other.Y);
	}
	Vector2i operator -(const Vector2i &other) const
	{
		return Vector2i(X - other.X, Y - other.Y);
	}
	Vector2i operator -() const
	{
		return Vector2i(-X, -Y);
	}
	Vector2i operator *(const Vector2i &other) const
	{
		return Vector2i(X * other.X, Y * other.Y);
	}
	Vector2i operator *(int32 scalar) const
	{
		return Vector2i(X * scalar, Y * scalar);
	}
	Vector2i operator /(const Vector2i &other) const
	{
		return Vector2i(X / other.X, Y / other.Y);
	}
	Vector2i operator /(int32 scalar) const
	{
		return Vector2i(X / scalar, Y / scalar);
	}
	Vector2i& operator +=(const Vector2i &other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	Vector2i& operator -=(const Vector2i &other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	Vector2i& operator *=(const Vector2i &other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	Vector2i& operator *=(int32 scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
	Vector2i& operator /=(const Vector2i &other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	Vector2i& operator /=(int32 scalar)
	{
		X /= scalar;
		Y /= scalar;
		return *this;
	}
	bool operator ==(const Vector2i &other) const
	{
		return X == other.X && Y == other.Y;
	}
	bool operator !=(const Vector2i &other) const
	{
		return X != other.X || Y != other.Y;
	}
};