#include "ShadowMapStencilRasterizer.h"
#include "Vector2i.h"
#include "RasterizerMath.h"

static const int32 SubdivExponent = 4;
static const int32 Subdiv1 = 1 << SubdivExponent;
static const int32  SubdivMask = Subdiv1 - 1;
static constexpr float InvertedSubdiv1 = 1.0f / Subdiv1;

ShadowMapStencilRasterizer::ShadowMapStencilRasterizer(const ::RenderStates &renderStates, RenderStatistics &statistics) :
	RenderStates(renderStates),
	Statistics(statistics)
{
}
void ShadowMapStencilRasterizer::DrawTriangle(const Matrix4f &worldSpace, const Matrix4f &shadowLightSpace, const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	ShadowMapStencilVertex v1 = ShadowMapStencilVertex(_v1);
	ShadowMapStencilVertex v2 = ShadowMapStencilVertex(_v2);
	ShadowMapStencilVertex v3 = ShadowMapStencilVertex(_v3);

	// Transform vertices and shadow map coordinates.
	v1.Position = worldSpace * v1.Position;
	v2.Position = worldSpace * v2.Position;
	v3.Position = worldSpace * v3.Position;
	v1.Uvw = shadowLightSpace * v1.Uvw;
	v2.Uvw = shadowLightSpace * v2.Uvw;
	v3.Uvw = shadowLightSpace * v3.Uvw;

	// Back-face culling.
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1.Position, v2.Position, v3.Position))
	{
		return;
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
		ShadowMapStencilVertex v12;
		ShadowMapStencilVertex v23;
		ShadowMapStencilVertex v31;

		v12.Position = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
		v23.Position = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Position, v3.Position);
		v31.Position = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Position, v1.Position);

		v12.Uvw = Math::Interpolate(1, v1.Position.Z, v2.Position.Z, v1.Uvw, v2.Uvw);
		v23.Uvw = Math::Interpolate(1, v2.Position.Z, v3.Position.Z, v2.Uvw, v3.Uvw);
		v31.Uvw = Math::Interpolate(1, v3.Position.Z, v1.Position.Z, v3.Uvw, v1.Uvw);

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
bool ShadowMapStencilRasterizer::DrawClippedTriangle(const ShadowMapStencilVertex &v1, const ShadowMapStencilVertex &v2, const ShadowMapStencilVertex &v3) const
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
bool ShadowMapStencilRasterizer::DrawClippedTriangleT(ShadowMapStencilVertex v1, ShadowMapStencilVertex v2, ShadowMapStencilVertex v3) const
{
	// Project vertices to screen space.
	Vector2i v1Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v1.Position, RenderStates.Zoom);
	Vector2i v2Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v2.Position, RenderStates.Zoom);
	Vector2i v3Screen = RasterizerMath::ProjectVertex(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v3.Position, RenderStates.Zoom);

	if (RasterizerMath::ClipTriangleScreenSpace(RenderStates.FrameBuffer.Width, RenderStates.FrameBuffer.Height, v1Screen, v2Screen, v3Screen))
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

	// Uvw.X/Y = Shadow map texture coordinates
	// Uvw.Z   = Depth to light source
	float d = .5f * RenderStates.ShadowLightZoom;
	v1.Uvw.X = .5f + v1.Uvw.X * d / v1.Uvw.Z;
	v1.Uvw.Y = .5f - v1.Uvw.Y * d / v1.Uvw.Z;
	v2.Uvw.X = .5f + v2.Uvw.X * d / v2.Uvw.Z;
	v2.Uvw.Y = .5f - v2.Uvw.Y * d / v2.Uvw.Z;
	v3.Uvw.X = .5f + v3.Uvw.X * d / v3.Uvw.Z;
	v3.Uvw.Y = .5f - v3.Uvw.Y * d / v3.Uvw.Z;

	if (v1.Uvw.X < 0 && v2.Uvw.X < 0 && v3.Uvw.X < 0 ||
		v1.Uvw.X > 1 && v2.Uvw.X > 1 && v3.Uvw.X > 1 ||
		v1.Uvw.Y < 0 && v2.Uvw.Y < 0 && v3.Uvw.Y < 0 ||
		v1.Uvw.Y > 1 && v2.Uvw.Y > 1 && v3.Uvw.Y > 1)
	{
		// All projected shadow map texture coordinates are outside of the shadow light's perspective.
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

	// Shadow maps need to be perspective corrected by both the depth and the depth of the light source.
	v1.Uvw.X *= RenderStates.ShadowMap.Width * v1.Position.Z * v1.Uvw.Z;
	v1.Uvw.Y *= RenderStates.ShadowMap.Height * v1.Position.Z * v1.Uvw.Z;
	v1.Uvw.Z *= v1.Position.Z;
	v2.Uvw.X *= RenderStates.ShadowMap.Width * v2.Position.Z * v2.Uvw.Z;
	v2.Uvw.Y *= RenderStates.ShadowMap.Height * v2.Position.Z * v2.Uvw.Z;
	v2.Uvw.Z *= v2.Position.Z;
	v3.Uvw.X *= RenderStates.ShadowMap.Width * v3.Position.Z * v3.Uvw.Z;
	v3.Uvw.Y *= RenderStates.ShadowMap.Height * v3.Position.Z * v3.Uvw.Z;
	v3.Uvw.Z *= v3.Position.Z;

	d = (float)(v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	float v13z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d;
	Vector2f v13Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d;
	Vector3f v13Uvw = v1.Uvw + (v3.Uvw - v1.Uvw) * d;

	ShadowMapStencilScanlineParameters p;

	// Compute deltas per pixel and per subdiv (16 pixels).
	int32 v13x = v1Screen.X + (v3Screen.X - v1Screen.X) * (v2Screen.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);
	d = 1.0f / (v13x - v2Screen.X);
	p.DeltaZ = (v13z - v2.Position.Z) * d;
	p.DeltaZSubdiv = p.DeltaZ * Subdiv1;
	if (hasTexture) p.DeltaTextureSubdiv = (v13Texture - v2.TextureCoordinates) * (d * Subdiv1);
	p.DeltaUvwSubdiv = (v13Uvw - v2.Uvw) * (d * Subdiv1);

	int32 yStart = Math::Clamp(v1Screen.Y, 0, RenderStates.FrameBuffer.Height);
	int32 yEnd = Math::Clamp(v2Screen.Y, 0, RenderStates.FrameBuffer.Height);

	int32 workloadOffset;
	int32 workloadIncrement;
	RasterizerMath::GetWorkloadParameters(RenderStates.Workload, yStart, workloadOffset, workloadIncrement);

	for (p.Y = yStart + workloadOffset; p.Y < yEnd; p.Y += workloadIncrement)
	{
		float d1 = (float)(p.Y - v1Screen.Y) / (v2Screen.Y - v1Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v1Screen.X + (v2Screen.X - v1Screen.X) * d1;
		p.V1.Z = v1.Position.Z + (v2.Position.Z - v1.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v1.TextureCoordinates + (v2.TextureCoordinates - v1.TextureCoordinates) * d1;
		p.V1.Uvw = v1.Uvw + (v2.Uvw - v1.Uvw) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;
		p.V2.Uvw = v1.Uvw + (v3.Uvw - v1.Uvw) * d2;

		DrawScanlineT<hasTexture>(p);
	}

	yStart = Math::Clamp(v2Screen.Y, 0, RenderStates.FrameBuffer.Height);
	yEnd = Math::Clamp(v3Screen.Y, 0, RenderStates.FrameBuffer.Height);

	RasterizerMath::GetWorkloadParameters(RenderStates.Workload, yStart, workloadOffset, workloadIncrement);

	for (p.Y = yStart + workloadOffset; p.Y < yEnd; p.Y += workloadIncrement)
	{
		float d1 = (float)(p.Y - v2Screen.Y) / (v3Screen.Y - v2Screen.Y);
		float d2 = (float)(p.Y - v1Screen.Y) / (v3Screen.Y - v1Screen.Y);

		p.V1.X = v2Screen.X + (v3Screen.X - v2Screen.X) * d1;
		p.V1.Z = v2.Position.Z + (v3.Position.Z - v2.Position.Z) * d1;
		if (hasTexture) p.V1.Texture = v2.TextureCoordinates + (v3.TextureCoordinates - v2.TextureCoordinates) * d1;
		p.V1.Uvw = v2.Uvw + (v3.Uvw - v2.Uvw) * d1;

		p.V2.X = v1Screen.X + (v3Screen.X - v1Screen.X) * d2;
		p.V2.Z = v1.Position.Z + (v3.Position.Z - v1.Position.Z) * d2;
		if (hasTexture) p.V2.Texture = v1.TextureCoordinates + (v3.TextureCoordinates - v1.TextureCoordinates) * d2;
		p.V2.Uvw = v1.Uvw + (v3.Uvw - v1.Uvw) * d2;

		DrawScanlineT<hasTexture>(p);
	}

	return true;
}
template<bool hasTexture>
void ShadowMapStencilRasterizer::DrawScanlineT(ShadowMapStencilScanlineParameters p) const
{
	if (p.V1.X > p.V2.X)
	{
		// Scanline must go from left to right.
		std::swap(p.V1.X, p.V2.X);
		std::swap(p.V1.Z, p.V2.Z);
		if (hasTexture) std::swap(p.V1.Texture, p.V2.Texture);
		std::swap(p.V1.Uvw, p.V2.Uvw);
	}

	if (p.V2.X < 0 || p.V1.X >= RenderStates.FrameBuffer.Width)
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
		p.V1.Uvw -= (p.V2.Uvw - p.V1.Uvw) * d;
	}

	if (p.V2.X >= RenderStates.FrameBuffer.Width)
	{
		// Clip scanline to right screen edge.
		p.V2.X = RenderStates.FrameBuffer.Width - 1;
	}

	float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>(p.V1.X + p.Y * RenderStates.FrameBuffer.Width);
	byte *stencilBuffer = RenderStates.StencilBuffer.GetBuffer<byte>(p.V1.X + p.Y * RenderStates.FrameBuffer.Width);
	float *shadowMapBuffer = RenderStates.ShadowMap.GetBuffer<float>();
	int32 shadowMapWidthMask = RenderStates.ShadowMap.Width - 1;
	int32 shadowMapHeightMask = RenderStates.ShadowMap.Height - 1;
	int32 shadowMapWidthExponent = RenderStates.Precomputed.ShadowMapWidthExponent;
	int32 shadowMapHeightExponent = RenderStates.Precomputed.ShadowMapHeightExponent;

	int32 *textureBuffer = hasTexture ? RenderStates.Texture->MipMaps[0] : nullptr;
	int32 textureWidthMask = hasTexture ? RenderStates.Texture->Width - 1 : 0;
	int32 textureHeightMask = hasTexture ? RenderStates.Texture->Height - 1 : 0;
	int32 textureWidthExponent = hasTexture ? RenderStates.Precomputed.TextureWidthExponent : 0;

	float z = p.V1.Z;
	Vector2f texture = p.V1.Texture;
	Vector3f uvw = p.V1.Uvw;
	Vector2f subdivTexture;
	Vector3f subdivUvw;
	Vector2f subdivIncrementTexture;
	Vector3f subdivIncrementUvw;
	float subdivW;
	float subdivIncrementW;

	int32 subdivCount = ((p.V2.X - p.V1.X) >> SubdivExponent) + 1;
	for (int32 i = 0; i < subdivCount; i++)
	{
		int32 subdivSize = i < subdivCount - 1 ? Subdiv1 : (p.V2.X - p.V1.X) & SubdivMask;

		float zStart = 1 / z;
		float zEnd = 1 / (z + p.DeltaZSubdiv);
		if (hasTexture) subdivTexture = texture * zStart;
		if (hasTexture) subdivIncrementTexture = ((texture + p.DeltaTextureSubdiv) * zEnd - subdivTexture) * InvertedSubdiv1;

		subdivUvw = uvw / uvw.Z;
		subdivIncrementUvw = ((uvw + p.DeltaUvwSubdiv) / (uvw.Z + p.DeltaUvwSubdiv.Z) - subdivUvw) * InvertedSubdiv1;

		subdivW = uvw.Z * zStart;
		subdivIncrementW = ((uvw.Z + p.DeltaUvwSubdiv.Z) * zEnd - subdivW) * InvertedSubdiv1;

		for (int32 j = 0; j < subdivSize; j++)
		{
			if (*depthBuffer++ <= z)
			{
				if (!hasTexture || textureBuffer[(int32)subdivTexture.X & textureWidthMask | ((int32)subdivTexture.Y & textureHeightMask) << textureWidthExponent] != 0xff00ff)
				{
					int32 u = (int32)subdivUvw.X;
					int32 v = (int32)subdivUvw.Y;

					// We have zero overdraw here, because the depth buffer is already populated by the previous render pass.
					// Therefore, branching does not affect performance as much.

					if (!(u >> shadowMapWidthExponent) && !(v >> shadowMapHeightExponent))
					{
						float texel = shadowMapBuffer[u | v << shadowMapWidthExponent];
						*stencilBuffer = subdivW - RenderStates.ClipNear / texel > RenderStates.ShadowMapDepthBias;
					}
				}
			}

			stencilBuffer++;
			z += p.DeltaZ;
			if (hasTexture) subdivTexture += subdivIncrementTexture;
			subdivUvw += subdivIncrementUvw;
			subdivW += subdivIncrementW;
		}

		if (hasTexture) texture += p.DeltaTextureSubdiv;
		uvw += p.DeltaUvwSubdiv;
	}
}