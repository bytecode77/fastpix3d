#include "RenderUnit.h"
#include "Math/VectorMath.h"
#include "Rasterizer/FragmentRasterizer.h"
#include "Rasterizer/ShadowMapRasterizer.h"
#include "Rasterizer/WireframeRasterizer.h"
#include "RasterizerMath.h"

void RenderUnit::ClearFrameBuffer(const RenderStates &renderStates) const
{
	ClearFrameBuffer(renderStates, Color());
}
void RenderUnit::ClearFrameBuffer(const RenderStates &renderStates, byte r, byte g, byte b) const
{
	ClearFrameBuffer(renderStates, Color(r, g, b));
}
void RenderUnit::ClearFrameBuffer(const RenderStates &renderStates, const Color &color) const
{
	std::fill_n(renderStates.FrameBuffer.GetBuffer<int32>(), renderStates.FrameBuffer.Width * renderStates.FrameBuffer.Height, color.RGB);
}
void RenderUnit::ClearDepthBuffer(const RenderStates &renderStates) const
{
	std::fill_n(renderStates.DepthBuffer.GetBuffer<float>(), renderStates.DepthBuffer.Width * renderStates.DepthBuffer.Height, renderStates.ClipNear / renderStates.ClipFar);
}
void RenderUnit::ClearShadowMap(const RenderStates &renderStates) const
{
	std::fill_n(renderStates.ShadowMap.GetBuffer<float>(), renderStates.ShadowMap.Width * renderStates.ShadowMap.Height, 0.0f);
}

void RenderUnit::DrawMesh(const RenderStates &renderStates, WorkPartition workPartition, const Mesh &mesh, const Matrix4 &modelMatrix)
{
	::RenderStates renderStatesCopy = renderStates;
	renderStatesCopy.ModelMatrix = modelMatrix;

	bool hasTransparentSurfaces = false;
	int32 totalTriangleCount = 0;
	int32 renderedTriangleCount = 0;

	// 1.) Render opaque surfaces first.
	for (int32 i = 0; i < mesh.SurfaceCount; i++)
	{
		Surface *surface = mesh.Surfaces[i];

		if (surface->BlendMode == BlendMode::Alpha && surface->Alpha < 1 ||
			surface->BlendMode == BlendMode::Add ||
			surface->BlendMode == BlendMode::Multiply)
		{
			hasTransparentSurfaces = true;
		}
		else
		{
			renderStatesCopy.BlendMode = surface->Texture && surface->Texture->HasTransparencyKey ? BlendMode::TransparencyKey : BlendMode::None;
			renderStatesCopy.CullMode = surface->CullMode;
			renderStatesCopy.Texture = surface->Texture;
			renderStatesCopy.TextureSize = surface->TextureSize;
			renderStatesCopy.SpecularExponent = surface->SpecularExponent;
			renderStatesCopy.SpecularIntensity = surface->SpecularIntensity;

			for (int32 j = 0; j < surface->TriangleCount; j++)
			{
				DrawTriangle(
					renderStatesCopy,
					workPartition,
					*surface->GetTriangleVertex(j, 0),
					*surface->GetTriangleVertex(j, 1),
					*surface->GetTriangleVertex(j, 2),
					totalTriangleCount,
					renderedTriangleCount
				);
			}
		}
	}

	// 2.) Render transparent surfaces with z-writes disabled.
	if (hasTransparentSurfaces)
	{
		for (int32 i = 0; i < mesh.SurfaceCount; i++)
		{
			Surface *surface = mesh.Surfaces[i];

			if (surface->BlendMode == BlendMode::Alpha && surface->Alpha > 0 && surface->Alpha < 1 ||
				surface->BlendMode == BlendMode::Add ||
				surface->BlendMode == BlendMode::Multiply)
			{
				renderStatesCopy.DepthMode = renderStatesCopy.DepthMode == DepthMode::None ? DepthMode::None : DepthMode::Read;
				renderStatesCopy.BlendMode = surface->BlendMode;
				renderStatesCopy.CullMode = surface->CullMode;
				renderStatesCopy.Texture = surface->Texture;
				renderStatesCopy.TextureSize = surface->TextureSize;
				renderStatesCopy.Alpha = surface->Alpha;
				renderStatesCopy.SpecularExponent = surface->SpecularExponent;
				renderStatesCopy.SpecularIntensity = surface->SpecularIntensity;

				for (int32 j = 0; j < surface->TriangleCount; j++)
				{
					DrawTriangle(
						renderStatesCopy,
						workPartition,
						*surface->GetTriangleVertex(j, 0),
						*surface->GetTriangleVertex(j, 1),
						*surface->GetTriangleVertex(j, 2),
						totalTriangleCount,
						renderedTriangleCount
					);
				}
			}
		}
	}

	switch (renderStatesCopy.Rasterizer)
	{
		case Rasterizer::Fragments:
			Statistics.TotalTriangleCount += totalTriangleCount;
			Statistics.RenderedTriangleCount += renderedTriangleCount;
			break;
		case Rasterizer::ShadowMap:
			Statistics.RenderedTriangleCount += renderedTriangleCount;
			break;
	}
}
void RenderUnit::DrawTriangle(const RenderStates &renderStates, WorkPartition workPartition, const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
	int32 totalTriangleCount = 0;
	int32 renderedTriangleCount = 0;
	DrawTriangle(renderStates, workPartition, v1, v2, v3, totalTriangleCount, renderedTriangleCount);

	switch (renderStates.Rasterizer)
	{
		case Rasterizer::Fragments:
			Statistics.TotalTriangleCount += totalTriangleCount;
			Statistics.RenderedTriangleCount += renderedTriangleCount;
			break;
		case Rasterizer::ShadowMap:
			Statistics.RenderedTriangleCount += renderedTriangleCount;
			break;
	}
}
void RenderUnit::RenderFog(const RenderStates &renderStates, WorkPartition workPartition) const
{
	if (renderStates.FogEnable)
	{
		int32 partitionOffset = halfspace_mul(workPartition.GetOffset(0));
		int32 partitionIncrement = halfspace_mul(workPartition.ThreadCount);

		float d = 255 / (renderStates.FogFar - renderStates.FogNear);
		vfloat8 clipNear = vfloat8(renderStates.ClipNear * d);
		vfloat8 fogNear = vfloat8(renderStates.FogNear * d);
		vfloat8 clearDepthValue = vfloat8(renderStates.ClipNear / renderStates.ClipFar);

		vushort16 fogColor = vuint8(renderStates.FogColor.RGB).Low8;

		Color *frameBuffer = renderStates.FrameBuffer.GetBuffer<Color>(partitionOffset * renderStates.FrameBuffer.Width);
		float *depthBuffer = renderStates.DepthBuffer.GetBuffer<float>(partitionOffset * renderStates.FrameBuffer.Width);
		int32 stride = renderStates.FrameBuffer.Width * (partitionIncrement - halfspace_mul(1));

		for (int32 y = partitionOffset; y < renderStates.FrameBuffer.Height; y += partitionIncrement)
		{
			for (int32 x = 0; x < renderStates.FrameBuffer.Width; x++)
			{
				vuint8 pixels = vuint8((vuint8*)frameBuffer);
				vfloat8 depth = vfloat8((vfloat8*)depthBuffer);
				vuint8 writeMask = VectorMath::CmpGt(depth, clearDepthValue); // Write only where Z-Buffer is not cleared to prevent skyboxes from disappearing in fog.

				vbyte32 alpha = VectorMath::Shuffle((vbyte32)VectorMath::Clamp((vint8)VectorMath::MulSub(clipNear, depth.Reciprocal, fogNear), 0, 255), BroadcastByteToInt32Mask);
				vbyte32 oneMinusAlpha = ~alpha;

				vuint8::Write(
					(vuint8*)frameBuffer,
					(vuint8)VectorMath::Pack(
						(fogColor * alpha.Low8) + (pixels.Low8 * oneMinusAlpha.Low8) >> 8,
						(fogColor * alpha.High8) + (pixels.High8 * oneMinusAlpha.High8) >> 8
					),
					writeMask
				);

				frameBuffer += halfspace_mul(1);
				depthBuffer += halfspace_mul(1);
			}

			frameBuffer += stride;
			depthBuffer += stride;
		}
	}
}

void RenderUnit::DrawTriangle(const RenderStates &renderStates, WorkPartition workPartition, const Vertex &v1, const Vertex &v2, const Vertex &v3, int32 &totalTriangleCount, int32 &renderedTriangleCount)
{
	if (workPartition.ThreadIndex == 0)
	{
		totalTriangleCount++;
	}

	switch (renderStates.Rasterizer)
	{
		case Rasterizer::Fragments:
		{
			FragmentRasterizer rasterizer = FragmentRasterizer(renderStates, workPartition);
			if (rasterizer.DrawTriangle(v1, v2, v3) && workPartition.ThreadIndex == 0)
			{
				renderedTriangleCount++;
			}
			break;
		}
		case Rasterizer::Wireframe:
		{
			WireframeRasterizer rasterizer = WireframeRasterizer(renderStates, workPartition);
			if (rasterizer.DrawTriangle(v1, v2, v3) && workPartition.ThreadIndex == 0)
			{
				renderedTriangleCount++;
			}
			break;
		}
		case Rasterizer::ShadowMap:
		{
			ShadowMapRasterizer rasterizer = ShadowMapRasterizer(renderStates, workPartition);
			if (rasterizer.DrawTriangle(v1, v2, v3) && workPartition.ThreadIndex == 0)
			{
				renderedTriangleCount++;
			}
			break;
		}
	}
}