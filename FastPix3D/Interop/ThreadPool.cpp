#include "ThreadPool.h"
#include <SDL3/SDL.h>

int32 ThreadPool::NextThreadId = 1;
SDL_Mutex *ThreadPool::Mutex;
ThreadPoolThread ThreadPool::Threads[ThreadCount];

int32 ThreadPool::Start(std::function<void()> function)
{
	if (!Mutex)
	{
		Mutex = SDL_CreateMutex();
	}

	int32 threadId = -1;

	SDL_LockMutex(Mutex);

	for (int32 i = 0; i < ThreadCount; i++)
	{
		ThreadPoolThread *thread = &Threads[i];

		if (!thread->Thread)
		{
			thread->Thread = new Thread([thread]
			{
				thread->ThreadFunction();
			});
		}

		if (!thread->Thread->IsRunning)
		{
			thread->ThreadId = NextThreadId++;
			thread->ThreadFunction = function;
			thread->Thread->Start();

			threadId = thread->ThreadId;
			break;
		}
	}

	SDL_UnlockMutex(Mutex);

	return threadId;
}
void ThreadPool::Join(int32 threadId)
{
	if (threadId != -1)
	{
		for (int32 i = 0; i < ThreadCount; i++)
		{
			ThreadPoolThread *thread = &Threads[i];

			if (!thread->Thread)
			{
				break;
			}
			else if (thread->ThreadId == threadId)
			{
				thread->Thread->Join();
				break;
			}
		}
	}
}