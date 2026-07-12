#pragma once
#include "FastPix3D.h"
#include "Math/Color.h"
#include "Math/Matrix4f.h"
#include "Mesh/Mesh.h"
#include "Mesh/Vertex.h"
#include "RenderStates.h"
#include "RenderStatistics.h"

class FASTPIX3D_API RenderUnit
{
public:
	RenderStatistics Statistics;

	void ClearFrameBuffer(const RenderStates &renderStates) const;
	void ClearFrameBuffer(const RenderStates &renderStates, int32 r, int32 g, int32 b) const;
	void ClearFrameBuffer(const RenderStates &renderStates, const Color &color) const;
	void ClearDepthBuffer(const RenderStates &renderStates) const;
	void ClearShadowMap(const RenderStates &renderStates) const;

	void DrawMesh(const RenderStates &renderStates, const Mesh &mesh, const Matrix4f &modelMatrix);
	void DrawTriangle(const RenderStates &renderStates, const Vertex &v1, const Vertex &v2, const Vertex &v3);
	void RenderFog(const RenderStates &renderStates) const;
};