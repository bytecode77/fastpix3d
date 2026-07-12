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
void RenderUnit::ClearFrameBuffer(const RenderStates &renderStates, int32 r, int32 g, int32 b) const
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
	std::fill_n(renderStates.ShadowMap.GetBuffer<float>(), renderStates.ShadowMap.Width * renderStates.ShadowMap.Height, 0);
}

void RenderUnit::DrawMesh(const RenderStates &renderStates, const Mesh& mesh, const Matrix4f &modelMatrix)
{
	::RenderStates renderStatesCopy = renderStates;
	renderStatesCopy.ModelMatrix = modelMatrix;

	bool hasTransparentSurfaces = false;

	// 1.) Render opaque surfaces first.
	for (int32 i = 0; i < mesh.SurfaceCount; i++)
	{
		Surface *surface = mesh.GetSurface(i);

		if (surface->BlendMode == BlendMode::Alpha && surface->Alpha < 1 || surface->BlendMode == BlendMode::Multiply || surface->BlendMode == BlendMode::Add)
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
					*surface->GetTriangleVertex(j, 0),
					*surface->GetTriangleVertex(j, 1),
					*surface->GetTriangleVertex(j, 2)
				);
			}
		}
	}

	// 3.) Render transparent surfaces with z-writes disabled.
	if (hasTransparentSurfaces)
	{
		for (int32 i = 0; i < mesh.SurfaceCount; i++)
		{
			Surface *surface = mesh.GetSurface(i);

			if (surface->BlendMode == BlendMode::Alpha && surface->Alpha < 1 || surface->BlendMode == BlendMode::Multiply || surface->BlendMode == BlendMode::Add)
			{
				renderStatesCopy.ZWriteEnable = false;
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
						*surface->GetTriangleVertex(j, 0),
						*surface->GetTriangleVertex(j, 1),
						*surface->GetTriangleVertex(j, 2)
					);
				}
			}
		}
	}
}
void RenderUnit::DrawTriangle(const RenderStates &renderStates, const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
	bool countTriangles = RasterizerMath::GetWorkloadThreadIndex(renderStates.Workload) == 0;
	bool countRenderedTriangles = countTriangles && renderStates.CountRenderedTriangles;
	bool countTotalTriangles = countTriangles && renderStates.CountTotalTriangles;

	switch (renderStates.Rasterizer)
	{
		case Rasterizer::Fragments:
		{
			FragmentRasterizer rasterizer = FragmentRasterizer(renderStates);
			if (rasterizer.DrawTriangle(v1, v2, v3) && countRenderedTriangles)
			{
				Statistics.RenderedTriangleCount++;
			}
			break;
		}
		case Rasterizer::Wireframe:
		{
			WireframeRasterizer rasterizer = WireframeRasterizer(renderStates);
			if (rasterizer.DrawTriangle(v1, v2, v3) && countRenderedTriangles)
			{
				Statistics.RenderedTriangleCount++;
			}
			break;
		}
		case Rasterizer::ShadowMap:
		{
			ShadowMapRasterizer rasterizer = ShadowMapRasterizer(renderStates);
			if (rasterizer.DrawTriangle(v1, v2, v3) && countRenderedTriangles)
			{
				Statistics.RenderedTriangleCount++;
			}
			break;
		}
	}

	if (countTotalTriangles)
	{
		Statistics.TotalTriangleCount++;
	}
}
void RenderUnit::RenderFog(const RenderStates &renderStates) const
{
	if (renderStates.FogEnable)
	{
		int32 workloadOffset;
		int32 workloadIncrement;
		RasterizerMath::GetWorkloadParameters(renderStates.Workload, 0, workloadOffset, workloadIncrement);
		workloadOffset = halfspace_mul(workloadOffset);
		workloadIncrement = halfspace_mul(workloadIncrement);

		float d = 255 / (renderStates.FogFar - renderStates.FogNear);
		vfloat8 clipNear = vfloat8(renderStates.ClipNear * d);
		vfloat8 fogNear = vfloat8(renderStates.FogNear * d);
		vfloat8 clearDepthValue = vfloat8(renderStates.ClipNear / renderStates.ClipFar);

		vushort16 fogColor = vuint8(renderStates.FogColor.RGB).Low8;

		Color *frameBuffer = renderStates.FrameBuffer.GetBuffer<Color>(workloadOffset * renderStates.FrameBuffer.Width);
		float *depthBuffer = renderStates.DepthBuffer.GetBuffer<float>(workloadOffset * renderStates.FrameBuffer.Width);
		int32 stride = renderStates.FrameBuffer.Width * (workloadIncrement - halfspace_mul(1));

		for (int32 y = workloadOffset; y < renderStates.FrameBuffer.Height; y += workloadIncrement)
		{
			for (int32 x = 0; x < renderStates.FrameBuffer.Width; x++)
			{
				vuint8 pixels = vuint8((vuint8*)frameBuffer);
				vfloat8 depth = vfloat8((vfloat8*)depthBuffer);
				vuint8 writeMask = VectorMath::CmpGt(depth, clearDepthValue); // Write only where Z-Buffer is not cleared to prevent skyboxes from disappearing in fog.

				vbyte32 alpha = VectorMath::Shuffle((vbyte32)VectorMath::Clamp((vint8)(clipNear / depth - fogNear), 0, 255), BroadcastByteToInt32Mask);
				vbyte32 oneMinusAlpha = vbyte32(255) - alpha;

				vushort16 blendedLo = (fogColor * alpha.Low8 >> 8) + (pixels.Low8 * oneMinusAlpha.Low8 >> 8);
				vushort16 blendedHi = (fogColor * alpha.High8 >> 8) + (pixels.High8 * oneMinusAlpha.High8 >> 8);
				vuint8::Write((vuint8*)frameBuffer, VectorMath::Pack(blendedLo, blendedHi), writeMask);

				frameBuffer += halfspace_mul(1);
				depthBuffer += halfspace_mul(1);
			}

			frameBuffer += stride;
			depthBuffer += stride;
		}
	}
}