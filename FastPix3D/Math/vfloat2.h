#pragma once
#include "../FastPix3D.h"

struct FASTPIX3D_API vfloat2
{
	float X;
	float Y;

	readonly_property(vfloat2, Reciprocal)
	{
		return vfloat2(1 / X, 1 / Y);
	}
	readonly_property(float, Length)
	{
		return sqrtf(X * X + Y * Y);
	}
	readonly_property(float, SquaredLength)
	{
		return X * X + Y * Y;
	}
	readonly_property(float, InverseLength)
	{
		return __rsqrt_ss(X * X + Y * Y);
	}
	readonly_property(float, InverseSquaredLength)
	{
		float inverseLength = __rsqrt_ss(X * X + Y * Y);
		return inverseLength * inverseLength;
	}

	__forceinline vfloat2() :
		X(0),
		Y(0)
	{
	}
	__forceinline vfloat2(const vfloat2 &value) :
		X(value.X),
		Y(value.Y)
	{
	}
	__forceinline explicit vfloat2(const float *ptr) :
		X(ptr[0]),
		Y(ptr[1])
	{
	}
	__forceinline explicit vfloat2(float uniform) :
		X(uniform),
		Y(uniform)
	{
	}
	__forceinline explicit vfloat2(float x, float y) :
		X(x),
		Y(y)
	{
	}

	__forceinline vfloat2 Normalize() const
	{
		float inverseLength = InverseLength;
		return vfloat2(X * inverseLength, Y * inverseLength);
	}
	__forceinline float DotProduct(const vfloat2 &other) const
	{
		return X * other.X + Y * other.Y;
	}

	__forceinline vfloat2& operator =(const vfloat2 &other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}
	__forceinline vfloat2 operator +(const vfloat2 &other) const
	{
		return vfloat2(X + other.X, Y + other.Y);
	}
	__forceinline vfloat2 operator +(float scalar) const
	{
		return vfloat2(X + scalar, Y + scalar);
	}
	__forceinline vfloat2 operator -(const vfloat2 &other) const
	{
		return vfloat2(X - other.X, Y - other.Y);
	}
	__forceinline vfloat2 operator -(float scalar) const
	{
		return vfloat2(X - scalar, Y - scalar);
	}
	__forceinline vfloat2 operator -() const
	{
		return vfloat2(-X, -Y);
	}
	__forceinline vfloat2 operator *(const vfloat2 &other) const
	{
		return vfloat2(X * other.X, Y * other.Y);
	}
	__forceinline vfloat2 operator *(float scalar) const
	{
		return vfloat2(X * scalar, Y * scalar);
	}
	__forceinline vfloat2 operator /(const vfloat2 &other) const
	{
		return vfloat2(X / other.X, Y / other.Y);
	}
	__forceinline vfloat2 operator /(float scalar) const
	{
		return vfloat2(X / scalar, Y / scalar);
	}
	__forceinline vfloat2& operator +=(const vfloat2 &other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	__forceinline vfloat2& operator +=(float scalar)
	{
		X += scalar;
		Y += scalar;
		return *this;
	}
	__forceinline vfloat2& operator -=(const vfloat2 &other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	__forceinline vfloat2& operator -=(float scalar)
	{
		X -= scalar;
		Y -= scalar;
		return *this;
	}
	__forceinline vfloat2& operator *=(const vfloat2 &other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	__forceinline vfloat2& operator *=(float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
	__forceinline vfloat2& operator /=(const vfloat2 &other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	__forceinline vfloat2& operator /=(float scalar)
	{
		X /= scalar;
		Y /= scalar;
		return *this;
	}
	__forceinline bool operator ==(const vfloat2 &other) const
	{
		return X == other.X && Y == other.Y;
	}
	__forceinline bool operator ==(float scalar) const
	{
		return X == scalar && Y == scalar;
	}
	__forceinline bool operator !=(const vfloat2 &other) const
	{
		return X != other.X || Y != other.Y;
	}
	__forceinline bool operator !=(float scalar) const
	{
		return X != scalar || Y != scalar;
	}
	__forceinline bool operator <(const vfloat2 &other) const
	{
		return X < other.X && Y < other.Y;
	}
	__forceinline bool operator <(float scalar) const
	{
		return X < scalar && Y < scalar;
	}
	__forceinline bool operator >(const vfloat2 &other) const
	{
		return X > other.X && Y > other.Y;
	}
	__forceinline bool operator >(float scalar) const
	{
		return X > scalar && Y > scalar;
	}
	__forceinline bool operator <=(const vfloat2 &other) const
	{
		return X <= other.X && Y <= other.Y;
	}
	__forceinline bool operator <=(float scalar) const
	{
		return X <= scalar && Y <= scalar;
	}
	__forceinline bool operator >=(const vfloat2 &other) const
	{
		return X >= other.X && Y >= other.Y;
	}
	__forceinline bool operator >=(float scalar) const
	{
		return X >= scalar && Y >= scalar;
	}
};