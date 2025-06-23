#pragma once
#include "../FastPix3D.h"

class FASTPIX3D_API FPSCounter
{
private:
	const int32 _Interval;
	int64 _LastFrameTimeStamp;
	int64 _FrameCount;
	int32 _RenderTimeCount;
	int32 _FPS;
	int32 _FrameTime;
	int32 _LastFrameTime;
	int32 _MinFrameTime;

public:
	readonly_property(int32, FPS)
	{
		return _FPS;
	}
	readonly_property(int32, FrameTime)
	{
		return _FrameTime;
	}
	readonly_property(int32, LastFrameTime)
	{
		return _LastFrameTime;
	}
	readonly_property(int32, MinFrameTime)
	{
		return _MinFrameTime;
	}

	FPSCounter();
	explicit FPSCounter(int32 interval);

	bool Frame();
	void ResetMinFrameTime();
};