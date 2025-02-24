#pragma once
#include "../FastPix3D.h"
#include "vfloat2.h"

struct FASTPIX3D_API vlong2
{
	int64 X;
	int64 Y;

	readonly_property(float, Length)
	{
		return sqrtf((float)(X * X + Y * Y));
	}
	readonly_property(int64, SquaredLength)
	{
		return X * X + Y * Y;
	}
	readonly_property(float, InverseSquaredLength)
	{
		return __rsqrt_ss((float)(X * X + Y * Y));
	}
	readonly_property(float, InverseLength)
	{
		float inverseLength = __rsqrt_ss((float)(X * X + Y * Y));
		return inverseLength * inverseLength;
	}

	__forceinline vlong2() :
		X(0),
		Y(0)
	{
	}
	__forceinline vlong2(const vlong2 &value) :
		X(value.X),
		Y(value.Y)
	{
	}
	__forceinline explicit vlong2(const int64 *ptr) :
		X(ptr[0]),
		Y(ptr[1])
	{
	}
	__forceinline explicit vlong2(const vfloat2 &value) :
		X((int64)value.X),
		Y((int64)value.Y)
	{
	}
	__forceinline explicit vlong2(int64 uniform) :
		X(uniform),
		Y(uniform)
	{
	}
	__forceinline explicit vlong2(int64 x, int64 y) :
		X(x),
		Y(y)
	{
	}

	__forceinline int64 DotProduct(const vlong2 &other) const
	{
		return X * other.X + Y * other.Y;
	}

	__forceinline vlong2& operator =(const vlong2 &other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}
	__forceinline vlong2 operator +(const vlong2 &other) const
	{
		return vlong2(X + other.X, Y + other.Y);
	}
	__forceinline vlong2 operator +(int64 scalar) const
	{
		return vlong2(X + scalar, Y + scalar);
	}
	__forceinline vlong2 operator -(const vlong2 &other) const
	{
		return vlong2(X - other.X, Y - other.Y);
	}
	__forceinline vlong2 operator -(int64 scalar) const
	{
		return vlong2(X - scalar, Y - scalar);
	}
	__forceinline vlong2 operator -() const
	{
		return vlong2(-X, -Y);
	}
	__forceinline vlong2 operator *(const vlong2 &other) const
	{
		return vlong2(X * other.X, Y * other.Y);
	}
	__forceinline vlong2 operator *(int64 scalar) const
	{
		return vlong2(X * scalar, Y * scalar);
	}
	__forceinline vlong2 operator /(const vlong2 &other) const
	{
		return vlong2(X / other.X, Y / other.Y);
	}
	__forceinline vlong2 operator /(int64 scalar) const
	{
		return vlong2(X / scalar, Y / scalar);
	}
	__forceinline vlong2 operator %(const vlong2 &other) const
	{
		return vlong2(X % other.X, Y % other.Y);
	}
	__forceinline vlong2 operator %(int64 scalar) const
	{
		return vlong2(X % scalar, Y % scalar);
	}
	__forceinline vlong2 operator ^(const vlong2 &other) const
	{
		return vlong2(X ^ other.X, Y ^ other.Y);
	}
	__forceinline vlong2 operator ^(int64 scalar) const
	{
		return vlong2(X ^ scalar, Y ^ scalar);
	}
	__forceinline vlong2 operator |(const vlong2 &other) const
	{
		return vlong2(X | other.X, Y | other.Y);
	}
	__forceinline vlong2 operator |(int64 scalar) const
	{
		return vlong2(X | scalar, Y | scalar);
	}
	__forceinline vlong2 operator &(const vlong2 &other) const
	{
		return vlong2(X & other.X, Y & other.Y);
	}
	__forceinline vlong2 operator &(int64 scalar) const
	{
		return vlong2(X & scalar, Y & scalar);
	}
	__forceinline vlong2 operator ~() const
	{
		return vlong2(~X, ~Y);
	}
	__forceinline vlong2 operator <<(const vlong2 &other) const
	{
		return vlong2(X << other.X, Y << other.Y);
	}
	__forceinline vlong2 operator <<(int32 count) const
	{
		return vlong2(X << count, Y << count);
	}
	__forceinline vlong2 operator >>(const vlong2 &other) const
	{
		return vlong2(X >> other.X, Y >> other.Y);
	}
	__forceinline vlong2 operator >>(int32 count) const
	{
		return vlong2(X >> count, Y >> count);
	}
	__forceinline vlong2& operator +=(const vlong2 &other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	__forceinline vlong2& operator +=(int64 scalar)
	{
		X += scalar;
		Y += scalar;
		return *this;
	}
	__forceinline vlong2& operator -=(const vlong2 &other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	__forceinline vlong2& operator -=(int64 scalar)
	{
		X -= scalar;
		Y -= scalar;
		return *this;
	}
	__forceinline vlong2& operator *=(const vlong2 &other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	__forceinline vlong2& operator *=(int64 scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
	__forceinline vlong2& operator /=(const vlong2 &other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	__forceinline vlong2& operator /=(int64 scalar)
	{
		X /= scalar;
		Y /= scalar;
		return *this;
	}
	__forceinline vlong2& operator %=(const vlong2 &other)
	{
		X %= other.X;
		Y %= other.Y;
		return *this;
	}
	__forceinline vlong2& operator %=(int64 scalar)
	{
		X %= scalar;
		Y %= scalar;
		return *this;
	}
	__forceinline vlong2& operator ^=(const vlong2 &other)
	{
		X ^= other.X;
		Y ^= other.Y;
		return *this;
	}
	__forceinline vlong2& operator ^=(int64 scalar)
	{
		X ^= scalar;
		Y ^= scalar;
		return *this;
	}
	__forceinline vlong2& operator |=(const vlong2 &other)
	{
		X |= other.X;
		Y |= other.Y;
		return *this;
	}
	__forceinline vlong2& operator |=(int64 scalar)
	{
		X |= scalar;
		Y |= scalar;
		return *this;
	}
	__forceinline vlong2& operator &=(const vlong2 &other)
	{
		X &= other.X;
		Y &= other.Y;
		return *this;
	}
	__forceinline vlong2& operator &=(int64 scalar)
	{
		X &= scalar;
		Y &= scalar;
		return *this;
	}
	__forceinline vlong2& operator <<=(const vlong2 &other)
	{
		X <<= other.X;
		Y <<= other.Y;
		return *this;
	}
	__forceinline vlong2& operator <<=(int32 count)
	{
		X <<= count;
		Y <<= count;
		return *this;
	}
	__forceinline vlong2& operator >>=(const vlong2 &other)
	{
		X >>= other.X;
		Y >>= other.Y;
		return *this;
	}
	__forceinline vlong2& operator >>=(int32 count)
	{
		X >>= count;
		Y >>= count;
		return *this;
	}
	__forceinline bool operator ==(const vlong2 &other) const
	{
		return X == other.X && Y == other.Y;
	}
	__forceinline bool operator ==(int64 scalar) const
	{
		return X == scalar && Y == scalar;
	}
	__forceinline bool operator !=(const vlong2 &other) const
	{
		return X != other.X || Y != other.Y;
	}
	__forceinline bool operator !=(int64 scalar) const
	{
		return X != scalar || Y != scalar;
	}
	__forceinline bool operator <(const vlong2 &other) const
	{
		return X < other.X && Y < other.Y;
	}
	__forceinline bool operator <(int64 scalar) const
	{
		return X < scalar && Y < scalar;
	}
	__forceinline bool operator >(const vlong2 &other) const
	{
		return X > other.X && Y > other.Y;
	}
	__forceinline bool operator >(int64 scalar) const
	{
		return X > scalar && Y > scalar;
	}
	__forceinline bool operator <=(const vlong2 &other) const
	{
		return X <= other.X && Y <= other.Y;
	}
	__forceinline bool operator <=(int64 scalar) const
	{
		return X <= scalar && Y <= scalar;
	}
	__forceinline bool operator >=(const vlong2 &other) const
	{
		return X >= other.X && Y >= other.Y;
	}
	__forceinline bool operator >=(int64 scalar) const
	{
		return X >= scalar && Y >= scalar;
	}
	__forceinline operator vfloat2() const
	{
		return vfloat2((float)X, (float)Y);
	}
};

using vfixlong2 = vlong2;