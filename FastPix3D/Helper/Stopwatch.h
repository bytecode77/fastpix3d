#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API Stopwatch
{
private:
	bool _IsRunning;
	int64 _ElapsedNanoseconds;
	int64 _StartTime;

public:
	readonly_property(bool, IsRunning)
	{
		return _IsRunning;
	}
	readonly_property(int64, ElapsedMilliseconds)
	{
		return ElapsedNanoseconds / 1000000;
	}
	readonly_property(int64, ElapsedMicroseconds)
	{
		return ElapsedNanoseconds / 1000;
	}
	readonly_property(int64, ElapsedNanoseconds);

	Stopwatch();
	static Stopwatch StartNew();

	void Start();
	void Stop();
	void Reset();
	void Restart();
	void Toggle();
};