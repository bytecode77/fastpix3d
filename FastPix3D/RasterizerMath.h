#pragma once
#include "FastPix3D.h"
#include "Vector2f.h"
#include "Vector2i.h"
#include "Math.h"

// Commonly used algorithms.
//   * All of them are inlined by the compiler (I have checked the assembly).
//   * Branchless is used where possible.

class RasterizerMath
{
public:
	RasterizerMath() = delete;

	static bool IsTriangleCulled(CullMode cullMode, const Vector3f &v1, const Vector3f &v2, const Vector3f &v3)
	{
		return cullMode != CullMode::None && (cullMode == CullMode::Front) == RasterizerMath::IsTriangleFrontFace(v1, v2, v3);
	}
	static bool IsTriangleFrontFace(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3)
	{
		Vector3f normal = v1 * (v3 - v2).CrossProduct(v2 - v1);
		return normal.X + normal.Y + normal.Z > 0;
	}
	static Vector2i ProjectVertex(int32 screenWidth, int32 screenHeight, const Vector3f &position, float zoom)
	{
		float d = (screenWidth / 2) * zoom / position.Z;

		return Vector2i(
			screenWidth / 2 + (int32)(position.X * d),
			screenHeight / 2 - (int32)(position.Y * d));
	}
	static Vector3f DecomposeProjection(int32 screenWidth, int32 screenHeight, int32 screenX, int32 screenY, float depthBufferValue, float zoom)
	{
		float d = (screenWidth / 2) * zoom * depthBufferValue;

		return Vector3f(
			(screenX - screenWidth / 2) / d,
			-(screenY - screenHeight / 2) / d,
			1 / depthBufferValue);
	}
	static bool ClipTriangleScreenSpace(int32 width, int32 height, const Vector2i &v1, const Vector2i &v2, const Vector2i &v3)
	{
		return
			v1.X < 0 && v2.X < 0 && v3.X < 0 ||
			v1.Y < 0 && v2.Y < 0 && v3.Y < 0 ||
			v1.X >= width && v2.X >= width && v3.X >= width ||
			v1.Y >= height && v2.Y >= height && v3.Y >= height;
	}
	static void MakeTextureCoordinatesPositive(Vector2f &v1, Vector2f &v2, Vector2f &v3)
	{
		float offset = Math::Floor(1 - Math::Min(v1.X, Math::Min(v2.X, v3.X)));

		v1.X += offset;
		v2.X += offset;
		v3.X += offset;

		offset = Math::Floor(1 - Math::Min(v1.Y, Math::Min(v2.Y, v3.Y)));

		v1.Y += offset;
		v2.Y += offset;
		v3.Y += offset;
	}
	static void GetWorkloadParameters(Workload workload, int32 y, int32 &workloadOffset, int32 &workloadIncrement)
	{
		// Distribute the workload so that every thread renders every n'th scanline.
		// offset: The first scanline that the thread will render.
		// increment: The vertical increment after each scanline is rendered.

		int32 threadIndex = (int32)workload & 0xff;
		int32 threadCount = ((int32)workload >> 8) & 0xff;
		int32 threadCountMinusOne = (int32)workload >> 16;

		workloadOffset = (threadIndex - (y & threadCountMinusOne)) & threadCountMinusOne;
		workloadIncrement = threadCount;
	}
	static int32 GetMipLevel(const Vector2f &inverseTextureCoordinates, int32 maxMipLevels)
	{
		int32 mipLevel = 0;
		float inversePixelSize = Math::Max(Math::Abs(inverseTextureCoordinates.X), Math::Abs(inverseTextureCoordinates.Y));

		if (inversePixelSize > 1)
		{
			mipLevel = 1;
			if (inversePixelSize > 2)
			{
				mipLevel = 2;
				if (inversePixelSize > 4)
				{
					mipLevel = 3;
					if (inversePixelSize > 8)
					{
						mipLevel = 4;
						if (inversePixelSize > 16)
						{
							mipLevel = 5;
							if (inversePixelSize > 32)
							{
								mipLevel = 6;
								if (inversePixelSize > 64)
								{
									mipLevel = 7;
								}
							}
						}
					}
				}
			}
		}

		return Math::Min(mipLevel, maxMipLevels);
	}
};