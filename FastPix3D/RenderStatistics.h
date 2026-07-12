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

	RenderStatistics& operator =(const RenderStatistics &other);
};