#include "RenderUnit.h"
#include "Math/VectorMath.h"
#include "Rasterizer/FragmentRasterizer.h"
#include "Rasterizer/ShadowMapRasterizer.h"
#include "Rasterizer/WireframeRasterizer.h"
#include "RasterizerMath.h"

void RenderUnit::ClearFrameBuffer() const
{
	ClearFrameBuffer(Color());
}
void RenderUnit::ClearFrameBuffer(int32 r, int32 g, int32 b) const
{
	ClearFrameBuffer(Color(r, g, b));
}
void RenderUnit::ClearFrameBuffer(const Color &color) const
{
	std::fill_n(RenderStates.FrameBuffer.GetBuffer<int32>(), RenderStates.FrameBuffer.Width * RenderStates.FrameBuffer.Height, color.RGB);
}
void RenderUnit::ClearDepthBuffer() const
{
	std::fill_n(RenderStates.DepthBuffer.GetBuffer<float>(), RenderStates.DepthBuffer.Width * RenderStates.DepthBuffer.Height, RenderStates.ClipNear / RenderStates.ClipFar);
}
void RenderUnit::ClearShadowMap() const
{
	std::fill_n(RenderStates.ShadowMap.GetBuffer<float>(), RenderStates.ShadowMap.Width * RenderStates.ShadowMap.Height, 0);
}

void RenderUnit::DrawMesh(const Mesh& mesh, const Matrix4f &modelMatrix)
{
	::RenderStates previousRenderStates = RenderStates;
	RenderStates.ModelMatrix = modelMatrix;

	bool countTotalTriangles = RasterizerMath::GetWorkloadThreadIndex(RenderStates.Workload) == 0;
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
			RenderStates.BlendMode = surface->Texture && surface->Texture->HasTransparencyKey ? BlendMode::TransparencyKey : BlendMode::None;
			RenderStates.CullMode = surface->CullMode;
			RenderStates.Texture = surface->Texture;
			RenderStates.TextureSize = surface->TextureSize;
			RenderStates.SpecularExponent = surface->SpecularExponent;
			RenderStates.SpecularIntensity = surface->SpecularIntensity;

			for (int32 j = 0; j < surface->TriangleCount; j++)
			{
				DrawTriangle(
					*surface->GetTriangleVertex(j, 0),
					*surface->GetTriangleVertex(j, 1),
					*surface->GetTriangleVertex(j, 2)
				);
			}

			if (countTotalTriangles)
			{
				Statistics.TotalTriangleCount += surface->TriangleCount;
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
				RenderStates.ZWriteEnable = false;
				RenderStates.BlendMode = surface->BlendMode;
				RenderStates.CullMode = surface->CullMode;
				RenderStates.Texture = surface->Texture;
				RenderStates.TextureSize = surface->TextureSize;
				RenderStates.Alpha = surface->Alpha;
				RenderStates.SpecularExponent = surface->SpecularExponent;
				RenderStates.SpecularIntensity = surface->SpecularIntensity;

				for (int32 j = 0; j < surface->TriangleCount; j++)
				{
					DrawTriangle(
						*surface->GetTriangleVertex(j, 0),
						*surface->GetTriangleVertex(j, 1),
						*surface->GetTriangleVertex(j, 2)
					);
				}

				if (countTotalTriangles)
				{
					Statistics.TotalTriangleCount += surface->TriangleCount;
				}
			}
		}
	}

	RenderStates = previousRenderStates;
}
void RenderUnit::DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
	switch (RenderStates.Rasterizer)
	{
		case Rasterizer::Fragments:
		{
			FragmentRasterizer rasterizer = FragmentRasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(v1, v2, v3);
			break;
		}
		case Rasterizer::Wireframe:
		{
			WireframeRasterizer rasterizer = WireframeRasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(v1, v2, v3);
			break;
		}
		case Rasterizer::ShadowMap:
		{
			ShadowMapRasterizer rasterizer = ShadowMapRasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(v1, v2, v3);
			break;
		}
	}
}
void RenderUnit::RenderFog()
{
	if (RenderStates.FogEnable)
	{
		int32 workloadOffset;
		int32 workloadIncrement;
		RasterizerMath::GetWorkloadParameters(RenderStates.Workload, 0, workloadOffset, workloadIncrement);
		workloadOffset = halfspace_mul(workloadOffset);
		workloadIncrement = halfspace_mul(workloadIncrement);

		vfloat8 d = vfloat8(255 / (RenderStates.FogFar - RenderStates.FogNear));
		vfloat8 clipNear = vfloat8(RenderStates.ClipNear) * d;
		vfloat8 fogNear = vfloat8(RenderStates.FogNear) * d;
		vfloat8 clearDepthValue = vfloat8(RenderStates.ClipNear / RenderStates.ClipFar);

		vushort16 fogColor = vuint8(RenderStates.FogColor.RGB).Low8;

		Color *frameBuffer = RenderStates.FrameBuffer.GetBuffer<Color>(workloadOffset * RenderStates.FrameBuffer.Width);
		float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>(workloadOffset * RenderStates.FrameBuffer.Width);
		int32 stride = RenderStates.FrameBuffer.Width * (workloadIncrement - halfspace_mul(1));

		for (int32 y = workloadOffset; y < RenderStates.FrameBuffer.Height; y += workloadIncrement)
		{
			for (int32 x = 0; x < RenderStates.FrameBuffer.Width; x++)
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