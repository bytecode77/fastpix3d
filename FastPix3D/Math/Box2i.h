#pragma once
#include "../FastPix3D.h"
#include "vint2.h"

struct FASTPIX3D_API Box2i
{
	vint2 Min;
	vint2 Max;

	readonly_property(vint2, Size)
	{
		return Max - Min;
	}

	__forceinline Box2i()
	{
	}
	__forceinline explicit Box2i(const vint2 &min, const vint2 &max) :
		Min(min),
		Max(max)
	{
	}
	__forceinline explicit Box2i(const vint2 &size)
	{
		vint2 halfSize = size / 2;
		Min = -halfSize;
		Max = halfSize;
	}
	__forceinline explicit Box2i(float size)
	{
		vint2 halfSize = vint2((int32)(size * .5f));
		Min = -halfSize;
		Max = halfSize;
	}

	__forceinline bool Contains(const vint2 &point) const
	{
		return point.X >= Min.X && point.X <= Max.X && point.Y >= Min.Y && point.Y <= Max.Y;
	}
};

using Box2x = Box2i;