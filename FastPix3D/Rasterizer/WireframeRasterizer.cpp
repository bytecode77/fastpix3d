#include "WireframeRasterizer.h"
#include "../RasterizerMath.h"

void WireframeRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	WireframeRasterizerVertex v1 = WireframeRasterizerVertex(_v1);
	WireframeRasterizerVertex v2 = WireframeRasterizerVertex(_v2);
	WireframeRasterizerVertex v3 = WireframeRasterizerVertex(_v3);

	// Transform vertices to view space.
	v1.Position = RenderStates.Precomputed.ModelViewMatrix * v1.Position;
	v2.Position = RenderStates.Precomputed.ModelViewMatrix * v2.Position;
	v3.Position = RenderStates.Precomputed.ModelViewMatrix * v3.Position;

	// Back-face culling.
	bool isFrontFace;
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1.Position, v2.Position, v3.Position, isFrontFace))
	{
		return;
	}

	bool rendered = false;

	rendered |= DrawEdge(v1, v2);
	rendered |= DrawEdge(v2, v3);
	rendered |= DrawEdge(v3, v1);

	if (rendered && RasterizerMath::GetWorkloadThreadIndex(RenderStates.Workload) == 0)
	{
		Statistics.RenderedTriangleCount++;
	}
}
bool WireframeRasterizer::DrawEdge(const WireframeRasterizerVertex &v1, const WireframeRasterizerVertex &v2) const
{
	if (RenderStates.ZEnable)
	{
		if (RenderStates.ZWriteEnable)
		{
			return DrawEdge<true, true>(v1, v2);
		}
		else
		{
			return DrawEdge<true, false>(v1, v2);
		}
	}
	else
	{
		if (RenderStates.ZWriteEnable)
		{
			return DrawEdge<false, true>(v1, v2);
		}
		else
		{
			return DrawEdge<false, false>(v1, v2);
		}
	}
}
template<bool zEnable, bool zWriteEnable>
bool WireframeRasterizer::DrawEdge(WireframeRasterizerVertex v1, WireframeRasterizerVertex v2) const
{
	// Clip along near clipping plane.
	bool vertex1Visible = v1.Position.Z > RenderStates.ClipNear;
	bool vertex2Visible = v2.Position.Z > RenderStates.ClipNear;

	if (!vertex1Visible && !vertex2Visible)
	{
		// Both ends of the line are behind the near clipping plane.
		return false;
	}

	if (!vertex1Visible || !vertex2Visible)
	{
		// One vertex is behind the near clipping plane.
		(vertex1Visible ? v2 : v1).Position = Math::Interpolate(RenderStates.ClipNear, v1.Position.Z, v2.Position.Z, v1.Position, v2.Position);
	}

	int32 frameBufferWidth = RenderStates.FrameBuffer.Width;
	int32 frameBufferHeight = RenderStates.FrameBuffer.Height;
	float wireframeDepthBias = RenderStates.WireframeDepthBias;

	// Transform vertices to clip space.
	v1.Position = RasterizerMath::ToClipSpace(v1.Position, frameBufferWidth, frameBufferHeight, RenderStates.Zoom, RenderStates.ClipNear);
	v2.Position = RasterizerMath::ToClipSpace(v2.Position, frameBufferWidth, frameBufferHeight, RenderStates.Zoom, RenderStates.ClipNear);

	// Project vertices screen space.
	vint2 v1Screen = RasterizerMath::ToScreenSpace(v1.Position, frameBufferWidth, frameBufferHeight);
	vint2 v2Screen = RasterizerMath::ToScreenSpace(v2.Position, frameBufferWidth, frameBufferHeight);

	// Clip line along screen space edges.
	RasterizerMath::ClipEdgeToScreenSpace(frameBufferWidth, frameBufferHeight, v1.Position, v2.Position, v1Screen, v2Screen);
	RasterizerMath::ClipEdgeToScreenSpace(frameBufferWidth, frameBufferHeight, v2.Position, v1.Position, v2Screen, v1Screen);

	// Clip in screen space to guarantee that the line is inside the screen, to avoid per-pixel checks.
	if (v1Screen.X < 0 && v2Screen.X < 0 ||
		v1Screen.Y < 0 && v2Screen.Y < 0 ||
		v1Screen.X >= frameBufferWidth && v2Screen.X >= frameBufferWidth ||
		v1Screen.Y >= frameBufferHeight && v2Screen.Y >= frameBufferHeight)
	{
		return false;
	}

	vint2 delta = VectorMath::Abs(v2Screen - v1Screen);
	float deltaZ = zEnable || zWriteEnable ? delta.X > delta.Y ? (v2.Position.Z - v1.Position.Z) / delta.X : (v2.Position.Z - v1.Position.Z) / delta.Y : 0;
	vint2 direction = vint2(v1Screen.X < v2Screen.X ? 1 : -1, v1Screen.Y < v2Screen.Y ? 1 : -1);
	int32 error = delta.X - delta.Y;

	Color *frameBuffer = RenderStates.FrameBuffer.GetBuffer<Color>();
	float *depthBuffer = zEnable || zWriteEnable ? RenderStates.DepthBuffer.GetBuffer<float>() : nullptr;
	int32 color = RenderStates.WireframeColor.RGB;

	int32 workloadThreadIndex = RasterizerMath::GetWorkloadThreadIndex(RenderStates.Workload);
	int32 workloadThreadCountMinusOne = RasterizerMath::GetWorkloadThreadCount(RenderStates.Workload) - 1;

	while (true)
	{
		if ((halfspace_div(v1Screen.Y) & workloadThreadCountMinusOne) == workloadThreadIndex)
		{
			int32 offset = v1Screen.X + v1Screen.Y * frameBufferWidth;

			if (!zEnable || v1.Position.Z * wireframeDepthBias > depthBuffer[offset])
			{
				frameBuffer[offset].RGB = color;
				if constexpr (zWriteEnable) depthBuffer[offset] = v1.Position.Z;
			}
		}

		if (v1Screen == v2Screen)
		{
			break;
		}

		int32 error2 = error << 1;
		if (-delta.Y < error2)
		{
			error -= delta.Y;
			v1Screen.X += direction.X;
		}

		if (delta.X > error2)
		{
			error += delta.X;
			v1Screen.Y += direction.Y;
		}

		if constexpr (zEnable || zWriteEnable) v1.Position.Z += deltaZ;
	}

	return true;
}