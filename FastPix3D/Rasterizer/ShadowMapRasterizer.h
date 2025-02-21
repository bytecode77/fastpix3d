#pragma once
#include "FastPix3D.h"
#include "RenderStates.h"
#include "RenderStatistics.h"
#include "Vertex.h"
#include "Vector3f.h"

struct ShadowMapVertex
{
	Vector3f Position;
	Vector2f TextureCoordinates;

	ShadowMapVertex()
	{
	}
	explicit ShadowMapVertex(const Vertex &vertex) :
		Position(vertex.Position),
		TextureCoordinates(vertex.TextureCoordinates)
	{
	}
};

struct ShadowMapScanlineEdge
{
	int32 X;
	float Z;
	Vector2f Texture;
};

struct ShadowMapScanlineParameters
{
	int32 Y;
	ShadowMapScanlineEdge V1;
	ShadowMapScanlineEdge V2;
	float DeltaZ;
	float DeltaZSubdiv;
	Vector2f DeltaTextureSubdiv;
};

class ShadowMapRasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit ShadowMapRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics);

	void DrawTriangle(const Matrix4f &shadowLightSpace, const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawClippedTriangle(const ShadowMapVertex &v1, const ShadowMapVertex &v2, const ShadowMapVertex &v3) const;
	template<bool hasTexture>
	bool DrawClippedTriangle(ShadowMapVertex v1, ShadowMapVertex v2, ShadowMapVertex v3) const;
	template<bool hasTexture>
	void DrawScanline(ShadowMapScanlineParameters p) const;
};