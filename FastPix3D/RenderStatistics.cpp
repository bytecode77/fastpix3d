#include "RenderStatistics.h"

RenderStatistics::RenderStatistics()
{
	Clear();
}
RenderStatistics::RenderStatistics(const RenderStatistics &other)
{
	TotalTriangleCount.store(other.TotalTriangleCount.load(std::memory_order_relaxed), std::memory_order_relaxed);
	RenderedTriangleCount.store(other.RenderedTriangleCount.load(std::memory_order_relaxed), std::memory_order_relaxed);
}

void RenderStatistics::Clear()
{
	TotalTriangleCount.store(0, std::memory_order_relaxed);
	RenderedTriangleCount.store(0, std::memory_order_relaxed);
}

RenderStatistics& RenderStatistics::operator =(const RenderStatistics &other)
{
	TotalTriangleCount.store(other.TotalTriangleCount.load(std::memory_order_relaxed), std::memory_order_relaxed);
	RenderedTriangleCount.store(other.RenderedTriangleCount.load(std::memory_order_relaxed), std::memory_order_relaxed);

	return *this;
}