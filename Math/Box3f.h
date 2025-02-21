#pragma once
#include "FastPix3D.h"
#include "Vector3f.h"

struct Box3f
{
	Vector3f Min;
	Vector3f Max;

	property_get(Vector3f, Size)
	{
		return Max - Min;
	}

	Box3f()
	{
	}
	explicit Box3f(const Vector3f &min, const Vector3f &max) :
		Min(min),
		Max(max)
	{
	}

	bool Contains(const Vector3f &point) const
	{
		return point.X >= Min.X && point.X <= Max.X && point.Y >= Min.Y && point.Y <= Max.Y && point.Z >= Min.Z && point.Z <= Max.Z;
	}
};