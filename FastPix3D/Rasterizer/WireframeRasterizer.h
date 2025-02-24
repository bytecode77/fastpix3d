#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../RenderStatistics.h"

struct WireframeRasterizerVertex
{
	vfloat3 Position;

	WireframeRasterizerVertex()
	{
	}
	explicit WireframeRasterizerVertex(const Vertex &vertex) :
		Position(vertex.Position)
	{
	}
};

class WireframeRasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit WireframeRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
		RenderStates(renderStates),
		Statistics(statistics)
	{
	}

	void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawEdge(const WireframeRasterizerVertex &v1, const WireframeRasterizerVertex &v2) const;
	template<bool zEnable, bool zWriteEnable>
	bool DrawEdge(WireframeRasterizerVertex v1, WireframeRasterizerVertex v2) const;
};