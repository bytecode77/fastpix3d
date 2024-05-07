#pragma once
#include "FastPix3D.h"
#include "Vector2f.h"
#include "Vector3f.h"

class Math
{
private:
	static constexpr float RadiansToDegrees = (float)M_PI / 180;
	static bool IsRandomSeedInitialized;

	static void EnsureRandomSeedInitialized()
	{
		if (!IsRandomSeedInitialized)
		{
			srand((uint32)GetTickCount64());
			IsRandomSeedInitialized = true;
		}
	}

public:
	Math() = delete;

	static float Sin(float angle)
	{
		return sinf(angle * RadiansToDegrees);
	}
	static float Cos(float angle)
	{
		return cosf(angle * RadiansToDegrees);
	}
	template<class T>
	static T Min(T a, T b)
	{
		return a < b ? a : b;
	}
	template<class T>
	static T Max(T a, T b)
	{
		return a > b ? a : b;
	}
	template<class T>
	static T Clamp(T value, T min, T max)
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
	static int32 Abs(int32 value)
	{
		return abs(value);
	}
	static float Abs(float value)
	{
		return fabsf(value);
	}
	static float Floor(float value)
	{
		return floorf(value);
	}
	static int32 Interpolate(int32 value, int32 valueMin, int32 valueMax, int32 returnMin, int32 returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	static float Interpolate(float value, float valueMin, float valueMax, float returnMin, float returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	static Vector2f Interpolate(const Vector2f &value, const Vector2f &valueMin, const Vector2f &valueMax, const Vector2f &returnMin, const Vector2f &returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	static Vector2f Interpolate(float value, float valueMin, float valueMax, const Vector2f &returnMin, const Vector2f &returnMax)
	{
		return returnMin + (returnMax - returnMin) * ((value - valueMin) / (valueMax - valueMin));
	}
	static Vector3f Interpolate(const Vector3f &value, const Vector3f &valueMin, const Vector3f &valueMax, const Vector3f &returnMin, const Vector3f &returnMax)
	{
		return returnMin + (returnMax - returnMin) * (value - valueMin) / (valueMax - valueMin);
	}
	static Vector3f Interpolate(float value, float valueMin, float valueMax, const Vector3f &returnMin, const Vector3f &returnMax)
	{
		return returnMin + (returnMax - returnMin) * ((value - valueMin) / (valueMax - valueMin));
	}
	static int32 GetLogarithmicCeiling(int32 number)
	{
		return 1 << (32 - __lzcnt(number - 1));
	}
	static int32 GetExponent(int32 number)
	{
		return 31 - __lzcnt(number);
	}

	static int64 Time()
	{
		return SDL_GetTicks64();
	}
	static int32 Random(int32 min, int32 maxExclusive)
	{
		EnsureRandomSeedInitialized();

		return rand() * (maxExclusive - min - 1) / RAND_MAX + min;
	}
};