#include "Stopwatch.h"

Stopwatch::Stopwatch() :
	_IsRunning(false),
	_ElapsedMicroseconds(0),
	_StartTime(0)
{
}
Stopwatch Stopwatch::StartNew()
{
	Stopwatch stopwatch;
	stopwatch.Start();

	return stopwatch;
}

void Stopwatch::Start()
{
	if (!_IsRunning)
	{
		_IsRunning = true;
		_StartTime = System::Microseconds();
	}
}
void Stopwatch::Stop()
{
	if (_IsRunning)
	{
		_ElapsedMicroseconds += System::Microseconds() - _StartTime;
		_IsRunning = false;
	}
}
void Stopwatch::Reset()
{
	_IsRunning = false;
	_ElapsedMicroseconds = 0;
}
void Stopwatch::Restart()
{
	_IsRunning = true;
	_ElapsedMicroseconds = 0;
	_StartTime = System::Microseconds();
}
void Stopwatch::Toggle()
{
	if (_IsRunning)
	{
		Stop();
	}
	else
	{
		Start();
	}
}