#pragma once
#include "FastPix3D.h"

struct Vector3i
{
	int32 X;
	int32 Y;
	int32 Z;

	property_get(float, Length)
	{
		return sqrtf(X * X + Y * Y + Z * Z);
	}

	Vector3i() :
		X(0),
		Y(0),
		Z(0)
	{
	}
	explicit Vector3i(int32 uniform) :
		X(uniform),
		Y(uniform),
		Z(uniform)
	{
	}
	explicit Vector3i(int32 x, int32 y, int32 z) :
		X(x),
		Y(y),
		Z(z)
	{
	}

	int32 DotProduct(const Vector3i &other) const
	{
		return X * other.X + Y * other.Y + Z * other.Z;
	}
	Vector3i CrossProduct(const Vector3i &other) const
	{
		return Vector3i(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);
	}

	Vector3i operator +(const Vector3i &other) const
	{
		return Vector3i(X + other.X, Y + other.Y, Z + other.Z);
	}
	Vector3i operator -(const Vector3i &other) const
	{
		return Vector3i(X - other.X, Y - other.Y, Z - other.Z);
	}
	Vector3i operator -() const
	{
		return Vector3i(-X, -Y, -Z);
	}
	Vector3i operator *(const Vector3i &other) const
	{
		return Vector3i(X * other.X, Y * other.Y, Z * other.Z);
	}
	Vector3i operator *(int32 scalar) const
	{
		return Vector3i(X * scalar, Y * scalar, Z * scalar);
	}
	Vector3i operator /(const Vector3i &other) const
	{
		return Vector3i(X / other.X, Y / other.Y, Z / other.Z);
	}
	Vector3i operator /(int32 scalar) const
	{
		return Vector3i(X / scalar, Y / scalar, Z / scalar);
	}
	Vector3i& operator +=(const Vector3i &other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	Vector3i& operator -=(const Vector3i &other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	Vector3i& operator *=(const Vector3i &other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	}
	Vector3i& operator *=(int32 scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}
	Vector3i& operator /=(const Vector3i &other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	}
	Vector3i& operator /=(int32 scalar)
	{
		X /= scalar;
		Y /= scalar;
		Z /= scalar;
		return *this;
	}
	bool operator ==(const Vector3i &other) const
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}
	bool operator !=(const Vector3i &other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
	}
};