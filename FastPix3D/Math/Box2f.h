#pragma once
#include "../FastPix3D.h"
#include "vfloat2.h"

struct FASTPIX3D_API Box2f
{
	vfloat2 Min;
	vfloat2 Max;

	readonly_property(vfloat2, Size)
	{
		return Max - Min;
	}

	__forceinline Box2f()
	{
	}
	__forceinline explicit Box2f(const vfloat2 &min, const vfloat2 &max) :
		Min(min),
		Max(max)
	{
	}
	__forceinline explicit Box2f(const vfloat2 &size)
	{
		vfloat2 halfSize = size * .5f;
		Min = -halfSize;
		Max = halfSize;
	}
	__forceinline explicit Box2f(float size)
	{
		vfloat2 halfSize = vfloat2(size * .5f);
		Min = -halfSize;
		Max = halfSize;
	}

	__forceinline bool Contains(const vfloat2 &point) const
	{
		return point.X >= Min.X && point.X <= Max.X && point.Y >= Min.Y && point.Y <= Max.Y;
	}
};