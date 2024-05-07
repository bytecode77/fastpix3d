#pragma once
#include "FastPix3D.h"

class RenderStatistics
{
public:
	int32 TotalTriangleCount;
	int32 RenderedTriangleCount;

	RenderStatistics()
	{
		Clear();
	}
	void Clear()
	{
		TotalTriangleCount = 0;
		RenderedTriangleCount = 0;
	}

	void Merge(const RenderStatistics &statistics)
	{
		TotalTriangleCount += statistics.TotalTriangleCount;
		RenderedTriangleCount += statistics.RenderedTriangleCount;
	}
};