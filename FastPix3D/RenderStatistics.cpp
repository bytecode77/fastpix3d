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
void RenderStatistics::Merge(const RenderStatistics &statistics)
{
	Merge(statistics, true, true);
}
void RenderStatistics::Merge(const RenderStatistics &statistics, bool totalTriangleCount, bool renderedTriangleCount)
{
	if (totalTriangleCount)
	{
		TotalTriangleCount += statistics.TotalTriangleCount;
	}

	if (renderedTriangleCount)
	{
		RenderedTriangleCount += statistics.RenderedTriangleCount;
	}
}

RenderStatistics& RenderStatistics::operator =(const RenderStatistics &other)
{
	TotalTriangleCount.store(other.TotalTriangleCount.load());
	RenderedTriangleCount.store(other.RenderedTriangleCount.load());
	return *this;
}