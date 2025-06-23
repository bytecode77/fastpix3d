#pragma once
#include "../FastPix3D.h"
#include "VectorMath.h"

class FASTPIX3D_API Math
{
private:
	static constexpr float RadiansToDegrees = (float)M_PI / 180;
	static bool IsRandomSeedInitialized;
	static uint32 Crc32Table[256];

public:
	Math() = delete;

	__forceinline static float Pow(float x, float y)
	{
		return powf(x, y);
	}
	__forceinline static float Sin(float angle)
	{
		return sinf(angle * RadiansToDegrees);
	}
	__forceinline static float Cos(float angle)
	{
		return cosf(angle * RadiansToDegrees);
	}
	__forceinline static float Tan(float angle)
	{
		return tanf(angle * RadiansToDegrees);
	}
	template<class T>
	__forceinline static T Min(T a, T b)
	{
		return a < b ? a : b;
	}
	template<class T>
	__forceinline static T Min(T a, T b, T c)
	{
		return Min(Min(a, b), c);
	}
	template<class T>
	__forceinline static T Max(T a, T b)
	{
		return a > b ? a : b;
	}
	template<class T>
	__forceinline static T Max(T a, T b, T c)
	{
		return Max(Max(a, b), c);
	}
	template<class T>
	__forceinline static T Clamp(T value, T min, T max)
	{
		if (value < min)
		{
			return min;
		}
		else if (value > max)
		{
			return max;
		}
		else
		{
			return value;
		}
	}
	__forceinline static int32 Abs(int32 value)
	{
		return abs(value);
	}
	__forceinline static float Abs(float value)
	{
		return fabsf(value);
	}
	__forceinline static float Floor(float value)
	{
		return floorf(value);
	}
	__forceinline static float Ceil(float value)
	{
		return ceilf(value);
	}
	__forceinline static float Round(float value)
	{
		return roundf(value);
	}
	__forceinline static int32 Interpolate(int32 value, int32 valueMin, int32 valueMax, int32 returnMin, int32 returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	__forceinline static float Interpolate(float value, float valueMin, float valueMax, float returnMin, float returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	__forceinline static vfloat2 Interpolate(const vfloat2 &value, const vfloat2 &valueMin, const vfloat2 &valueMax, const vfloat2 &returnMin, const vfloat2 &returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	__forceinline static vfloat2 Interpolate(float value, float valueMin, float valueMax, const vfloat2 &returnMin, const vfloat2 &returnMax)
	{
		return returnMin + (returnMax - returnMin) * ((value - valueMin) / (valueMax - valueMin));
	}
	__forceinline static vfloat3 Interpolate(const vfloat3 &value, const vfloat3 &valueMin, const vfloat3 &valueMax, const vfloat3 &returnMin, const vfloat3 &returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	__forceinline static vfloat3 Interpolate(float value, float valueMin, float valueMax, const vfloat3 &returnMin, const vfloat3 &returnMax)
	{
		return returnMin + (returnMax - returnMin) * ((value - valueMin) / (valueMax - valueMin));
	}
	__forceinline static int32 GetLogarithmicCeiling(int32 number)
	{
		return 1 << (32 - __lzcnt(number - 1));
	}
	__forceinline static int32 GetExponent(int32 number)
	{
		return 31 - __lzcnt(number);
	}

	static int32 Random(int32 min, int32 maxExclusive);
	static uint32 ComputeCRC32(const void *buffer, int32 size);
};