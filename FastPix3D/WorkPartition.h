#pragma once
#include "FastPix3D.h"

class WorkPartition
{
private:
	int32 _ThreadIndex;
	int32 _ThreadCount;

public:
	readonly_property(int32, ThreadIndex)
	{
		return _ThreadIndex;
	}
	readonly_property(int32, ThreadCount)
	{
		return _ThreadCount;
	}

	WorkPartition() :
		_ThreadIndex(0),
		_ThreadCount(1)
	{
	}
	WorkPartition(int32 threadIndex, int32 threadCount) :
		_ThreadIndex(threadIndex),
		_ThreadCount(threadCount)
	{
		if (threadIndex < 0 || threadIndex >= threadCount) throw std::out_of_range("threadIndex must be in the range 0..threadCount.");
		if (threadCount != 1 && threadCount != 2 && threadCount != 4 && threadCount != 8 && threadCount != 16 && threadCount != 32) throw std::invalid_argument("threadCount must be 1, 2, 4, 8, 16 or 32.");
	}

	int32 GetOffset(int32 y) const
	{
		// Partition the workload so that every thread is assigned to an exclusive subset of the screen.

		// For 4 threads that is:
		//   - Thread 1 renders row 0-7
		//   - Thread 2 renders row 8-15
		//   - Thread 3 renders row 16-23
		//   - Thread 4 renders row 24-31

		// The fifth band of 8 rows is rendered by thread 1 again, and so on.

		// offset: The first line that *THIS* thread will render.
		// increment: The vertical increment after each line is rendered.

		return (_ThreadIndex - y) & (_ThreadCount - 1);
	}
};