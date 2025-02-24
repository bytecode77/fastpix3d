#include "FPSCounter.h"
#include "System.h"

FPSCounter::FPSCounter() :
	_Interval(1000),
	_LastFrameTimeStamp(0),
	_FrameCount(0),
	_RenderTimeCount(0),
	_FPS(0),
	_FrameTime(0),
	_LastFrameTime(0),
	_MinFrameTime(-1)
{
}
FPSCounter::FPSCounter(int32 interval) :
	_Interval(interval),
	_LastFrameTimeStamp(0),
	_FrameCount(0),
	_RenderTimeCount(0),
	_FPS(0),
	_FrameTime(0),
	_LastFrameTime(0),
	_MinFrameTime(-1)
{
}

bool FPSCounter::Frame()
{
	int64 time = System::Microseconds();

	_FrameCount++;
	_LastFrameTime = (int32)(time - _LastFrameTimeStamp);
	_RenderTimeCount += _LastFrameTime;
	_LastFrameTimeStamp = time;

	if (_MinFrameTime == -1 || _LastFrameTime < _MinFrameTime)
	{
		_MinFrameTime = _LastFrameTime;
	}

	if (_RenderTimeCount > _Interval * 1000)
	{
		_FPS = (int32)(_FrameCount * 1000000 / _RenderTimeCount);
		_FrameTime = (int32)(_RenderTimeCount / _FrameCount);
		_FrameCount = 0;
		_RenderTimeCount = 0;

		return true;
	}
	else
	{
		return false;
	}
}
void FPSCounter::ResetMinFrameTime()
{
	_MinFrameTime = -1;
}