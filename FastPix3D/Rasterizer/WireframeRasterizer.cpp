#include "WireframeRasterizer.h"
#include "../RasterizerMath.h"
#include "../Math/VectorMath.h"

bool WireframeRasterizer::DrawTriangle(const Vertex &_v1, const Vertex &_v2, const Vertex &_v3) const
{
	// Transform vertices to view space.
	vfloat3 v1 = RenderStates.Precomputed.ModelViewMatrix * _v1.Position;
	vfloat3 v2 = RenderStates.Precomputed.ModelViewMatrix * _v2.Position;
	vfloat3 v3 = RenderStates.Precomputed.ModelViewMatrix * _v3.Position;

	// Back-face culling.
	if (RasterizerMath::IsTriangleCulled(RenderStates.CullMode, v1, v2, v3))
	{
		return false;
	}

	// Clip along near clipping plane.
	bool vertex1Visible = v1.Z > RenderStates.ClipNear;
	bool vertex2Visible = v2.Z > RenderStates.ClipNear;
	bool vertex3Visible = v3.Z > RenderStates.ClipNear;

	if (!vertex1Visible && !vertex2Visible && !vertex3Visible)
	{
		// All vertices are behind the near clipping plane.
		return false;
	}

	if (vertex1Visible && vertex2Visible && vertex3Visible)
	{
		return DrawClippedEdge(v1, v2) | DrawClippedEdge(v2, v3) | DrawClippedEdge(v3, v1);
	}
	else if (vertex1Visible && vertex2Visible)
	{
		vfloat3 v23;
		vfloat3 v31;

		ClipEdges(v2, v3, v3, v1, v23, v31);
		return DrawClippedEdge(v1, v2) | DrawClippedEdge(v1, v31) | DrawClippedEdge(v2, v23);
	}
	else if (vertex2Visible && vertex3Visible)
	{
		vfloat3 v12;
		vfloat3 v31;

		ClipEdges(v1, v2, v3, v1, v12, v31);
		return DrawClippedEdge(v2, v3) | DrawClippedEdge(v2, v12) | DrawClippedEdge(v3, v31);
	}
	else if (vertex1Visible && vertex3Visible)
	{
		vfloat3 v12;
		vfloat3 v23;

		ClipEdges(v1, v2, v2, v3, v12, v23);
		return DrawClippedEdge(v1, v3) | DrawClippedEdge(v1, v12) | DrawClippedEdge(v3, v23);
	}
	else if (vertex1Visible)
	{
		vfloat3 v12;
		vfloat3 v31;

		ClipEdges(v1, v2, v3, v1, v12, v31);
		return DrawClippedEdge(v1, v12) | DrawClippedEdge(v1, v31);
	}
	else if (vertex2Visible)
	{
		vfloat3 v12;
		vfloat3 v23;

		ClipEdges(v1, v2, v2, v3, v12, v23);
		return DrawClippedEdge(v2, v12) | DrawClippedEdge(v2, v23);
	}
	else if (vertex3Visible)
	{
		vfloat3 v23;
		vfloat3 v31;

		ClipEdges(v2, v3, v3, v1, v23, v31);
		return DrawClippedEdge(v3, v31) | DrawClippedEdge(v3, v23);
	}
	else
	{
		return false;
	}
}
void WireframeRasterizer::ClipEdges(const vfloat3 &edge1a, const vfloat3 &edge1b, const vfloat3 &edge2a, const vfloat3 &edge2b, vfloat3 &intersection1, vfloat3 &intersection2) const
{
	// When a triangle intersects the near clipping plane, two intersection points need to be computed.

	float t1 = (RenderStates.ClipNear - edge1a.Z) / (edge1b.Z - edge1a.Z);
	float t2 = (RenderStates.ClipNear - edge2a.Z) / (edge2b.Z - edge2a.Z);

	intersection1 = VectorMath::MulAdd(edge1b - edge1a, vfloat3(t1), edge1a);
	intersection2 = VectorMath::MulAdd(edge2b - edge2a, vfloat3(t2), edge2a);
}
bool WireframeRasterizer::DrawClippedEdge(const vfloat3 &v1, const vfloat3 &v2) const
{
	switch (RenderStates.DepthMode)
	{
		case DepthMode::None: return DrawClippedEdge<DepthMode::None>(v1, v2);
		case DepthMode::Read: return DrawClippedEdge<DepthMode::Read>(v1, v2);
		case DepthMode::ReadWrite: return DrawClippedEdge<DepthMode::ReadWrite>(v1, v2);
	}

	return false;
}
template<DepthMode depthMode>
__forceinline bool WireframeRasterizer::DrawClippedEdge(vfloat3 v1, vfloat3 v2) const
{
	int32 frameBufferWidth = RenderStates.FrameBuffer.Width;
	int32 frameBufferHeight = RenderStates.FrameBuffer.Height;

	// Project vertices to screen space.
	v1 = RasterizerMath::Project(v1, RenderStates.Precomputed.ProjectionScale);
	v2 = RasterizerMath::Project(v2, RenderStates.Precomputed.ProjectionScale);
	float v1z = v1.Z;
	float v2z = v2.Z;

	// Project vertices screen space.
	vint2 v1Screen = RasterizerMath::ToScreenSpace(v1, frameBufferWidth, frameBufferHeight);
	vint2 v2Screen = RasterizerMath::ToScreenSpace(v2, frameBufferWidth, frameBufferHeight);

	// Clip line along screen edges.
	if ((uint32)v1Screen.X >= (uint32)frameBufferWidth ||
		(uint32)v1Screen.Y >= (uint32)frameBufferHeight ||
		(uint32)v2Screen.X >= (uint32)frameBufferWidth ||
		(uint32)v2Screen.Y >= (uint32)frameBufferHeight)
	{
		if (v1Screen.X < 0 && v2Screen.X < 0 ||
			v1Screen.Y < 0 && v2Screen.Y < 0 ||
			v1Screen.X >= frameBufferWidth && v2Screen.X >= frameBufferWidth ||
			v1Screen.Y >= frameBufferHeight && v2Screen.Y >= frameBufferHeight)
		{
			// Both vertices are outside the same screen edge.
			return false;
		}

		ClipScreenEdge<depthMode, true>(v1Screen.X, v1Screen.Y, v2Screen.X, v2Screen.Y, 0, v1z, v2z);
		ClipScreenEdge<depthMode, true>(v1Screen.Y, v1Screen.X, v2Screen.Y, v2Screen.X, 0, v1z, v2z);
		ClipScreenEdge<depthMode, false>(v1Screen.X, v1Screen.Y, v2Screen.X, v2Screen.Y, frameBufferWidth - 1, v1z, v2z);
		ClipScreenEdge<depthMode, false>(v1Screen.Y, v1Screen.X, v2Screen.Y, v2Screen.X, frameBufferHeight - 1, v1z, v2z);

		ClipScreenEdge<depthMode, true>(v2Screen.X, v2Screen.Y, v1Screen.X, v1Screen.Y, 0, v2z, v1z);
		ClipScreenEdge<depthMode, true>(v2Screen.Y, v2Screen.X, v1Screen.Y, v1Screen.X, 0, v2z, v1z);
		ClipScreenEdge<depthMode, false>(v2Screen.X, v2Screen.Y, v1Screen.X, v1Screen.Y, frameBufferWidth - 1, v2z, v1z);
		ClipScreenEdge<depthMode, false>(v2Screen.Y, v2Screen.X, v1Screen.Y, v1Screen.X, frameBufferHeight - 1, v2z, v1z);

		if (v1Screen.X < 0 && v2Screen.X < 0 ||
			v1Screen.Y < 0 && v2Screen.Y < 0 ||
			v1Screen.X >= frameBufferWidth && v2Screen.X >= frameBufferWidth ||
			v1Screen.Y >= frameBufferHeight && v2Screen.Y >= frameBufferHeight)
		{
			return false;
		}
	}

	if constexpr (depthMode != DepthMode::None)
	{
		v1z *= RenderStates.WireframeDepthBias;
		v2z *= RenderStates.WireframeDepthBias;
	}

	vint2 delta = VectorMath::Abs(v2Screen - v1Screen);
	float deltaZ = delta.X > delta.Y ? (v2z - v1z) / delta.X : (v2z - v1z) / delta.Y;
	vint2 direction = vint2(v1Screen.X < v2Screen.X ? 1 : -1, v1Screen.Y < v2Screen.Y ? 1 : -1);
	int32 error = delta.X - delta.Y;

	Color *frameBuffer = RenderStates.FrameBuffer.GetBuffer<Color>();
	float *depthBuffer = RenderStates.DepthBuffer.GetBuffer<float>();
	int32 color = RenderStates.WireframeColor.RGB;

	int32 threadCount = WorkPartition.ThreadCount;
	int32 threadCountMask = threadCount - 1;

	if (delta.Y == 0)
	{
		if (WorkPartition.GetOffset(halfspace_div(v1Screen.Y)) == 0)
		{
			while (true)
			{
				int32 offset = v1Screen.X + v1Screen.Y * frameBufferWidth;

				if constexpr (depthMode == DepthMode::None)
				{
					frameBuffer[offset].RGB = color;
				}
				else if (v1z > depthBuffer[offset])
				{
					frameBuffer[offset].RGB = color;
					if constexpr (depthMode == DepthMode::ReadWrite) depthBuffer[offset] = v1z;
				}

				if (v1Screen == v2Screen)
				{
					break;
				}

				v1Screen.X += direction.X;

				if constexpr (depthMode != DepthMode::None)
				{
					v1z += deltaZ;
				}
			}
		}
	}
	else
	{
		vint2 lineStart = v1Screen;
		float lineStartZ = v1z;

		// Find first 8-row band belonging to this thread.
		int32 startBand = halfspace_div(v1Screen.Y);
		int32 band = startBand;

		int32 bandOffset = WorkPartition.GetOffset(band);

		if (direction.Y < 0)
		{
			bandOffset = (threadCount - bandOffset) & threadCountMask;
		}

		band += direction.Y * bandOffset;

		while (true)
		{
			int32 targetY = band == startBand ? lineStart.Y : direction.Y > 0 ? halfspace_mul(band) : halfspace_mul(band + 1) - 1;

			if (direction.Y > 0 ? targetY > v2Screen.Y : targetY < v2Screen.Y)
			{
				break;
			}

			int32 ySteps = Math::Abs(targetY - lineStart.Y);
			int32 xSteps = delta.X > delta.Y
				? (ySteps == 0 ? 0 : 1 + (ySteps * delta.X - ((delta.X + 1) >> 1)) / delta.Y)
				: (ySteps * delta.X + ((delta.Y - 1) >> 1)) / delta.Y;

			v1Screen.X = lineStart.X + direction.X * xSteps;
			v1Screen.Y = targetY;

			error = delta.X - delta.Y - xSteps * delta.Y + ySteps * delta.X;

			if constexpr (depthMode != DepthMode::None)
			{
				v1z = lineStartZ + deltaZ * (delta.X > delta.Y ? xSteps : ySteps);
			}

			for (int32 bandMinY = halfspace_mul(band); (uint32)(v1Screen.Y - bandMinY) < halfspace_mul(1);)
			{
				int32 offset = v1Screen.X + v1Screen.Y * frameBufferWidth;

				if constexpr (depthMode == DepthMode::None)
				{
					frameBuffer[offset].RGB = color;
				}
				else if (v1z > depthBuffer[offset])
				{
					frameBuffer[offset].RGB = color;

					if constexpr (depthMode == DepthMode::ReadWrite)
					{
						depthBuffer[offset] = v1z;
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

				if constexpr (depthMode != DepthMode::None)
				{
					v1z += deltaZ;
				}
			}

			band += direction.Y * threadCount;
		}
	}

	return true;
}
template<DepthMode depthMode, bool clipLower>
__forceinline void WireframeRasterizer::ClipScreenEdge(int32 &coordinate, int32 &coordinateOther, int32 otherCoordinate, int32 otherCoordinateOther, int32 clipValue, float &z, float otherZ)
{
	if constexpr (clipLower)
	{
		if (coordinate >= clipValue || otherCoordinate < clipValue)
		{
			return;
		}
	}
	else
	{
		if (coordinate <= clipValue || otherCoordinate > clipValue)
		{
			return;
		}
	}

	float t = (float)(clipValue - coordinate) / (float)(otherCoordinate - coordinate);

	coordinateOther += (int32)((otherCoordinateOther - coordinateOther) * t);
	coordinate = clipValue;

	if constexpr (depthMode != DepthMode::None)
	{
		z += (otherZ - z) * t;
	}
}