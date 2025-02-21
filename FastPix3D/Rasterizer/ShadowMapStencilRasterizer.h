#pragma once
#include "FastPix3D.h"
#include "RenderStates.h"
#include "RenderStatistics.h"
#include "Vertex.h"
#include "Vector3f.h"

struct ShadowMapStencilVertex
{
	Vector3f Position;
	Vector2f TextureCoordinates;
	Vector3f Uvw;

	ShadowMapStencilVertex()
	{
	}
	explicit ShadowMapStencilVertex(const Vertex &vertex) :
		Position(vertex.Position),
		TextureCoordinates(vertex.TextureCoordinates),
		Uvw(vertex.Position)
	{
	}
};

struct ShadowMapStencilScanlineEdge
{
	int32 X;
	float Z;
	Vector2f Texture;
	Vector3f Uvw;
};

struct ShadowMapStencilScanlineParameters
{
	int32 Y;
	ShadowMapStencilScanlineEdge V1;
	ShadowMapStencilScanlineEdge V2;
	float DeltaZ;
	float DeltaZSubdiv;
	Vector2f DeltaTextureSubdiv;
	Vector3f DeltaUvwSubdiv;
};

class ShadowMapStencilRasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit ShadowMapStencilRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics);

	void DrawTriangle(const Matrix4f &worldSpace, const Matrix4f &shadowLightSpace, const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawClippedTriangle(const ShadowMapStencilVertex &v1, const ShadowMapStencilVertex &v2, const ShadowMapStencilVertex &v3) const;
	template<bool hasTexture, bool pcf>
	bool DrawClippedTriangle(ShadowMapStencilVertex v1, ShadowMapStencilVertex v2, ShadowMapStencilVertex v3) const;
	template<bool hasTexture, bool pcf>
	void DrawScanline(ShadowMapStencilScanlineParameters p) const;
};