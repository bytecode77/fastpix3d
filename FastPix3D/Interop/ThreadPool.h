#pragma once
#include "../FastPix3D.h"
#include "../WorkPartition.h"

class FASTPIX3D_API ThreadPool
{
private:
	struct Worker
	{
		int32 ThreadId = 0;
		HANDLE Thread = nullptr;
		CONDITION_VARIABLE Condition = CONDITION_VARIABLE_INIT;
		std::function<void()> Function;
		bool IsRunning = false;
	};

	static const int32 WorkerCount = 256;
	static int32 NextThreadId;
	static SRWLOCK Mutex;
	static Worker Workers[WorkerCount];

public:
	ThreadPool() = delete;

	static int32 Start(std::function<void()> function);
	static void Join(int32 threadId);
	static void Run(std::initializer_list<std::function<void()>> functions);
	static void Run(int32 count, std::function<void(WorkPartition)> function);
	static void Run(int32 count, std::function<void(int32 threadIndex)> function);
	static void Run(int32 count, std::function<void(int32 threadIndex, int32 threadCount)> function);

private:
	static DWORD WINAPI ThreadFunc(void *data);
};