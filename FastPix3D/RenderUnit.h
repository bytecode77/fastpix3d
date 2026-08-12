#pragma once
#include "FastPix3D.h"
#include "Math/Color.h"
#include "Math/Matrix4.h"
#include "Mesh/Mesh.h"
#include "Mesh/Vertex.h"
#include "RenderStates.h"
#include "RenderStatistics.h"
#include "WorkPartition.h"

class FASTPIX3D_API RenderUnit
{
public:
	RenderStatistics Statistics;

	void ClearFrameBuffer(const RenderStates &renderStates) const;
	void ClearFrameBuffer(const RenderStates &renderStates, byte r, byte g, byte b) const;
	void ClearFrameBuffer(const RenderStates &renderStates, const Color &color) const;
	void ClearDepthBuffer(const RenderStates &renderStates) const;
	void ClearShadowMap(const RenderStates &renderStates) const;

	void DrawMesh(const RenderStates &renderStates, WorkPartition workPartition, const Mesh &mesh, const Matrix4 &modelMatrix);
	void DrawTriangle(const RenderStates &renderStates, WorkPartition workPartition, const Vertex &v1, const Vertex &v2, const Vertex &v3);
	void RenderFog(const RenderStates &renderStates, WorkPartition workPartition) const;

private:
	void DrawTriangle(const RenderStates &renderStates, WorkPartition workPartition, const Vertex &v1, const Vertex &v2, const Vertex &v3, int32 &totalTriangleCount, int32 &renderedTriangleCount);
};