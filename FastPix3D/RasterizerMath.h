#pragma once
#include "FastPix3D.h"
#include "Math/Math_.h"
#include "Math/VectorMath.h"

// Commonly used algorithms.
//   * All of them are inlined by the compiler (I have checked the assembly).
//   * Branchless implementations where possible.

class RasterizerMath
{
public:
	RasterizerMath() = delete;

	__forceinline static int32 GetWorkloadThreadIndex(Workload workload)
	{
		return (int32)workload & 0xff;
	}
	__forceinline static int32 GetWorkloadThreadCount(Workload workload)
	{
		return (int32)workload >> 8;
	}
	__forceinline static void GetWorkloadParameters(Workload workload, int32 y, int32 &workloadOffset, int32 &workloadIncrement)
	{
		// Distribute the workload so that every thread renders every n'th line.
		// offset: The first line that the thread will render.
		// increment: The vertical increment after each line is rendered.

		int32 threadIndex = (int32)workload & 0xff;
		int32 threadCount = (int32)workload >> 8;
		int32 threadCountMinusOne = threadCount - 1;

		workloadOffset = ~(y + (threadCountMinusOne - threadIndex)) & threadCountMinusOne;
		workloadIncrement = threadCount;
	}

	__forceinline static bool IsTriangleCulled(CullMode cullMode, const vfloat3 &v1, const vfloat3 &v2, const vfloat3 &v3, bool &isFrontFace)
	{
		vfloat3 normal = (v2 - v1).CrossProduct(v3 - v1);
		vfloat3 center = vfloat3(v1 + v2 + v3) * (1.0f / 3.0f);
		isFrontFace = normal.Normalize().DotProduct(center.Normalize()) < 0;

		return cullMode != CullMode::None && (cullMode == CullMode::Front) == isFrontFace;
	}
	__forceinline static vfloat3 ToClipSpace(const vfloat3 &position, int32 screenWidth, int32 screenHeight, float zoom, float clipNear)
	{
		// Transform view space coordinates into clip space.
		// Clip space coordinates X and Y range from -1 to 1. Z ranges from 1 to 0 (1 = near clipping plane, 0 = infinitely far away).

		float d = zoom / position.Z;

		return vfloat3(
			position.X * d,
			position.Y * d * screenWidth / screenHeight,
			clipNear / position.Z
		);
	}
	__forceinline static int32 GetTriangleFixExponent(const vfloat2 &v1, const vfloat2 &v2, const vfloat2 &v3)
	{
		// Compute proper exponent for fixed points.

		// Using a constant exponent of 3 is good enough in most cases.
		// However, triangles that extend far off the screen tend to overflow.
		// A positive side effect is improved precision of the fill convention.

		// Maximum width OR height of the triangle in screen space.
		int32 size = (int32)Math::Max(
			Math::Max(v1.X, v2.X, v3.X) - Math::Min(v1.X, v2.X, v3.X),
			Math::Max(v1.Y, v2.Y, v3.Y) - Math::Min(v1.Y, v2.Y, v3.Y)
		);

		return Math::Clamp((int32)__lzcnt(size) - 16, 0, 8);
	}
	__forceinline static vint2 ToScreenSpace(const vfloat3 &position, int32 screenWidth, int32 screenHeight)
	{
		return (vint2)VectorMath::Round(ToScreenSpaceF(position, screenWidth, screenHeight));
	}
	__forceinline static vfloat2 ToScreenSpaceF(const vfloat3 &position, int32 screenWidth, int32 screenHeight)
	{
		// Transform clip space coordinates into screen coordinates.
		// A position of -1,-1 represents the top-left corner of the screen, and 1,1 the bottom-right corner.

		return vfloat2(
			(1 + position.X) * (screenWidth / 2),
			(1 - position.Y) * (screenHeight / 2)
		);
	}
	__forceinline static void ClipEdgeToScreenSpace(int32 screenWidth, int32 screenHeight, vfloat3 &v1, vfloat3 &v2, vint2 &v1Screen, vint2 &v2Screen)
	{
		if (v1Screen.X < 0 && v2Screen.X >= 0)
		{
			v1Screen.Y = Math::Interpolate(0, v1Screen.X, v2Screen.X, v1Screen.Y, v2Screen.Y);
			v1 = Math::Interpolate(0.0f, (float)v1Screen.X, (float)v2Screen.X, v1, v2);
			v1Screen.X = 0;
		}

		if (v1Screen.Y < 0 && v2Screen.Y >= 0)
		{
			v1Screen.X = Math::Interpolate(0, v1Screen.Y, v2Screen.Y, v1Screen.X, v2Screen.X);
			v1 = Math::Interpolate(0.0f, (float)v1Screen.Y, (float)v2Screen.Y, v1, v2);
			v1Screen.Y = 0;
		}

		if (v1Screen.X >= screenWidth && v2Screen.X < screenWidth)
		{
			v1Screen.Y = Math::Interpolate(screenWidth - 1, v1Screen.X, v2Screen.X, v1Screen.Y, v2Screen.Y);
			v1 = Math::Interpolate((float)(screenWidth - 1), (float)v1Screen.X, (float)v2Screen.X, v1, v2);
			v1Screen.X = screenWidth - 1;
		}

		if (v1Screen.Y >= screenHeight && v2Screen.Y < screenHeight)
		{
			v1Screen.X = Math::Interpolate(screenHeight - 1, v1Screen.Y, v2Screen.Y, v1Screen.X, v2Screen.X);
			v1 = Math::Interpolate((float)(screenHeight - 1), (float)v1Screen.Y, (float)v2Screen.Y, v1, v2);
			v1Screen.Y = screenHeight - 1;
		}
	}
	__forceinline static bool IsTriangleOutsideClipVolume(const vfloat3 &v1, const vfloat3 &v2, const vfloat3 &v3)
	{
		return
			v1.X < -1 && v2.X < -1 && v3.X < -1 ||
			v1.Y < -1 && v2.Y < -1 && v3.Y < -1 ||
			v1.X > 1 && v2.X > 1 && v3.X > 1 ||
			v1.Y > 1 && v2.Y > 1 && v3.Y > 1;
	}
	__forceinline static vfloat3 DecomposeProjection(int32 screenWidth, int32 screenHeight, int32 screenX, int32 screenY, float depthBufferValue, float zoom)
	{
		float d = (screenWidth / 2) * zoom * depthBufferValue;

		return vfloat3(
			(screenX - screenWidth / 2) / d,
			-(screenY - screenHeight / 2) / d,
			1 / depthBufferValue
		);
	}

	__forceinline static void MakeTextureCoordinatesPositive(vfloat2 &v1, vfloat2 &v2, vfloat2 &v3)
	{
		vfloat2 offset = vfloat2(
			Math::Floor(1 - Math::Min(v1.X, v2.X, v3.X)),
			Math::Floor(1 - Math::Min(v1.Y, v2.Y, v3.Y))
		);

		v1 += offset;
		v2 += offset;
		v3 += offset;
	}
	__forceinline static void GetMipLevel(float inverseDeltaXU, float inverseDeltaXV, float inverseDeltaYU, float inverseDeltaYV, int32 maxMipLevels, int32 &mipLevelU, int32 &mipLevelV)
	{
		// Smash together all integer's bits to find the position of the first set bit.

		int32 inversePixelSizeU = Math::Abs((int32)inverseDeltaXU) | Math::Abs((int32)inverseDeltaYU);
		int32 inversePixelSizeV = Math::Abs((int32)inverseDeltaXV) | Math::Abs((int32)inverseDeltaYV);

		// Example:
		//   - inversePixelSize = 32
		//   - Means, the pixel is 32 times smaller than a pixel on the screen
		//   - Result: MipLap level is 6

		mipLevelU = Math::Min(32 - (int32)__lzcnt(inversePixelSizeU), maxMipLevels);
		mipLevelV = Math::Min(32 - (int32)__lzcnt(inversePixelSizeV), maxMipLevels);
	}
};