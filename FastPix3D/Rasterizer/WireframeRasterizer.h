#pragma once
#include "../FastPix3D.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"

class WireframeRasterizer
{
private:
	const RenderStates &RenderStates;

public:
	explicit WireframeRasterizer(const ::RenderStates &renderStates) :
		RenderStates(renderStates)
	{
	}

	bool DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawEdge(const vfloat3 &v1, const vfloat3 &v2) const;
	template<bool zEnable, bool zWriteEnable>
	bool DrawEdge(vfloat3 v1, vfloat3 v2) const;
};