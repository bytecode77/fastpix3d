#include "RenderStatistics.h"

RenderStatistics::RenderStatistics()
{
	Clear();
}
RenderStatistics::RenderStatistics(const RenderStatistics &renderStatistics)
{
	TotalTriangleCount.store(renderStatistics.TotalTriangleCount.load());
	RenderedTriangleCount.store(renderStatistics.RenderedTriangleCount.load());
}

void RenderStatistics::Clear()
{
	TotalTriangleCount = 0;
	RenderedTriangleCount = 0;
}

RenderStatistics& RenderStatistics::operator =(const RenderStatistics &other)
{
	TotalTriangleCount.store(other.TotalTriangleCount.load());
	RenderedTriangleCount.store(other.RenderedTriangleCount.load());
	return *this;
}