#include "FPSCounter.h"
#include "Math_.h"

FPSCounter::FPSCounter() :
	_Interval(1000),
	_LastTime(Math::Time()),
	_LastIntervalTime(Math::Time()),
	_FrameCount(0),
	_FPS(0),
	_FrameTime(0),
	_AverageFrameTime(0)
{
}
FPSCounter::FPSCounter(int32 interval) :
	_Interval(interval),
	_LastTime(Math::Time()),
	_LastIntervalTime(Math::Time()),
	_FrameCount(0),
	_FPS(0),
	_FrameTime(0),
	_AverageFrameTime(0)
{
}

bool FPSCounter::Frame()
{
	int64 time = Math::Time();

	_FrameCount++;

	_FrameTime = time - _LastTime;
	_LastTime = time;

	if (time - _LastIntervalTime > _Interval)
	{
		_FPS = _FrameCount * 1000 / _Interval;
		_AverageFrameTime = _FrameCount == 0 ? 0 : _Interval / _FrameCount;
		_FrameCount = 0;
		_LastIntervalTime = time;

		return true;
	}
	else
	{
		return false;
	}
}