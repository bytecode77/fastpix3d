#pragma once
#include "../FastPix3D.h"
#include "Thread.h"

struct SDL_Mutex;

class ThreadPoolThread
{
private:
	Thread *Thread;
	int32 ThreadId;
	std::function<void()> ThreadFunction;

	friend class ThreadPool;
};

class FASTPIX3D_API ThreadPool
{
private:
	static const int32 ThreadCount = 256;
	static int32 NextThreadId;
	static SDL_Mutex *Mutex;
	static ThreadPoolThread Threads[ThreadCount];

public:
	static int32 Start(std::function<void()> function);
	static void Join(int32 threadId);
};