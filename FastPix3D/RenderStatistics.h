#pragma once
#include "FastPix3D.h"

class FASTPIX3D_API RenderStatistics
{
public:
	std::atomic<int32> TotalTriangleCount;
	std::atomic<int32> RenderedTriangleCount;

	RenderStatistics();
	RenderStatistics(const RenderStatistics &renderStatistics);

	void Clear();
	void Merge(const RenderStatistics &statistics);
	void Merge(const RenderStatistics &statistics, bool totalTriangleCount, bool renderedTriangleCount);

	RenderStatistics& operator =(const RenderStatistics &other);
};