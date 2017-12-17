class FASTPIX3D_API MathUtility
{
public:
	static void Swap(int32 &x, int32 &y);
	static void Swap(int64 &x, int64 &y);
	static void Swap(float &x, float &y);
	static int32 InterpolateInt32(int32 value, int32 vMin, int32 vMax, int32 retMin, int32 retMax);
	static float Interpolate(float value, float vMin, float vMax, float retMin, float retMax);
	static void SetRandomSeed();
	static void SetRandomSeed(uint32 seed);
	static int32 Random(int32 min, int32 max);
};