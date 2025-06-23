#pragma once
#include "../FastPix3D.h"
#include "System.h"

class FASTPIX3D_API Stopwatch
{
private:
	bool _IsRunning;
	int64 _ElapsedMicroseconds;
	int64 _StartTime;

public:
	readonly_property(bool, IsRunning)
	{
		return _IsRunning;
	}
	readonly_property(int64, ElapsedMilliseconds)
	{
		return ElapsedMicroseconds / 1000;
	}
	readonly_property(int64, ElapsedMicroseconds)
	{
		int64 elapsedMicroseconds = _ElapsedMicroseconds;

		if (_IsRunning)
		{
			elapsedMicroseconds += System::Microseconds() - _StartTime;
		}

		return elapsedMicroseconds;
	}

	Stopwatch();
	static Stopwatch StartNew();

	void Start();
	void Stop();
	void Reset();
	void Restart();
	void Toggle();
};