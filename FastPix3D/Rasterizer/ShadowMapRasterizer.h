#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../RenderStatistics.h"

struct ShadowMapRasterizerVertex
{
	vfloat3 Position;
	vfloat2 TextureCoordinates;

	ShadowMapRasterizerVertex()
	{
	}
	explicit ShadowMapRasterizerVertex(const Vertex &vertex) :
		Position(vertex.Position),
		TextureCoordinates(vertex.TextureCoordinates)
	{
	}
};

class ShadowMapRasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit ShadowMapRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
		RenderStates(renderStates),
		Statistics(statistics)
	{
	}

	void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawClippedTriangle(const ShadowMapRasterizerVertex &v1, const ShadowMapRasterizerVertex &v2, const ShadowMapRasterizerVertex &v3) const;
	template<bool hasTexture>
	bool DrawClippedTriangle(ShadowMapRasterizerVertex v1, ShadowMapRasterizerVertex v2, ShadowMapRasterizerVertex v3) const;
	template<bool hasTexture>
	__forceinline void DrawPixelRow(
		vfloat8 *shadowMap,
		vuint8 writeMask,
		const vfloat8 &attributeZ,
		const vfloat8 &attributeU,
		const vfloat8 &attributeV,
		const Color *textureBuffer,
		int32 textureWidthMask,
		int32 textureHeightMask,
		int32 textureWidthExponent) const;
};