#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API System
{
private:
	static INIT_ONCE InitOnce;
	static LARGE_INTEGER Start;
	static double MillisecondsPerTick;
	static double MicrosecondsPerTick;
	static double NanosecondsPerTick;

public:
	System() = delete;

	static int64 Milliseconds();
	static int64 Microseconds();
	static int64 Nanoseconds();

private:
	static void EnsureInitialized();
	static BOOL CALLBACK Initialize(PINIT_ONCE initOnce, PVOID parameter, PVOID *context);
};