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
	RenderStates RenderStates;
	RenderStatistics Statistics;

	void ClearFrameBuffer() const;
	void ClearFrameBuffer(int32 r, int32 g, int32 b) const;
	void ClearFrameBuffer(const Color &color) const;
	void ClearDepthBuffer() const;
	void ClearShadowMap() const;

	void DrawMesh(const Mesh &mesh, const Matrix4f &modelMatrix);
	void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
	void RenderFog();
};