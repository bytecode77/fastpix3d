#pragma once
#include "FastPix3D.h"
#include "RenderStates.h"
#include "RenderStatistics.h"
#include "Vertex.h"
#include "Vector3f.h"

struct RasterizerVertex
{
	Vector3f Position;
	Vector3f Normals;
	Vector2f TextureCoordinates;
	Vector3f Color;

	RasterizerVertex()
	{
	}
	explicit RasterizerVertex(const Vertex &vertex) :
		Position(vertex.Position),
		Normals(vertex.Normals),
		TextureCoordinates(vertex.TextureCoordinates),
		Color((Vector3f)vertex.Color)
	{
	}
};

struct ScanlineEdge
{
	int32 X;
	float Z;
	Vector2f Texture;
	Vector3f Color;
};

struct ScanlineParameters
{
	int32 Y;
	ScanlineEdge V1;
	ScanlineEdge V2;
	float DeltaZ;
	float DeltaZSubdiv;
	Vector2f DeltaTextureSubdiv;
	Vector3f DeltaColorSubdiv;
};

class Rasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit Rasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics);

	void DrawTriangle(const Matrix4f &worldSpace, const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawClippedTriangle(const RasterizerVertex &v1, const RasterizerVertex &v2, const RasterizerVertex &v3, bool hasColors) const;
	template<bool hasTexture, bool hasColors>
	bool DrawClippedTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3) const;
	template<bool hasTexture, bool hasColors>
	void DrawScanline(const ScanlineParameters &p) const;
	template<bool zEnable, bool zWriteEnable, StencilFunc stencilFunc, bool hasTexture, bool hasColors, BlendMode blendMode>
	void DrawScanline(ScanlineParameters p) const;
};