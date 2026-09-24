#include "ThreadPool.h"

int32 ThreadPool::NextThreadId = 1;
SRWLOCK ThreadPool::Mutex = SRWLOCK_INIT;
ThreadPool::Worker ThreadPool::Workers[WorkerCount];

int32 ThreadPool::Start(std::function<void()> function)
{
	AcquireSRWLockExclusive(&Mutex);

	for (int32 i = 0; i < WorkerCount; i++)
	{
		Worker &worker = Workers[i];

		if (!worker.Thread)
		{
			worker.Thread = CreateThread(nullptr, 0, ThreadFunc, &worker, 0, nullptr);

			if (!worker.Thread)
			{
				ReleaseSRWLockExclusive(&Mutex);
				throw std::runtime_error("Failed to create thread pool worker.");
			}
		}

		if (!worker.IsRunning)
		{
			int32 threadId = NextThreadId++;

			worker.ThreadId = threadId;
			worker.Function = std::move(function);
			worker.IsRunning = true;

			ReleaseSRWLockExclusive(&Mutex);
			WakeConditionVariable(&worker.Condition);
			return threadId;
		}
	}

	ReleaseSRWLockExclusive(&Mutex);
	throw std::runtime_error("The maximum number of threads in the thread pool has been reached.");
}
void ThreadPool::Join(int32 threadId)
{
	AcquireSRWLockExclusive(&Mutex);

	if (threadId <= 0 || threadId >= NextThreadId)
	{
		ReleaseSRWLockExclusive(&Mutex);
		throw std::invalid_argument("Invalid thread ID.");
	}

	for (int32 i = 0; i < WorkerCount; i++)
	{
		Worker &worker = Workers[i];

		if (worker.ThreadId == threadId)
		{
			while (worker.IsRunning && worker.ThreadId == threadId)
			{
				SleepConditionVariableSRW(&worker.Condition, &Mutex, INFINITE, 0);
			}

			ReleaseSRWLockExclusive(&Mutex);
			return;
		}
	}

	ReleaseSRWLockExclusive(&Mutex);
}
void ThreadPool::Run(std::initializer_list<std::function<void()>> functions)
{
	int32 threadIds[WorkerCount];
	int32 count = 0;

	for (const std::function<void()> &function : functions)
	{
		threadIds[count++] = Start(function);
	}

	for (int32 i = 0; i < count; i++)
	{
		Join(threadIds[i]);
	}
}
void ThreadPool::Run(int32 count, std::function<void(WorkPartition)> function)
{
	int32 threadIds[WorkerCount];

	for (int32 i = 0; i < count; i++)
	{
		WorkPartition workPartition = WorkPartition(i, count);

		threadIds[i] = Start([&function, workPartition]
		{
			function(workPartition);
		});
	}

	for (int32 i = 0; i < count; i++)
	{
		Join(threadIds[i]);
	}
}
void ThreadPool::Run(int32 count, std::function<void(int32 threadIndex)> function)
{
	int32 threadIds[WorkerCount];

	for (int32 i = 0; i < count; i++)
	{
		threadIds[i] = Start([&function, i]
		{
			function(i);
		});
	}

	for (int32 i = 0; i < count; i++)
	{
		Join(threadIds[i]);
	}
}
void ThreadPool::Run(int32 count, std::function<void(int32 threadIndex, int32 threadCount)> function)
{
	int32 threadIds[WorkerCount];

	for (int32 i = 0; i < count; i++)
	{
		threadIds[i] = Start([&function, i, count]
		{
			function(i, count);
		});
	}

	for (int32 i = 0; i < count; i++)
	{
		Join(threadIds[i]);
	}
}

DWORD WINAPI ThreadPool::ThreadFunc(void *data)
{
	Worker *worker = (Worker*)data;
	_MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

	while (true)
	{
		AcquireSRWLockExclusive(&Mutex);

		while (!worker->IsRunning)
		{
			SleepConditionVariableSRW(&worker->Condition, &Mutex, INFINITE, 0);
		}

		std::function<void()> function = std::move(worker->Function);

		ReleaseSRWLockExclusive(&Mutex);

		function();

		AcquireSRWLockExclusive(&Mutex);
		worker->IsRunning = false;
		ReleaseSRWLockExclusive(&Mutex);
		WakeAllConditionVariable(&worker->Condition);
	}

	return 0;
}