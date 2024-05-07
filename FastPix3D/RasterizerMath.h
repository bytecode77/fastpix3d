#pragma once
#include "FastPix3D.h"
#include "Vector2f.h"
#include "Math.h"

// Commonly used algorithms.
//   * All of them are inlined by the compiler (I have checked the assembly).
//   * Branchless is used where possible.

class RasterizerMath
{
public:
	RasterizerMath() = delete;

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
	static bool ClipTriangleScreenSpace(int32 x, int32 y, int32 width, int32 height, const Vector2i &v1, const Vector2i &v2, const Vector2i &v3)
	{
		return
			v1.X < x && v2.X < x && v3.X < x ||
			v1.Y < y && v2.Y < y && v3.Y < y ||
			v1.X >= x + width && v2.X >= x + width && v3.X >= x + width ||
			v1.Y >= y + height && v2.Y >= y + height && v3.Y >= y + height;
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
	static void GetWorkloadScreenPart(Workload workload, int32 screenHeight, int32 &from, int32 &height)
	{
		switch (workload)
		{
			case Workload::Full:
				from = 0;
				height = screenHeight;
				break;
			case Workload::Half1:
				from = 0;
				height = screenHeight / 2;
				break;
			case Workload::Half2:
				from = screenHeight / 2;
				height = screenHeight - from;
				break;
			case Workload::Quarter1:
				from = 0;
				height = screenHeight / 4;
				break;
			case Workload::Quarter2:
				from = screenHeight / 4;
				height = screenHeight / 4;
				break;
			case Workload::Quarter3:
				from = screenHeight / 4 * 2;
				height = screenHeight / 4;
				break;
			case Workload::Quarter4:
				from = screenHeight / 4 * 3;
				height = screenHeight - from;
				break;
		}
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