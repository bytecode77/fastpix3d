#pragma once
#include "../FastPix3D.h"
#include "vfloat3.h"

struct FASTPIX3D_API Box3f
{
	vfloat3 Min;
	vfloat3 Max;

	readonly_property(vfloat3, Size)
	{
		return Max - Min;
	}

	__forceinline Box3f()
	{
	}
	__forceinline explicit Box3f(const vfloat3 &min, const vfloat3 &max) :
		Min(min),
		Max(max)
	{
	}
	__forceinline explicit Box3f(const vfloat3 &size)
	{
		vfloat3 halfSize = size * .5f;
		Min = -halfSize;
		Max = halfSize;
	}
	__forceinline explicit Box3f(float size)
	{
		vfloat3 halfSize = vfloat3(size * .5f);
		Min = -halfSize;
		Max = halfSize;
	}

	__forceinline bool Contains(const vfloat3 &point) const
	{
		return point.X >= Min.X && point.X <= Max.X && point.Y >= Min.Y && point.Y <= Max.Y && point.Z >= Min.Z && point.Z <= Max.Z;
	}
};