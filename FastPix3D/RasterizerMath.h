#pragma once
#include "FastPix3D.h"
#include "Math/Math_.h"
#include "Math/VectorMath.h"
#include <bit>

// Commonly used algorithms.
//   * All of them are inlined by the compiler (I have checked the assembly).
//   * Branchless implementations where possible.

class RasterizerMath
{
public:
	RasterizerMath() = delete;

	__forceinline static bool IsTriangleCulled(CullMode cullMode, const vfloat3 &v1, const vfloat3 &v2, const vfloat3 &v3)
	{
		if (cullMode == CullMode::None)
		{
			return false;
		}
		else
		{
			bool isFrontFace = v1.DotProduct(v2.CrossProduct(v3)) < 0;
			return (cullMode == CullMode::Front) == isFrontFace;
		}
	}
	__forceinline static bool IsTriangleCulled(CullMode cullMode, const vfloat3 &v1, const vfloat3 &v2, const vfloat3 &v3, bool &isFrontFace)
	{
		isFrontFace = v1.DotProduct(v2.CrossProduct(v3)) < 0;
		return cullMode != CullMode::None && (cullMode == CullMode::Front) == isFrontFace;
	}
	__forceinline static vfloat3 Project(const vfloat3 &position, const vfloat3 &scale)
	{
		// Perspective-project view-space coordinates.
		// X/Y range from -1 to 1 across the viewport.
		// Z is reverse depth (1 = near clipping plane, 0 = infinitely far away)

		return vfloat3(position.X, position.Y, 1) * scale / position.Z;
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
		// Transform projected X/Y coordinates into screen coordinates.
		// (-1, 1) is the top-left and (1, -1) is the bottom-right.

		return vfloat2(
			(1 + position.X) * (screenWidth >> 1),
			(1 - position.Y) * (screenHeight >> 1)
		);
	}
	__forceinline static bool IsTriangleOutsideClipVolume(const vfloat3 &v1, const vfloat3 &v2, const vfloat3 &v3)
	{
		return
			v1.X < -1 && v2.X < -1 && v3.X < -1 ||
			v1.Y < -1 && v2.Y < -1 && v3.Y < -1 ||
			v1.X > 1 && v2.X > 1 && v3.X > 1 ||
			v1.Y > 1 && v2.Y > 1 && v3.Y > 1;
	}
	__forceinline static vfloat3 DecomposeProjection(int32 screenWidth, int32 screenHeight, int32 screenX, int32 screenY, float depthBufferValue, float zoom, float clipNear)
	{
		float d = (screenWidth >> 1) * zoom * depthBufferValue;

		return vfloat3(
			(screenX - (screenWidth >> 1)) * clipNear / d,
			-(screenY - (screenHeight >> 1)) * clipNear / d,
			clipNear / depthBufferValue
		);
	}
	__forceinline static void MakeTextureCoordinatesPositive(vfloat2 &v1, vfloat2 &v2, vfloat2 &v3)
	{
		vfloat2 offset = vfloat2(
			Math::Floor(Math::Min(v1.X, v2.X, v3.X) - .5f),
			Math::Floor(Math::Min(v1.Y, v2.Y, v3.Y) - .5f)
		);

		// Subtract 0.5 to guarantee U/V >= 0.5, because the bilinear filter subtracts 0.5.

		v1 -= offset;
		v2 -= offset;
		v3 -= offset;
	}
};