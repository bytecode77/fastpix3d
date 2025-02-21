#include "RenderUnit.h"
#include "RasterizerMath.h"
#include "Rasterizer/Rasterizer.h"
#include "Rasterizer/ShadowMapRasterizer.h"
#include "Rasterizer/ShadowMapStencilRasterizer.h"

void RenderUnit::ClearFrameBuffer() const
{
	memset(RenderStates.FrameBuffer.Buffer, 0, RenderStates.FrameBuffer.Width * RenderStates.FrameBuffer.Height * 4);
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
	memset(RenderStates.DepthBuffer.Buffer, 0, RenderStates.DepthBuffer.Width * RenderStates.DepthBuffer.Height * 4);
}
void RenderUnit::ClearStencilBuffer() const
{
	memset(RenderStates.StencilBuffer.Buffer, 0, RenderStates.StencilBuffer.Width * RenderStates.StencilBuffer.Height);
}
void RenderUnit::ClearShadowMap() const
{
	memset(RenderStates.ShadowMap.Buffer, 0, RenderStates.ShadowMap.Width * RenderStates.ShadowMap.Height * 4);
}

void RenderUnit::DrawMesh(const Mesh& mesh, const Matrix4f &modelSpace)
{
	Matrix4f worldSpace = modelSpace * RenderStates.CameraSpace;
	Matrix4f shadowLightSpace = modelSpace * RenderStates.ShadowLightMatrix;

	::RenderStates previousRenderStates = RenderStates;
	bool hasTransparentSurfaces = false;

	// 1.) Render opaque surfaces first.
	for (int32 i = 0; i < mesh.SurfaceCount; i++)
	{
		Surface *surface = mesh.GetSurface(i);

		if (surface->Opacity >= 1)
		{
			RenderStates.CullMode = surface->CullMode;
			RenderStates.Texture = surface->Texture;
			RenderStates.TextureSize = surface->TextureSize;
			RenderStates.BlendMode = RenderStates.Texture && RenderStates.Texture->HasTransparencyKey ? BlendMode::TransparencyKey : BlendMode::None;

			for (int32 j = 0; j < surface->TriangleCount; j++)
			{
				DrawTriangle(
					worldSpace,
					shadowLightSpace,
					*surface->GetTriangleVertex(j, 0),
					*surface->GetTriangleVertex(j, 1),
					*surface->GetTriangleVertex(j, 2)
				);
			}

			Statistics.TotalTriangleCount += surface->TriangleCount;
		}
		else
		{
			hasTransparentSurfaces = true;
		}
	}

	// 2.) Render transparent surfaces with z-writes disabled.
	if (hasTransparentSurfaces)
	{
		RenderStates.ZWriteEnable = false;
		RenderStates.BlendMode = BlendMode::Alpha;

		for (int32 i = 0; i < mesh.SurfaceCount; i++)
		{
			Surface *surface = mesh.GetSurface(i);

			if (surface->Opacity < 1)
			{
				RenderStates.CullMode = surface->CullMode;
				RenderStates.Texture = surface->Texture;
				RenderStates.TextureSize = surface->TextureSize;
				RenderStates.Alpha = surface->Opacity;

				for (int32 j = 0; j < surface->TriangleCount; j++)
				{
					DrawTriangle(
						worldSpace,
						shadowLightSpace,
						*surface->GetTriangleVertex(j, 0),
						*surface->GetTriangleVertex(j, 1),
						*surface->GetTriangleVertex(j, 2)
					);
				}

				Statistics.TotalTriangleCount += surface->TriangleCount;
			}
		}
	}

	RenderStates = previousRenderStates;
}
void RenderUnit::DrawTriangle(const Matrix4f &worldSpace, const Matrix4f &shadowLightSpace, const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
	switch (RenderStates.RenderPass)
	{
		case RenderPass::Fragments:
		{
			Rasterizer rasterizer = Rasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(worldSpace, v1, v2, v3);
			break;
		}
		case RenderPass::ShadowMap:
		{
			ShadowMapRasterizer rasterizer = ShadowMapRasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(shadowLightSpace, v1, v2, v3);
			break;
		}
		case RenderPass::ShadowMapStencil:
		{
			ShadowMapStencilRasterizer rasterizer = ShadowMapStencilRasterizer(RenderStates, Statistics);
			rasterizer.DrawTriangle(worldSpace, shadowLightSpace, v1, v2, v3);
			break;
		}
	}
}
void RenderUnit::RenderDeferredPass()
{
	if (RenderStates.FogEnable)
	{
		int32 workloadOffset;
		int32 workloadIncrement;
		RasterizerMath::GetWorkloadParameters(RenderStates.Workload, 0, workloadOffset, workloadIncrement);

		float d = 255 / (RenderStates.FogFar - RenderStates.FogNear);
		float e = RenderStates.ClipNear * d;
		float f = RenderStates.FogNear * d;

		// depthMin and depthMax are Z-Buffer equvalents of FogNear and FogFar.
		float depthMin = e / f;
		float depthMax = e / (f + 255);
		int32 fogColorRgb = RenderStates.FogColor.RGB;

		for (int32 y = workloadOffset; y < RenderStates.FrameBuffer.Height; y += workloadIncrement)
		{
			int32 *frameBuffer = RenderStates.FrameBuffer.GetBuffer<int32>(y * RenderStates.FrameBuffer.Width);
			float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>(y * RenderStates.FrameBuffer.Width);

			for (int32 x = 0; x < RenderStates.FrameBuffer.Width; x++)
			{
				if (*depthBuffer > 0 && *depthBuffer < depthMin)
				{
					if (*depthBuffer < depthMax)
					{
						// > FogFar
						*frameBuffer = fogColorRgb;
					}
					else
					{
						// > FogNear and < FogFar
						int32 fog = (int32)(e / *depthBuffer - f);
						int32 inverseFog = 255 - fog;

						*frameBuffer =
							(((byte*)frameBuffer)[2] * inverseFog + RenderStates.FogColor.R * fog) >> 8 << 16 |
							(((byte*)frameBuffer)[1] * inverseFog + RenderStates.FogColor.G * fog) >> 8 << 8 |
							(((byte*)frameBuffer)[0] * inverseFog + RenderStates.FogColor.B * fog) >> 8;
					}
				}

				frameBuffer++;
				depthBuffer++;
			}
		}
	}
}