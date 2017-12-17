#include "FastPix3D.h"

FPSCounter::FPSCounter()
{
	Interval = 1000;
	FPS = 0;
	FPSCount = 0;
	FrameTime = 0;
	MedianFrameTime = 0;
	LastMeasuredTime = clock();
}
FPSCounter::FPSCounter(int32 interval)
{
	Interval = interval;
	FPS = 0;
	FPSCount = 0;
	FrameTime = 0;
	MedianFrameTime = 0;
	LastMeasuredTime = clock();
}

void FPSCounter::Frame()
{
	clock_t time = clock();
	if (time - LastMeasuredTime > Interval)
	{
		FPS = FPSCount * 1000 / Interval;
		MedianFrameTime = FPSCount == 0 ? 0 : Interval / FPSCount;
		FPSCount = 0;
		LastMeasuredTime = time;
	}
	FrameTime = time - LastFrame;
	LastFrame = time;
	FPSCount++;
}

int32 FPSCounter::getInterval()
{
	return Interval;
}
int32 FPSCounter::getFPS()
{
	return FPS;
}
int32 FPSCounter::getFrameTime()
{
	return FrameTime;
}
int32 FPSCounter::getMedianFrameTime()
{
	return MedianFrameTime;
}

void FPSCounter::setInterval(int32 interval)
{
	Interval = interval;
}