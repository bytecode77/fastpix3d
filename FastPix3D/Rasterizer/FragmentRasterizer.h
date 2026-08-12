#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../WorkPartition.h"

class FragmentRasterizer
{
private:
	struct RasterizerVertex
	{
		vfloat3 Position;
		vfloat3 Normals;
		vfloat2 TextureCoordinates;
		vfloat3 Color;
		vfloat3 Specular;
		vfloat3 ShadowXyz;
		vfloat3 ShadowColor;

		RasterizerVertex()
		{
		}
		explicit RasterizerVertex(const Vertex &vertex) :
			Position(vertex.Position),
			Normals(vertex.Normals),
			TextureCoordinates(vertex.TextureCoordinates),
			Color((vfloat3)vertex.Color),
			ShadowXyz(vertex.Position),
			ShadowColor((vfloat3)vertex.Color)
		{
		}
	};

	const RenderStates &RenderStates;
	const WorkPartition WorkPartition;

public:
	explicit FragmentRasterizer(const ::RenderStates &renderStates, ::WorkPartition workPartition) :
		RenderStates(renderStates),
		WorkPartition(workPartition)
	{
	}

	bool DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	void ClipEdges(bool hasColor, bool hasSpecular, const RasterizerVertex &edge1a, const RasterizerVertex &edge1b, const RasterizerVertex &edge2a, const RasterizerVertex &edge2b, RasterizerVertex &intersection1, RasterizerVertex &intersection2) const;
	bool DrawClippedTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3, bool hasColor, bool hasSpecular) const;
	template<bool hasColor, bool hasSpecular>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
	void DrawClippedTriangle(RasterizerVertex &v1, RasterizerVertex &v2, RasterizerVertex &v3) const;
	template<DepthMode depthMode, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
	__forceinline void DrawPixelVector(
		vuint8 *frameBuffer,
		vfloat8 *depthBuffer,
		vuint8 writeMask,
		const vfloat8 &attributeZ,
		const vfloat8 &attributeU,
		const vfloat8 &attributeV,
		const vfloat8 &attributeR,
		const vfloat8 &attributeG,
		const vfloat8 &attributeB,
		const vfloat8 &attributeSpecularR,
		const vfloat8 &attributeSpecularG,
		const vfloat8 &attributeSpecularB,
		const vfloat8 &attributeShadowX,
		const vfloat8 &attributeShadowY,
		const vfloat8 &attributeShadowZ,
		const vfloat8 &attributeShadowR,
		const vfloat8 &attributeShadowG,
		const vfloat8 &attributeShadowB,
		const Color *textureBuffer,
		int32 textureWidthMask,
		int32 textureHeightMask,
		int32 textureWidthExponent,
		const float *shadowMap,
		int32 shadowMapSize,
		int32 shadowMapSizeMask,
		int32 shadowMapSizeExponent,
		int32 alpha) const;
};