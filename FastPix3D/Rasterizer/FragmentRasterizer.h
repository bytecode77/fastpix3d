#pragma once
#include "../FastPix3D.h"
#include "../Math/VectorMath.h"
#include "../Mesh/Vertex.h"
#include "../RenderStates.h"
#include "../RenderStatistics.h"

struct FragmentRasterizerVertex
{
	vfloat3 Position;
	vfloat3 Normals;
	vfloat2 TextureCoordinates;
	vfloat3 Color;
	vfloat3 Specular;
	vfloat3 ShadowXyz;
	vfloat3 ShadowColor;

	FragmentRasterizerVertex()
	{
	}
	explicit FragmentRasterizerVertex(const Vertex &vertex) :
		Position(vertex.Position),
		Normals(vertex.Normals),
		TextureCoordinates(vertex.TextureCoordinates),
		Color((vfloat3)vertex.Color),
		ShadowXyz(vertex.Position),
		ShadowColor((vfloat3)vertex.Color)
	{
	}
};

class FragmentRasterizer
{
private:
	const RenderStates &RenderStates;
	RenderStatistics &Statistics;

public:
	explicit FragmentRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
		RenderStates(renderStates),
		Statistics(statistics)
	{
	}

	void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) const;

private:
	bool DrawClippedTriangle(const FragmentRasterizerVertex &v1, const FragmentRasterizerVertex &v2, const FragmentRasterizerVertex &v3, bool hasColor, bool hasSpecular) const;
	template<bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
	bool DrawClippedTriangle(const FragmentRasterizerVertex &v1, const FragmentRasterizerVertex &v2, const FragmentRasterizerVertex &v3) const;
	template<bool zEnable, bool zWriteEnable, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
	bool DrawClippedTriangle(FragmentRasterizerVertex v1, FragmentRasterizerVertex v2, FragmentRasterizerVertex v3) const;
	template<bool zEnable, bool zWriteEnable, bool hasTexture, bool textureFilteringEnable, bool hasColor, bool hasSpecular, BlendMode blendMode, ShadowMapFunc shadowMapFunc, ShadowMapProjection shadowMapProjection>
	__forceinline void DrawPixelRow(
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