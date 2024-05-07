#include "ShadowMapRasterizer.h"
#include "Vector2i.h"
#include "RasterizerMath.h"

static const int32 SubdivExponent = 4;
static const int32 Subdiv1 = 1 << SubdivExponent;
static const int32  SubdivMask = Subdiv1 - 1;
static constexpr float InvertedSubdiv1 = 1.0f / Subdiv1;

ShadowMapRasterizer::ShadowMapRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
	RenderStates(renderStates),
	Statistics(statistics)
{
}
void ShadowMapRasterizer::DrawTriangle(const Matrix4f &shadowLightSpace, const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	ShadowMapVertex v1 = ShadowMapVertex(_v1);
	ShadowMapVertex v2 = ShadowMapVertex(_v2);
	ShadowMapVertex v3 = ShadowMapVertex(_v3);

	// Transform vertices to shadow map space.
	v1.Position = shadowLightSpace * v1.Position;
	v2.Position = shadowLightSpace * v2.Position;
	v3.Position = shadowLightSpace * v3.Position;

	// Back-face culling.
	if (RenderStates.CullMode != CullMode::None)
	{
		if (((RenderStates.CullMode == CullMode::Back) ^ RasterizerMath::IsTriangleFrontFace(v1.Position, v2.Position, v3.Position)) != 0)
		{
			return;
		}
	}

	// Clip along near clipping plane.
	bool vertex1Visible = v1.Position.Z > RenderStates.ClipNear;
	bool vertex2Visible = v2.Position.Z > RenderStates.ClipNear;
	bool vertex3Visible = v3.Position.Z > RenderStates.ClipNear;

	if (!vertex1Visible && !vertex2Visible && !vertex3Visible)
	{
		// All vertices are behind the near clipping plane -> early out.
		return;
	}

	// Normalize near clipping plane to 1.0
	v1.Position *= RenderStates.Precomputed.InverseClipNear;
	v2.Position *= RenderStates.Precomputed.InverseClipNear;
	v3.Position *= RenderStates.Precomputed.InverseClipNear;

	bool rendered = false;

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		// All vertices are in front of the near clipping plane.
		rendered = DrawClippedTriangle(v1, v2, v3);
	}
	else
	{
		// 1 or 2 vertices are in front of the near clipping plane.
		ShadowMapVertex v12;
		ShadowMapVertex v23;
		ShadowMapVertex v31;

		v12.Position = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
		v23.Position = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Position, v3.Position);
		v31.Position = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Position, v1.Position);

		if (RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
		{
			v12.TextureCoordinates = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.TextureCoordinates, v2.TextureCoordinates);
			v23.TextureCoordinates = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.TextureCoordinates, v3.TextureCoordinates);
			v31.TextureCoordinates = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.TextureCoordinates, v1.TextureCoordinates);
		}

		if (vertex1Visible && vertex2Visible)
		{
			rendered = DrawClippedTriangle(v31, v1, v23);
			rendered |= DrawClippedTriangle(v1, v2, v23);
		}
		else if (vertex2Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v2);
			rendered |= DrawClippedTriangle(v12, v2, v31);
		}
		else if (vertex1Visible && vertex3Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v23);
			rendered |= DrawClippedTriangle(v3, v1, v23);
		}
		else if (vertex1Visible)
		{
			rendered = DrawClippedTriangle(v1, v12, v31);
		}
		else if (vertex2Visible)
		{
			rendered = DrawClippedTriangle(v2, v23, v12);
		}
		else if (vertex3Visible)
		{
			rendered = DrawClippedTriangle(v3, v31, v23);
		}
	}

	if (rendered)
	{
		Statistics.RenderedTriangleCount++;
	}
}
bool ShadowMapRasterizer::DrawClippedTriangle(const ShadowMapVertex &v1, const ShadowMapVertex &v2, const ShadowMapVertex &v3) const
{
	if (RenderStates.Texture && RenderStates.Texture->HasTransparencyKey)
	{
		return DrawClippedTriangleT<true>(v1, v2, v3);
	}
	else
	{
		return DrawClippedTriangleT<false>(v1, v2, v3);
	}
}
template<bool hasTexture>
bool ShadowMapRasterizer::DrawClippedTriangleT(ShadowMapVertex v1, ShadowMapVertex v2, ShadowMapVertex v3) const
{
	// Project vertices to screen space.
	Vector2i v1Screen = RasterizerMath::ProjectVertex(RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Height, v1.Position, RenderStates.ShadowLightZoom);
	Vector2i v2Screen = RasterizerMath::ProjectVertex(RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Height, v2.Position, RenderStates.ShadowLightZoom);
	Vector2i v3Screen = RasterizerMath::ProjectVertex(RenderStates.ShadowMap.Width, RenderStates.ShadowMap.Height, v3.Position, RenderStates.ShadowLightZoom);

	// Clip to the region of the screen that is being rendered by this thread.
	int32 frameYFrom;
	int32 frameYHeight;
	RasterizerMath::GetWorkloadScreenPart(RenderStates.Workload, RenderStates.ShadowMap.Height, frameYFrom, frameYHeight);

	if (RasterizerMath::ClipTriangleScreenSpace(0, frameYFrom, RenderStates.ShadowMap.Width, frameYHeight, v1Screen, v2Screen, v3Screen))
	{
		// All projected screen coordinates are ouside of the screen.
		return false;
	}

	// Sort vertices by screen Y coordinate, so that v1y < v2y < v3y.
	if (v1Screen.Y > v2Screen.Y)
	{
		std::swap(v1, v2);
		std::swap(v1Screen, v2Screen);
	}

	if (v2Screen.Y > v3Screen.Y)
	{
		std::swap(v2, v3);
		std::swap(v2Screen, v3Screen);

		if (v1Screen.Y > v2Screen.Y)
		{
			std::swap(v1, v2);
			std::swap(v1Screen, v2Screen);
		}
	}

	if (v1Screen.Y == v3Screen.Y)
	{
		// Triangle is a 1px horizontal line.
		return false;
	}

	// Store inverse of Z from here on.
	v1.Position.Z = 1 / v1.Position.Z;
	v2.Position.Z = 1 / v2.Position.Z;
	v3.Position.Z = 1 / v3.Position.Z;

	if (hasTexture)
	{
		// Scale texture coordinates by texture size.
		v1.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v2.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;
		v3.TextureCoordinates *= RenderStates.Precomputed.InverseTextureSize;

		// Texture coordinates must be positive.
		RasterizerMath::MakeTextureCoordinatesPositive(v1.TextureCoordinates, v2.TextureCoordinates, v3.TextureCoordinates);

		v1.TextureCoordinates.X *= RenderStates.Texture->Width * v1.Position.Z;
		v1.TextureCoordinates.Y *= RenderStates.Texture->Height * v1.Position.Z;
		v2.TextureCoordinates.X *= RenderStates.Texture->Width * v2.Position.Z;
		v2.TextureCoordinates.Y *= RenderStates.Texture->Height * v2.Position.Z;
		v3.TextureCoordinates.X *= RenderStates.Texture->Width * v3.Position.Z;
		v3.TextureCoordinates.Y *= RenderStates.Texture->Height * v3.Position.Z;
	}

	float d = (float)(v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	float v13z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d;
	Vector2f v13Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d;

	ShadowMapScanlineParameters p;

	// Compute deltas per pixel and per subdiv (16 pixels).
	int32 v13x = v1Screen.X + (v3Screen.X - v1Screen.X) * (v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	d = 1.0f / (v13x - v2Screen.X);
	p.DeltaZ = (v13z - v2.Position.Z) * d;
	p.DeltaZSubdiv = p.DeltaZ * Subdiv1;
	if (hasTexture) p.DeltaTextureSubdiv = (v13Texture - v2.TextureCoordinates) * (d * Subdiv1);

	int32 yStart = Math::Clamp(v1Screen.Y, frameYFrom, frameYFrom + frameYHeight);
	int32 yEnd = Math::Clamp(v2Screen.Y, frameYFrom, frameYFrom + frameYHeight);

	for (p.Y = yStart; p.Y < yEnd; p.Y++)
	{
		float d1 = (float)(p.Y - v1Screen.Y) / (v2Screen.Y - v1Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v1Screen.X + (v2Screen.X - v1Screen.X) * d1;
		p.V1.Z = v1.Position.Z + (v2.Position.Z - v1.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v1.TextureCoordinates + (v2.TextureCoordinates - v1.TextureCoordinates) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;

		DrawScanlineT<hasTexture>(p);
	}

	yStart = Math::Clamp(v2Screen.Y, frameYFrom, frameYFrom + frameYHeight);
	yEnd = Math::Clamp(v3Screen.Y, frameYFrom, frameYFrom + frameYHeight);

	for (p.Y = yStart; p.Y < yEnd; p.Y++)
	{
		float d1 = (float)(p.Y - v2Screen.Y) / (v3Screen.Y - v2Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v2Screen.X + (v3Screen.X - v2Screen.X) * d1;
		p.V1.Z = v2.Position.Z + (v3.Position.Z - v2.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v2.TextureCoordinates + (v3.TextureCoordinates - v2.TextureCoordinates) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;

		DrawScanlineT<hasTexture>(p);
	}

	return true;
}
template<bool hasTexture>
void ShadowMapRasterizer::DrawScanlineT(ShadowMapScanlineParameters p) const
{
	if (p.V1.X > p.V2.X)
	{
		// Scanline must go from left to right.
		std::swap(p.V1.X, p.V2.X);
		std::swap(p.V1.Z, p.V2.Z);
		if (hasTexture) std::swap(p.V1.Texture, p.V2.Texture);
	}

	if (p.V2.X < 0 || p.V1.X >= RenderStates.ShadowMap.Width)
	{
		// Scanline is outside of the screen.
		return;
	}

	if (p.V1.X < 0)
	{
		// Clip scanline to left screen edge.
		float d = (float)p.V1.X / (p.V2.X - p.V1.X);
		p.V1.X = 0;
		p.V1.Z -= (p.V2.Z - p.V1.Z) * d;
		if (hasTexture) p.V1.Texture -= (p.V2.Texture - p.V1.Texture) * d;
	}

	if (p.V2.X >= RenderStates.ShadowMap.Width)
	{
		// Clip scanline to right screen edge.
		p.V2.X = RenderStates.ShadowMap.Width - 1;
	}

	float *shadowMapBuffer = RenderStates.ShadowMap.GetBuffer<float>(p.V1.X + p.Y * RenderStates.ShadowMap.Width);

	float z = p.V1.Z;
	Vector2f texture = p.V1.Texture;

	if (hasTexture)
	{
		int32 *textureBuffer = RenderStates.Texture->MipMaps[0];
		int32 textureWidthMask = RenderStates.Texture->Width - 1;
		int32 textureHeightMask = RenderStates.Texture->Height - 1;
		int32 textureWidthExponent = RenderStates.Precomputed.TextureWidthExponent;

		int32 subdivCount = ((p.V2.X - p.V1.X) >> SubdivExponent) + 1;
		for (int32 i = 0; i < subdivCount; i++)
		{
			int32 subdivSize = i < subdivCount - 1 ? Subdiv1 : (p.V2.X - p.V1.X) & SubdivMask;

			float zStart = 1 / z;
			float zEnd = 1 / (z + p.DeltaZSubdiv);
			Vector2f subdivTexture = texture * zStart;
			Vector2f subdivIncrementTexture = ((texture + p.DeltaTextureSubdiv) * zEnd - subdivTexture) * InvertedSubdiv1;

			for (int32 j = 0; j < subdivSize; j++)
			{
				if (*shadowMapBuffer < z)
				{
					int32 texel = textureBuffer[(int32)subdivTexture.X & textureWidthMask | ((int32)subdivTexture.Y & textureHeightMask) << textureWidthExponent];

					if (texel != 0xff00ff)
					{
						*shadowMapBuffer = z;
					}
				}

				shadowMapBuffer++;
				z += p.DeltaZ;
				subdivTexture += subdivIncrementTexture;
			}

			texture += p.DeltaTextureSubdiv;
		}
	}
	else
	{
		// No texture -> no subdivs!
		for (int32 i = p.V1.X; i < p.V2.X; i++)
		{
			if (*shadowMapBuffer < z)
			{
				*shadowMapBuffer = z;
			}

			shadowMapBuffer++;
			z += p.DeltaZ;
		}
	}
}