#pragma once
#include "FastPix3D.h"

typedef void(*ThreadFunction)(void *sender, void *parameter);

class Thread
{
private:
	const ThreadFunction Function;
	void *Sender;
	void *Parameter;

	SDL_Mutex *Mutex;
	SDL_Condition *Cond;
	SDL_Thread *SDLThread;
	bool IsRunning;

public:
	Thread(ThreadFunction function, void *sender, void *parameter);
	~Thread();

	void Start();
	void Join();

private:
	static int ThreadFunc(void *data);
};