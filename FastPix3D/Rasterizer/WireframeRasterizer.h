#pragma once
#include "../FastPix3D.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../WorkPartition.h"

class WireframeRasterizer
{
private:
	const RenderStates &RenderStates;
	const WorkPartition WorkPartition;

public:
	explicit WireframeRasterizer(const ::RenderStates &renderStates, ::WorkPartition workPartition) :
		RenderStates(renderStates),
		WorkPartition(workPartition)
	{
	}

	bool DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	void ClipEdges(const vfloat3 &edge1a, const vfloat3 &edge1b, const vfloat3 &edge2a, const vfloat3 &edge2b, vfloat3 &intersection1, vfloat3 &intersection2) const;
	bool DrawClippedEdge(const vfloat3 &v1, const vfloat3 &v2) const;
	template<DepthMode depthMode>
	__forceinline bool DrawClippedEdge(vfloat3 v1, vfloat3 v2) const;
	template<DepthMode depthMode, bool clipLower>
	__forceinline static void ClipScreenEdge(int32 &coordinate, int32 &coordinateOther, int32 otherCoordinate, int32 otherCoordinateOther, int32 clipValue, float &z, float otherZ);
};