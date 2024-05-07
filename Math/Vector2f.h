#pragma once
#include "FastPix3D.h"

struct Vector2f
{
	float X;
	float Y;

	property_get(float, Length)
	{
		return sqrtf(X * X + Y * Y);
	}

	Vector2f() :
		X(0),
		Y(0)
	{
	}
	explicit Vector2f(float uniform) :
		X(uniform),
		Y(uniform)
	{
	}
	explicit Vector2f(float x, float y) :
		X(x),
		Y(y)
	{
	}

	Vector2f Normalize() const
	{
		float length = Length;
		return Vector2f(X / length, Y / length);
	}
	float DotProduct(const Vector2f &other) const
	{
		return X * other.X + Y * other.Y;
	}

	Vector2f operator +(const Vector2f &other) const
	{
		return Vector2f(X + other.X, Y + other.Y);
	}
	Vector2f operator -(const Vector2f &other) const
	{
		return Vector2f(X - other.X, Y - other.Y);
	}
	Vector2f operator -() const
	{
		return Vector2f(-X, -Y);
	}
	Vector2f operator *(const Vector2f &other) const
	{
		return Vector2f(X * other.X, Y * other.Y);
	}
	Vector2f operator *(float scalar) const
	{
		return Vector2f(X * scalar, Y * scalar);
	}
	Vector2f operator /(const Vector2f &other) const
	{
		return Vector2f(X / other.X, Y / other.Y);
	}
	Vector2f operator /(float scalar) const
	{
		return Vector2f(X / scalar, Y / scalar);
	}
	Vector2f& operator +=(const Vector2f &other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	Vector2f& operator -=(const Vector2f &other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	Vector2f& operator *=(const Vector2f &other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	Vector2f& operator *=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
	Vector2f& operator /=(const Vector2f &other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	Vector2f& operator /=(float scalar)
	{
		X /= scalar;
		Y /= scalar;
		return *this;
	}
	bool operator ==(const Vector2f &other) const
	{
		return X == other.X && Y == other.Y;
	}
	bool operator !=(const Vector2f &other) const
	{
		return X != other.X || Y != other.Y;
	}
};