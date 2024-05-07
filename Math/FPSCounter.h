#pragma once
#include "FastPix3D.h"

class FPSCounter
{
private:
	int32 _Interval;
	int64 _LastTime;
	int64 _LastIntervalTime;
	int32 _FrameCount;
	int32 _FPS;
	int32 _FrameTime;
	int32 _AverageFrameTime;

public:
	property_get(int32, FPS)
	{
		return _FPS;
	}
	property_get(int32, FrameTime)
	{
		return _FrameTime;
	}
	property_get(int32, AverageFrameTime)
	{
		return _AverageFrameTime;
	}

	FPSCounter();
	explicit FPSCounter(int32 interval);

	bool Frame();
};