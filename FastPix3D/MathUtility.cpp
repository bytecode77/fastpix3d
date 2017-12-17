#include "FastPix3D.h"

void MathUtility::Swap(int32 &x, int32 &y)
{
	int32 swap = x;
	x = y;
	y = swap;
}
void MathUtility::Swap(int64 &x, int64 &y)
{
	int64 swap = x;
	x = y;
	y = swap;
}
void MathUtility::Swap(float &x, float &y)
{
	float swap = x;
	x = y;
	y = swap;
}
int32 MathUtility::InterpolateInt32(int32 value, int32 vMin, int32 vMax, int32 retMin, int32 retMax)
{
	return retMin + (value - vMin) * (retMax - retMin) / (vMax - vMin);
}
float MathUtility::Interpolate(float value, float vMin, float vMax, float retMin, float retMax)
{
	return retMin + (value - vMin) * (retMax - retMin) / (vMax - vMin);
}
void MathUtility::SetRandomSeed()
{
	srand((uint32)time(NULL));
}
void MathUtility::SetRandomSeed(uint32 seed)
{
	srand(seed);
}
int32 MathUtility::Random(int32 min, int32 max)
{
	return rand() * (max - min + 1) / RAND_MAX + min;
}