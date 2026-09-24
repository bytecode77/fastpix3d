#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../WorkPartition.h"

class ShadowMapRasterizer
{
private:
	struct RasterizerVertex
	{
		vfloat3 Position;
		vfloat2 TextureCoordinates;

		RasterizerVertex()
		{
		}
		explicit RasterizerVertex(const Vertex &vertex) :
			Position(vertex.Position),
			TextureCoordinates(vertex.TextureCoordinates)
		{
		}
	};

	const RenderStates &RenderStates;
	const WorkPartition WorkPartition;

public:
	explicit ShadowMapRasterizer(const ::RenderStates &renderStates, ::WorkPartition workPartition) :
		RenderStates(renderStates),
		WorkPartition(workPartition)
	{
	}

	bool DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	void ClipEdges(const RasterizerVertex &edge1a, const RasterizerVertex &edge1b, const RasterizerVertex &edge2a, const RasterizerVertex &edge2b, RasterizerVertex &intersection1, RasterizerVertex &intersection2) const;
	bool DrawClippedTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3) const;
	template<bool hasTexture>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<bool hasTexture>
	__forceinline void DrawPixelVector(
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