#include "System.h"

INIT_ONCE System::InitOnce = INIT_ONCE_STATIC_INIT;
LARGE_INTEGER System::Start;
double System::MillisecondsPerTick;
double System::MicrosecondsPerTick;
double System::NanosecondsPerTick;

int64 System::Milliseconds()
{
	EnsureInitialized();

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);

	return (int64)((count.QuadPart - Start.QuadPart) * MillisecondsPerTick);
}
int64 System::Microseconds()
{
	EnsureInitialized();

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);

	return (int64)((count.QuadPart - Start.QuadPart) * MicrosecondsPerTick);
}
int64 System::Nanoseconds()
{
	EnsureInitialized();

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);

	return (int64)((count.QuadPart - Start.QuadPart) * NanosecondsPerTick);
}

void System::EnsureInitialized()
{
	InitOnceExecuteOnce(&InitOnce, Initialize, nullptr, nullptr);
}
BOOL CALLBACK System::Initialize(PINIT_ONCE initOnce, PVOID parameter, PVOID *context)
{
	QueryPerformanceCounter(&Start);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	MillisecondsPerTick = 1000.0 / frequency.QuadPart;
	MicrosecondsPerTick = 1000000.0 / frequency.QuadPart;
	NanosecondsPerTick = 1000000000.0 / frequency.QuadPart;

	return TRUE;
}