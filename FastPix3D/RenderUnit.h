#pragma once
#include "FastPix3D.h"
#include "Color.h"
#include "Matrix4f.h"
#include "Mesh.h"
#include "RenderStates.h"
#include "RenderStatistics.h"

class RenderUnit
{
public:
	RenderStates RenderStates;
	RenderStatistics Statistics;

	void ClearFrameBuffer() const;
	void ClearFrameBuffer(int32 r, int32 g, int32 b) const;
	void ClearFrameBuffer(const Color &color) const;
	void ClearDepthBuffer() const;
	void ClearStencilBuffer() const;
	void ClearShadowMap() const;

	void DrawMesh(const Mesh &mesh, const Matrix4f &modelSpace);
	void RenderDeferredPass();

private:
	void DrawTriangle(const Matrix4f &worldSpace, const Matrix4f &shadowLightSpace, const Vertex &v1, const Vertex &v2, const Vertex &v3);
};