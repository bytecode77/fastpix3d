#pragma once
#include "../FastPix3D.h"
#include "vfloat2.h"

struct FASTPIX3D_API vint2
{
	int32 X;
	int32 Y;

	readonly_property(float, Length)
	{
		return sqrtf((float)(X * X + Y * Y));
	}
	readonly_property(int32, SquaredLength)
	{
		return X * X + Y * Y;
	}
	readonly_property(float, InverseLength)
	{
		return __rsqrt_ss((float)(X * X + Y * Y));
	}
	readonly_property(float, InverseSquaredLength)
	{
		return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss((float)(X * X + Y * Y))));
	}

	__forceinline vint2() :
		X(0),
		Y(0)
	{
	}
	__forceinline vint2(const vint2 &other) :
		X(other.X),
		Y(other.Y)
	{
	}
	__forceinline explicit vint2(const int32 *ptr) :
		X(ptr[0]),
		Y(ptr[1])
	{
	}
	__forceinline explicit vint2(const vfloat2 &other) :
		X((int32)other.X),
		Y((int32)other.Y)
	{
	}
	__forceinline explicit vint2(int32 uniform) :
		X(uniform),
		Y(uniform)
	{
	}
	__forceinline explicit vint2(int32 x, int32 y) :
		X(x),
		Y(y)
	{
	}

	__forceinline int32 DotProduct(const vint2 &other) const
	{
		return X * other.X + Y * other.Y;
	}

	__forceinline vint2& operator =(const vint2 &other)
	{
		X = other.X;
		Y = other.Y;
		return *this;
	}
	__forceinline vint2 operator +(const vint2 &other) const
	{
		return vint2(X + other.X, Y + other.Y);
	}
	__forceinline vint2 operator +(int32 scalar) const
	{
		return vint2(X + scalar, Y + scalar);
	}
	__forceinline vint2 operator -(const vint2 &other) const
	{
		return vint2(X - other.X, Y - other.Y);
	}
	__forceinline vint2 operator -(int32 scalar) const
	{
		return vint2(X - scalar, Y - scalar);
	}
	__forceinline vint2 operator -() const
	{
		return vint2(-X, -Y);
	}
	__forceinline vint2 operator *(const vint2 &other) const
	{
		return vint2(X * other.X, Y * other.Y);
	}
	__forceinline vint2 operator *(int32 scalar) const
	{
		return vint2(X * scalar, Y * scalar);
	}
	__forceinline vint2 operator /(const vint2 &other) const
	{
		return vint2(X / other.X, Y / other.Y);
	}
	__forceinline vint2 operator /(int32 scalar) const
	{
		return vint2(X / scalar, Y / scalar);
	}
	__forceinline vint2 operator %(const vint2 &other) const
	{
		return vint2(X % other.X, Y % other.Y);
	}
	__forceinline vint2 operator %(int32 scalar) const
	{
		return vint2(X % scalar, Y % scalar);
	}
	__forceinline vint2 operator ^(const vint2 &other) const
	{
		return vint2(X ^ other.X, Y ^ other.Y);
	}
	__forceinline vint2 operator ^(int32 scalar) const
	{
		return vint2(X ^ scalar, Y ^ scalar);
	}
	__forceinline vint2 operator |(const vint2 &other) const
	{
		return vint2(X | other.X, Y | other.Y);
	}
	__forceinline vint2 operator |(int32 scalar) const
	{
		return vint2(X | scalar, Y | scalar);
	}
	__forceinline vint2 operator &(const vint2 &other) const
	{
		return vint2(X & other.X, Y & other.Y);
	}
	__forceinline vint2 operator &(int32 scalar) const
	{
		return vint2(X & scalar, Y & scalar);
	}
	__forceinline vint2 operator ~() const
	{
		return vint2(~X, ~Y);
	}
	__forceinline vint2 operator <<(const vint2 &other) const
	{
		return vint2(X << other.X, Y << other.Y);
	}
	__forceinline vint2 operator <<(int32 count) const
	{
		return vint2(X << count, Y << count);
	}
	__forceinline vint2 operator >>(const vint2 &other) const
	{
		return vint2(X >> other.X, Y >> other.Y);
	}
	__forceinline vint2 operator >>(int32 count) const
	{
		return vint2(X >> count, Y >> count);
	}
	__forceinline vint2& operator +=(const vint2 &other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	__forceinline vint2& operator +=(int32 scalar)
	{
		X += scalar;
		Y += scalar;
		return *this;
	}
	__forceinline vint2& operator -=(const vint2 &other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	__forceinline vint2& operator -=(int32 scalar)
	{
		X -= scalar;
		Y -= scalar;
		return *this;
	}
	__forceinline vint2& operator *=(const vint2 &other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	__forceinline vint2& operator *=(int32 scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
	__forceinline vint2& operator /=(const vint2 &other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	__forceinline vint2& operator /=(int32 scalar)
	{
		X /= scalar;
		Y /= scalar;
		return *this;
	}
	__forceinline vint2& operator %=(const vint2 &other)
	{
		X %= other.X;
		Y %= other.Y;
		return *this;
	}
	__forceinline vint2& operator %=(int32 scalar)
	{
		X %= scalar;
		Y %= scalar;
		return *this;
	}
	__forceinline vint2& operator ^=(const vint2 &other)
	{
		X ^= other.X;
		Y ^= other.Y;
		return *this;
	}
	__forceinline vint2& operator ^=(int32 scalar)
	{
		X ^= scalar;
		Y ^= scalar;
		return *this;
	}
	__forceinline vint2& operator |=(const vint2 &other)
	{
		X |= other.X;
		Y |= other.Y;
		return *this;
	}
	__forceinline vint2& operator |=(int32 scalar)
	{
		X |= scalar;
		Y |= scalar;
		return *this;
	}
	__forceinline vint2& operator &=(const vint2 &other)
	{
		X &= other.X;
		Y &= other.Y;
		return *this;
	}
	__forceinline vint2& operator &=(int32 scalar)
	{
		X &= scalar;
		Y &= scalar;
		return *this;
	}
	__forceinline vint2& operator <<=(const vint2 &other)
	{
		X <<= other.X;
		Y <<= other.Y;
		return *this;
	}
	__forceinline vint2& operator <<=(int32 count)
	{
		X <<= count;
		Y <<= count;
		return *this;
	}
	__forceinline vint2& operator >>=(const vint2 &other)
	{
		X >>= other.X;
		Y >>= other.Y;
		return *this;
	}
	__forceinline vint2& operator >>=(int32 count)
	{
		X >>= count;
		Y >>= count;
		return *this;
	}
	__forceinline bool operator ==(const vint2 &other) const
	{
		return X == other.X && Y == other.Y;
	}
	__forceinline bool operator ==(int32 scalar) const
	{
		return X == scalar && Y == scalar;
	}
	__forceinline bool operator !=(const vint2 &other) const
	{
		return X != other.X || Y != other.Y;
	}
	__forceinline bool operator !=(int32 scalar) const
	{
		return X != scalar || Y != scalar;
	}
	__forceinline bool operator <(const vint2 &other) const
	{
		return X < other.X && Y < other.Y;
	}
	__forceinline bool operator <(int32 scalar) const
	{
		return X < scalar && Y < scalar;
	}
	__forceinline bool operator >(const vint2 &other) const
	{
		return X > other.X && Y > other.Y;
	}
	__forceinline bool operator >(int32 scalar) const
	{
		return X > scalar && Y > scalar;
	}
	__forceinline bool operator <=(const vint2 &other) const
	{
		return X <= other.X && Y <= other.Y;
	}
	__forceinline bool operator <=(int32 scalar) const
	{
		return X <= scalar && Y <= scalar;
	}
	__forceinline bool operator >=(const vint2 &other) const
	{
		return X >= other.X && Y >= other.Y;
	}
	__forceinline bool operator >=(int32 scalar) const
	{
		return X >= scalar && Y >= scalar;
	}
	__forceinline operator vfloat2() const
	{
		return vfloat2((float)X, (float)Y);
	}
};

using vfix2 = vint2;