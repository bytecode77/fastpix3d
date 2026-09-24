#include "Stopwatch.h"
#include "../Interop/System.h"

Stopwatch::Stopwatch() :
	_IsRunning(false),
	_ElapsedNanoseconds(0),
	_StartTime(0)
{
}
Stopwatch Stopwatch::StartNew()
{
	Stopwatch stopwatch;
	stopwatch.Start();

	return stopwatch;
}

int64 Stopwatch::get_ElapsedNanoseconds() const
{
	int64 elapsedNanoseconds = _ElapsedNanoseconds;

	if (_IsRunning)
	{
		elapsedNanoseconds += System::Nanoseconds() - _StartTime;
	}

	return elapsedNanoseconds;
}

void Stopwatch::Start()
{
	if (!_IsRunning)
	{
		_IsRunning = true;
		_StartTime = System::Nanoseconds();
	}
}
void Stopwatch::Stop()
{
	if (_IsRunning)
	{
		_ElapsedNanoseconds += System::Nanoseconds() - _StartTime;
		_IsRunning = false;
	}
}
void Stopwatch::Reset()
{
	_IsRunning = false;
	_ElapsedNanoseconds = 0;
}
void Stopwatch::Restart()
{
	_IsRunning = true;
	_ElapsedNanoseconds = 0;
	_StartTime = System::Nanoseconds();
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